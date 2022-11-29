#include "computerlevel2.h"
#include <cstdlib>
#include <time.h>


ComputerLevel2::ComputerLevel2(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel2::makeMove(Board &board) {
    auto legal_moves = board.listLegalMoves(side);
    int len = legal_moves.size();
    return legal_moves.at(std::rand() % len);
}
