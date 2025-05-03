#include <array>
#include <memory>

#include "include/ConnectN.h"
#include "include/Player.h"

int main() {
  //

  std::array<std::unique_ptr<Player>, 2> players = {
      std::make_unique<Player>("P1"), std::make_unique<Player>("P2")};

  ConnectN game({6, 7}, players);

  game.start();

  return 0;
}