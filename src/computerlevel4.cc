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
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }
    int depth = 3;
    auto preferred_moves = searchMoves(board, side, depth);
    int len = preferred_moves.size();
    int randint = std::rand();
    return preferred_moves.at(randint % len);
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
                    value -= position_values[type][sq->getCol()][sq->getCol()];
                }
            }
        }
    }
    return value;
}






