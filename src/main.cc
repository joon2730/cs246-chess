#include "chessgame.h"
#include "player.h"
#include "human.h"
#include "computerlevel1.h"
#include "computerlevel2.h"
#include "computerlevel3.h"
#include "computerlevel4.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

using std::string;
using std::vector;
using std::unique_ptr;

int main() {
    enum {WHITE=0, BLACK};
    // while (true) {
        // before game begins
        unique_ptr<ChessGame> game = std::make_unique<ChessGame>();
        // unique_ptr<Player> white = std::make_unique<Human>(WHITE);
        unique_ptr<Player> white = std::make_unique<ComputerLevel4>(WHITE);
        // unique_ptr<Player> black = std::make_unique<Human>(BLACK);
        unique_ptr<Player> black = std::make_unique<ComputerLevel4>(BLACK);
        game->addDisplay(game->TEXT_DISPLAY);
        game->begin(white, black);
        // in game
        while (game->getState() == game->ACTIVE) {
            game->takeTurn();
        }
        // after game ends
    // }
}
 
