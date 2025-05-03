#include <iostream>
#include <string>

#include "../include/ConnectN.h"
#include "../include/Player.h"

Player::Player(std::string name) : name(name) {}

std::string Player::getName() { return name; }
int Player::getTurn(ConnectN &game) { return game.getTurn(this); }

void Player::play(ConnectN &game) {
  // TODO: PLayer play()
  Grid &grid = game.getGrid();
  int col, maxCol = game.getGridSize()[1];

  std::cout << "[" << name << "] Enter the Column: ";
  std::cin >> col;

  if (col < 1 || col > maxCol || std::cin.fail()) {
    std::cout << "Valid columns range from 1 to " << maxCol << "\n";

    return play(game);
  }

  game.placeMark(getTurn(game), col);
  game.mapGrid();
  game.process();
}