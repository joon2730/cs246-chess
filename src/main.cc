#include "chessgame.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

int readPlayerType (const std::string& pt) {
    if (pt == "human") {
        return 0; // == game->HUMAN
    } else if (pt.substr(0, pt.size() - 1) == "computer") {
        int level = pt.back() - '0';
        return level; // == game->COMPUTERLEVEL$(level)
    } else {
        throw std::invalid_argument("Invalid player type");
    }
}

int main() {
    enum {WHITE=0, BLACK};
    // before game begins
    std::unique_ptr<ChessGame> game = std::make_unique<ChessGame>();
    game->addDisplay(game->TEXT_DISPLAY);
    game->addDisplay(game->GRAPHIC_DISPLAY);
    int white_score = 0;
    int black_score = 0;
    bool escape = false;
    std::string input;
    while (true) {
        // before game starts
        while (game->getState() != game->ACTIVE) {
            try {
                if (!(std::cin >> input)) {
                    escape = true;
                    break;
                } else if (input == "game") {
                    std::string p1, p2;
                    std::cin >> p1 >> p2;
                    int white_player_type = readPlayerType(p1);
                    int black_player_type = readPlayerType(p2);
                    game->reset(white_player_type, black_player_type);
                } else if (input == "setup") {
                    game->setup();
                }
            } catch (std::invalid_argument& ex) {
                std::cout << ex.what() << std::endl;
            }
        }
        if (escape) {
            break;
        }
        // in game
        while (game->getState() == game->ACTIVE) {
            game->takeTurn();
        }
        // after game ends
        if (game->getState() == game->WHITE_WIN) {
            white_score += 10;
        } else if (game->getState() == game->BLACK_WIN) {
            black_score += 10;
        } else if (game->getState() == game->STALEMATE) {
            white_score += 5;
            black_score += 5;
        } else if (game->getState() == game->INSUFFICIENT_MATERIAL) {
            white_score += 5;
            black_score += 5;
        } else if (game->getState() == game->THREEFOLD_REPETITION) {
            white_score += 5;
            black_score += 5;
        }

    }
    // when eof was read
    std::cout << "Final Score:\n";
    if (white_score % 10 == 0) {
        std::cout << "White: " << white_score / 10 << std::endl;
    } else {
        std::cout << "White: " << white_score / 10 << "." << white_score % 10 << std::endl;
    }
    if (black_score % 10 == 0) {
        std::cout << "Black: " << black_score / 10 << std::endl;
    } else {
        std::cout << "Black: " << black_score / 10 << "." << black_score % 10 << std::endl;
    }
}
 

