#pragma once

#include <string>

class ConnectN;

class Player {
protected:
  std::string name;
  int score;

public:
  Player(std::string name);

  std::string getName();
  int getScore();
  void setScore(int score);

  int getTurn(ConnectN &game);

  virtual void play(ConnectN &game);
};
