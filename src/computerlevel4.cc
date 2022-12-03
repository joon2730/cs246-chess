#include "computerlevel4.h"
#include <cstdlib>
#include <ctime>

using std::cout;

ComputerLevel4::ComputerLevel4(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel4::makeMove(Board &board) {
    auto preferred_moves = searchMoves(board, side);
    int len = preferred_moves.size();
    int randint = std::rand();
    return preferred_moves.at(randint % len);
}

int ComputerLevel4::evaluateBoard(Board& board) {
    int value = 0;
    for (int color = 0; color < board.NUM_COLORS; ++color) {
        for (int type = 0; type < board.NUM_PIECE_TYPES; ++type) {
            value = sign(side) * piece_value[type] * board.getNumAlivePieces(color, type);
        }
    }
    return value;
}






