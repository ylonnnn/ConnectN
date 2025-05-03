#pragma once

#include "Player.h"
#include "types.h"
#include <array>
#include <memory>
#include <vector>

class ConnectN {
protected:
  int n;

  GridSize gridSize;
  Grid grid;

  std::array<std::unique_ptr<Player>, 2> &players;
  Player *winner;

  int turn;

  void initializeGrid();
  void initializeTurn();

  int opposingTurn(int turn);
  void updateTurn();

public:
  ConnectN(GridSize gridSize, std::array<std::unique_ptr<Player>, 2> &players);
  ConnectN(int n, GridSize gridSize,
           std::array<std::unique_ptr<Player>, 2> &players);

  GridSize getGridSize() const;
  Grid &getGrid();

  int getTurn();
  int getTurn(const Player *player);

  bool hasMark(int position[2]);
  bool hasMark(int position[2], CellState state);
  bool exceedsGrid(int position[2]);

  void mapGrid();
  bool placeMark(int turn, int col);

  int requiredMoves(int turn, Orientation orientation);

  void check(int turn);
  void check(int turn, int position[2]);

  Orientation optimalOrientation(int turn, int position[2]);
  // Orientation optimalOrientation(int turn);

  std::vector<Orientation> horizontalOrientations(int turn, int position[2]);
  std::vector<Orientation> verticalOrientations(int turn, int position[2]);
  std::vector<Orientation> diagonalOrientations(int turn, int position[2]);

  void start();
  void process();
  void end();
};
