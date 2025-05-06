#include <chrono>
#include <cmath>
#include <thread>

#include "../include/Bot.h"
#include "../include/ConnectN.h"
#include "../include/utils.h"

int countEmptyCells(Orientation orientation) {
  int count = 0;

  for (const Cell &cell : orientation) {
    if (cell.state == CellState::Empty)
      count++;
  }

  return count;
}

Bot::Bot(std::string name) : Player(name), placeDelay(1'000), accuracy(50.0f) {}
Bot::Bot(std::string name, int placeDelay)
    : Player(name), placeDelay(placeDelay), accuracy(50.0f) {}
Bot::Bot(std::string name, int placeDelay, float accuracy)
    : Player(name), placeDelay(placeDelay), accuracy(accuracy) {}

int Bot::getPlaceDelay() const { return placeDelay; }
float Bot::getAccuracy() const { return accuracy; }

int Bot::decideColumn(ConnectN &game) {
  //
  const GridSize &gridSize = game.getGridSize();

  if ((random() * 100.0f) > accuracy) {
    std::vector<int> unfilledColumns = game.getUnfilledColumns();
    int &col = randomElement(unfilledColumns);

    return col;
  }

  const int &n = game.connectionLength();
  int turn = getTurn(game), oppTurn = game.opposingTurn(turn);

  Orientation selfOptimal = game.optimalOrientation(turn);
  Orientation oppOptimal = game.optimalOrientation(oppTurn);

  int pos, selfReq = game.requiredMoves(turn, selfOptimal),
           oppReq = game.requiredMoves(oppTurn, oppOptimal);

  // std::cout << selfOptimal << " | " << oppOptimal << "\n";
  // std::cout << selfReq << " | " << oppReq << "\n";

  // If the required moves of the bot's and opponent's optimal orientation
  // is the same as the grid size (no optimal orientation), pick from one of
  // the best initial places
  if (oppReq == n && selfReq == n) {
    int center = std::ceil((gridSize[1]) / 2);

    // If the center is vacant, place the mark
    int bCenter[2]{gridSize[0] - 1, center};
    if (!game.hasMark(bCenter))
      return center;
  }

  // If the required moves of the bot itself is less than the required moves of
  // the player, proceed to playing offensively
  else if (selfReq <= oppReq) {
    for (const Cell &cell : selfOptimal) {
      if (cell.state == (CellState)(turn))
        continue;

      return cell.position[1];
    }
  }

  // Otherwise, play defensively to keep the player from winning
  else {
    for (const Cell &cell : oppOptimal) {
      if (cell.state == (CellState)(oppTurn))
        continue;

      return cell.position[1];
    }
  }

  return pos;
}

void Bot::play(ConnectN &game) {
  int col = decideColumn(game) + 1;

  std::this_thread::sleep_for(std::chrono::milliseconds(placeDelay));

  game.placeMark(getTurn(game), col);
  game.mapGrid();
  game.process();
}
