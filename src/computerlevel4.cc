#include "computerlevel4.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

using std::cout;

ComputerLevel4::ComputerLevel4(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel4::makeMove(Board &board) {
    int depth = 3;
    auto preferred_moves = searchMoves(board, side, depth);
    int len = preferred_moves.size();
    int randint = std::rand();
    std::cout << "evaluation: " << evaluateBoard(board) << "\n\n";
    return preferred_moves.at(randint % len);
}

int ComputerLevel4::evaluateBoard(Board& board) {
    int value = 0;
    for (int type = 0; type < board.NUM_PIECE_TYPES; ++type) {
        value += piece_value[type] * board.getNumAlivePieces(MAXIMIZING_PLAYER, type);
        value -= piece_value[type] * board.getNumAlivePieces(MINIMIZING_PLAYER, type);
    }
    return value;
}






