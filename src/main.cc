#include "chessgame.h"
#include "player.h"
#include "human.h"
#include "computerlevel1.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

using std::string;
using std::vector;
using std::unique_ptr;

// shared_ptr<Move> createMove(Board& board, string p1, string p2) {
//     Square *start = board.getSquare(p1);
//     Square *end = board.getSquare(p2);
//     return std::make_shared<Move>(start, end);
// }

// void playMove(Board& board, string p1, string p2) {
//     try {
//         shared_ptr<Move> mv = createMove(board, p1, p2);
//         board.push(mv);
//         board.render();
//     } catch (std::invalid_argument ex) {
//         std::cout << ex.what() << std::endl;
//     }
// }

int main() {
    enum {WHITE=0, BLACK};
    while (true) {
        // before game begins
        ChessGame game = ChessGame();
        unique_ptr<Player> white = std::make_unique<Human>(WHITE);
        unique_ptr<Player> black = std::make_unique<Human>(BLACK);
        // unique_ptr<Player> black = std::make_unique<ComputerLevel1>(BLACK);
        game.addDisplay(game.TEXT_DISPLAY);
        game.begin(white, black);
        // in game
        while (game.getState() == game.ACTIVE) {
            game.takeTurn();
        }
        // after game ends
    }
}
 
