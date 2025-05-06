#pragma once

#include <string>

#include "Player.h"

class Bot : public Player {
protected:
  int placeDelay;
  float accuracy;

public:
  Bot(std::string name);
  Bot(std::string name, int placeDelay);
  Bot(std::string name, int placeDelay, float accuracy);

  int getPlaceDelay() const;
  float getAccuracy() const;

  int decideColumn(ConnectN &game);

  void play(ConnectN &game) override;
};