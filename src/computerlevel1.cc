#include "computerlevel1.h"
#include <cstdlib>
#include <time.h>


ComputerLevel1::ComputerLevel1(int side): Computer{side} {
    std::srand(std::time(0));
}

shared_ptr<Move> ComputerLevel1::makeMove(Board &board) {
    auto legal_moves = board.listLegalMoves(side);
    int len = legal_moves.size();
    return legal_moves.at(std::rand() % len);
}
