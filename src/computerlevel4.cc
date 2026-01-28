#include "computerlevel4.h"
#include "piece.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using std::cout;

ComputerLevel4::ComputerLevel4(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel4::makeMove(Board &board) {
    std::string input;
    bool study = false;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "study") {
            study = true;
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }
    if (study) {
        std::cout << "computer is thinking..." << std::endl;
        std::cout << "current score: " << evaluateBoard(board) << std::endl;
    }
    const bool was_auto = auto_move;
    auto preferred_moves = searchMoves(board, side, depth);
    int len = preferred_moves.size();
    int randint = std::rand();
    Move mv = preferred_moves.at(randint % len);
    if (was_auto) {
        auto toCoord = [](Square *sq) {
            char file = static_cast<char>('a' + sq->getCol());
            char rank = static_cast<char>('8' - sq->getRow());
            std::string s;
            s += file;
            s += rank;
            return s;
        };
        if (mv.is_kingside_castling) {
            std::cout << "computer moved: O-O" << std::endl;
        } else if (mv.is_queenside_castling) {
            std::cout << "computer moved: O-O-O" << std::endl;
        } else {
            std::cout << "computer moved: " << toCoord(mv.start) << " " << toCoord(mv.end) << std::endl;
        }
    }
    return mv;
}

int ComputerLevel4::evaluateBoard(Board& board) {
    int value = 0;
    for (int type = 0; type < board.NUM_PIECE_TYPES; ++type) {
        value += piece_value[type] * board.getNumAlivePieces(MAXIMIZING_PLAYER, type);
        value -= piece_value[type] * board.getNumAlivePieces(MINIMIZING_PLAYER, type);
    }
    for (int color = 0; color < board.NUM_COLORS; ++color) {
        for (auto pc: getPieces(board, color)) {
            if (!pc->isDead()) {
                Square *sq = pc->getPosition();
                int type = pc->getName();
                if (pc->getColor() == board.WHITE) {
                    value += position_values[type][sq->getRow()][sq->getCol()];
                } else {
                    // Mirror vertically so tables (white perspective) apply to black.
                    value -= position_values[type][board.ROWS - 1 - sq->getRow()][sq->getCol()];
                }
            }
        }
    }
    return value;
}



