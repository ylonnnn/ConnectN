#include <algorithm>
#include <cmath>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "../include/ConnectN.h"
#include "../include/types.h"
#include "../include/utils.h"

ConnectN::ConnectN(int n, GridSize gridSize,
                   std::array<std::unique_ptr<Player>, 2> &players)
    : n(n), gridSize(gridSize), players(players), winner(nullptr) {

  initializeTurn();
  initializeGrid();
}

ConnectN::ConnectN(GridSize gridSize,
                   std::array<std::unique_ptr<Player>, 2> &players)
    : n(4), gridSize(gridSize), players(players), winner(nullptr) {

  initializeTurn();
  initializeGrid();
}

void ConnectN::initializeGrid() {
  int nMin = 1, nMax = ((gridSize[0] * gridSize[1]) / 2);
  if (nMin > n && n > nMax)
    throw std::range_error(
        "n must be within " +
        std::string({(char)('0' + nMin), '-', (char)('0' + nMax)}));

  if (std::min(gridSize[0], gridSize[1]) <= n)
    throw std::range_error("Grid rows and columns must at least be " +
                           std::string({(char)('0' + n)}));

  grid = std::vector<std::vector<Cell>>(gridSize[0]);

  for (int i = 0; i < gridSize[0]; i++) {
    grid[i] = std::vector<Cell>(gridSize[1]);

    for (int j = 0; j < gridSize[1]; j++) {
      grid[i][j] = {CellState::Empty, {i, j}};

      std::cout << i << " : " << j << "\t";
    }

    std::cout << "\n";
  }
}

void ConnectN::initializeTurn() { turn = std::round(random() + 1); }

int ConnectN::opposingTurn(int turn) { return turn == 1 ? 2 : 1; }
void ConnectN::updateTurn() { turn = opposingTurn(turn); }

GridSize ConnectN::getGridSize() const { return gridSize; }
Grid &ConnectN::getGrid() { return grid; }

int ConnectN::getTurn() { return turn; }
int ConnectN::getTurn(const Player *player) {
  return players[0].get() == player ? 1 : 2;
}

bool ConnectN::hasMark(int position[2]) {
  return !hasMark(position, CellState::Empty);
}

bool ConnectN::hasMark(int position[2], CellState state) {
  if (exceedsGrid(position))
    return false;

  return grid[position[0]][position[1]].state == state;
}

bool ConnectN::exceedsGrid(int position[2]) {
  int row = position[0], col = position[1];

  return (row < 0 || row > (gridSize[0] - 1)) ||
         (col < 0 || col > (gridSize[1] - 1));
}

void ConnectN::mapGrid() {
  int row = gridSize[0], col = gridSize[1];

  const std::string singleLine = "|---------------";
  std::string line = "", columnLine = "|";

  for (int i = 0; i < col; i++) {
    columnLine += "\t" + std::string({(char)('0' + (i + 1))}) + "\t|";
    line += singleLine;
  }

  line += "|";
  std::cout << "\n" << line << "\n" << columnLine << "\n" << line << "\n";

  for (int i = 0; i < row; i++) {
    std::cout << "|\t";

    for (int j = 0; j < col; j++) {
      Cell &cell = grid[i][j];

      std::cout << (cell.state == CellState::Empty
                        ? "   "
                        : (cell.state == CellState::Player1 ? "[1]" : "[2]"))
                << "\t";

      if (j < (col - 1))
        std::cout << "|\t";
    }

    std::cout << "|\n" << line << "\n";
  }

  std::cout << "\n";
}

bool ConnectN::placeMark(int turn, int col) {
  // Normalized for vector indexing
  --col;

  int position[2]{-1};

  for (int i = grid.size() - 1; i > -1; i--) {
    Cell &cell = grid[i][col];

    if (cell.state != CellState::Empty)
      continue;

    position[0] = i, position[1] = col;
    cell.state = (CellState)(turn);

    break;
  }

  // If the position is unchanged, every cell within the row is full
  if (position[0] == -1) {
    std::cout << "You can no longer place marks on Column #" << col + 1
              << "!\n";

    return false;
  }

  // Check if the mark placement led the current turn to win
  // check(turn, position);

  return true;
}

int ConnectN::requiredMoves(int turn, Orientation orientation) {
  int moves = 0;
  CellState turnState = (CellState)(turn),
            oppState = (CellState)(opposingTurn(turn));

  std::unordered_map<int, bool> countedCells;

  for (const Cell &cell : orientation) {
    int row = cell.position[0], col = cell.position[1],
        rep = (row * gridSize[0]) + (col + 1);

    if (cell.state == oppState)
      return n;

    if (cell.state != turnState) {
      countedCells[rep] = true;

      moves++;
    }

    int below[2]{row + 1, col};

    if (!exceedsGrid(below) && !hasMark(below)) {
      for (int i = below[0]; i < (gridSize[0] - 1); i++) {
        if (countedCells.find(rep) != countedCells.end())
          break;

        int pos[2]{i, col};
        if (hasMark(pos))
          break;

        countedCells[rep] = true;

        moves++;
      }
    }
  }

  return moves;
}

void ConnectN::check(int turn) {
  for (int i = 0; i < gridSize[0]; i++) {
    for (int j = 0; j < gridSize[1]; j++) {
      if (winner != nullptr)
        return;

      int pos[2]{i, j};
      if (grid[i][j].state != (CellState)(turn))
        continue;

      check(turn, pos);
    }
  }
}

void ConnectN::check(int turn, int position[2]) {
  Orientation optimal = optimalOrientation(turn, position);
  int required = requiredMoves(turn, optimal);

  if (required == 0) {
    winner = std::move(players[turn - 1]).get();
  }
}

Orientation ConnectN::optimalOrientation(int turn, int position[2]) {
  Orientation orientation;
  int reqMoves = INT_MAX;

  std::vector<Orientation> horizontal = horizontalOrientations(turn, position),
                           vertical = verticalOrientations(turn, position),
                           diagonal = diagonalOrientations(turn, position);

  int hSize = horizontal.size(), vSize = vertical.size(),
      dgSize = diagonal.size();

  std::function<void(int, Orientation &)> optimal =
      [&](int turn, Orientation &_orientation) -> void {
    //
    if (_orientation.size() == 0)
      return;

    std::function<void(Orientation &, int)> update =
        [&](Orientation &_orientation, int required) -> void {
      orientation = _orientation;
      reqMoves = required;
    };

    int required = requiredMoves(turn, _orientation);
    if (required > reqMoves)
      return;

    // If the required moves is less than the current required moves, update the
    // optimal orientation
    if (required < reqMoves)
      update(_orientation, required);

    // If the required moves
    else if ((required == reqMoves) && (random() * 100.0 < 50))
      update(_orientation, required);
  };

  for (int i = 0; i < std::max({hSize, vSize, dgSize}); i++) {
    if (i < hSize)
      optimal(turn, horizontal[i]);

    if (i < vSize)
      optimal(turn, vertical[i]);

    if (i < dgSize)
      optimal(turn, diagonal[i]);
  }

  return orientation;
}

std::vector<Orientation> ConnectN::horizontalOrientations(int turn,
                                                          int position[2]) {
  int row = position[0], col = position[1], directions[2] = {1, -1},
      directionC = sizeof(directions) / sizeof(directions[0]);

  std::vector<Orientation> orientations(directionC);
  int validOrientations = 0;

  CellState oppState = turn == 1 ? CellState::Player2 : CellState::Player1;

  for (const int &direction : directions) {
    int colN = col + (direction * (n - 1));

    // If the required marks to connect from the given row exceeds the grid,
    // skip it
    if (0 > colN || colN >= gridSize[1])
      continue;

    Orientation orientation(n);
    int validCells = 0;

    for (int i = col; i != (colN + direction); i += direction) {
      const Cell &cell = grid[row][i];

      if (cell.state == oppState)
        break;

      orientation[validCells++] = cell;
    }

    // If the valid cells within the orientation from the grid is less than
    // the required amount for a connection, the orientation is blocked by the
    // opponent of the current turn
    if (validCells < (n - 1))
      continue;

    orientations[validOrientations++] = std::move(orientation);
  }

  // Resize the orientations to the actual amount of valid orientations to free
  // some memory
  orientations.resize(validOrientations);

  return orientations;
}

std::vector<Orientation> ConnectN::verticalOrientations(int turn,
                                                        int position[2]) {
  int row = position[0], col = position[1], directions[2] = {1, -1},
      directionC = sizeof(directions) / sizeof(directions[0]);

  std::vector<Orientation> orientations(directionC);
  int validOrientations = 0;

  CellState oppState = turn == 1 ? CellState::Player2 : CellState::Player1;

  for (const int &direction : directions) {
    int rowN = row + (direction * (n - 1));

    // If the required marks to connect from the given row exceeds the grid,
    // skip it
    if (0 > rowN || rowN >= gridSize[0])
      continue;

    Orientation orientation(n);
    int validCells = 0;

    for (int i = row; i != (rowN + direction); i += direction) {
      const Cell &cell = grid[i][col];

      if (cell.state == oppState)
        break;

      orientation[validCells++] = cell;
    }

    // If the valid cells within the orientation from the grid is less than
    // the required amount for a connection, the orientation is blocked by the
    // opponent of the current turn
    if (validCells < n)
      continue;

    orientations[validOrientations++] = std::move(orientation);
  }

  // Resize the orientations to the actual amount of valid orientations to free
  // some memory
  orientations.resize(validOrientations);

  return orientations;
}

std::vector<Orientation> ConnectN::diagonalOrientations(int turn,
                                                        int position[2]) {
  int row = position[0], col = position[1], directions[2] = {1, -1},
      directionC = sizeof(directions) / sizeof(directions[0]);

  std::vector<Orientation> orientations(directionC * 2);
  int validOrientations = 0;

  CellState oppState = turn == 1 ? CellState::Player2 : CellState::Player1;

  for (const int &direction : directions) {
    for (const int &direction1 : directions) {
      int rowN = row + (direction * (n - 1)),
          colN = col + (direction1 * (n - 1));

      // If the required marks to connect from the given row and column exceeds
      // the grid, skip it
      if ((0 > rowN || rowN >= gridSize[0]) ||
          (0 > colN || colN >= gridSize[1]))
        continue;

      Orientation orientation(n);
      int validCells = 0;

      for (int i = row, j = col;
           i != (rowN + direction) || j != (colN + direction1);
           i += direction, j += direction1) {
        const Cell &cell = grid[i][j];

        if (cell.state == oppState)
          break;

        orientation[validCells++] = cell;
      }

      // If the valid cells within the orientation from the grid is less than
      // the required amount for a connection, the orientation is blocked by the
      // opponent of the current turn
      if (validCells < (n - 1))
        continue;

      orientations[validOrientations++] = std::move(orientation);
    }
  }

  // Resize the orientations to the actual amount of valid orientations to free
  // some memory
  orientations.resize(validOrientations);

  return orientations;
}

void ConnectN::start() {
  // Map the grid for the initial display
  mapGrid();

  // Initialize the turn
  initializeTurn();

  // Allow the player to play their turn
  players[turn - 1]->play(*this);
}

void ConnectN::process() {
  check(turn);

  // If a winner exists, the game ends
  if (winner != nullptr) {
    end();

    return;
  }

  // Otherwise, continue the game by constantly switching turns
  updateTurn();
  players[turn - 1]->play(*this);
}

void ConnectN::end() {
  std::cout << "\n";

  if (winner == nullptr) {
    std::cout << "DRAW\n";
  } else {
    std::cout << "WINNER: Player " << winner->getName() << "\n";
  }

  std::cout << "\n";
}