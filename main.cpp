#include <array>
#include <iostream>
#include <memory>

#include "include/Bot.h"
#include "include/ConnectN.h"
#include "include/Player.h"
#include "include/types.h"

// If the game count is negative, the game count limit is indefinite
#define GAME_COUNT -1

// Whether the game will automatically restart or press [Enter] before
// proceeding
#define AUTO_GAME_RESTART false

#define CONNECTION_LENGTH 4

// Grid Size
// { row, column }
const GridSize GRID_SIZE{6, 7};

// Modify
// 1 - Player VS Player
// 2 - Player VS Bot
// 3 - Bot VS Bot
#define GAME_MODE 3

#define PLAYER_1_NAME "P-1"

// Bot 1 Settings
#define BOT_1_PLACE_DELAY 0
#define BOT_1_ACCURACY 100.0f

#define PLAYER_2_NAME "P-2"

// Bot 1 Settings
#define BOT_2_PLACE_DELAY 0
#define BOT_2_ACCURACY 100.0f

int main() {
  // Create the players
  std::array<std::unique_ptr<Player>, 2> players = {
      GAME_MODE == 3 ? std::make_unique<Bot>(PLAYER_1_NAME, BOT_1_PLACE_DELAY,
                                             BOT_1_ACCURACY)
                     : std::make_unique<Player>(PLAYER_1_NAME),
      GAME_MODE == 1 ? std::make_unique<Player>(PLAYER_2_NAME)
                     : std::make_unique<Bot>(PLAYER_2_NAME, BOT_2_PLACE_DELAY,
                                             BOT_2_ACCURACY)};

  // Initialize the game
  ConnectN game(CONNECTION_LENGTH, GRID_SIZE, players);

  for (int i = 0; i != GAME_COUNT; i++) {
    std::cout << "\nGAME " << i + 1 << "\n";

    // Start the game
    game.start();

    if (AUTO_GAME_RESTART)
      continue;
    else {
      std::string input;

      std::cout << "Please press [Enter] to continue\n";
      std::getline(std::cin, input);
    }
  }

  return 0;
}