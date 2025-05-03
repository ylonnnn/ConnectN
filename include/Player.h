#pragma once

#include <string>

class ConnectN;

class Player {
protected:
  std::string name;

public:
  Player(std::string name);

  std::string getName();
  int getTurn(ConnectN &game);

  void play(ConnectN &game);
};
