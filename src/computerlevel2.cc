#include "computerlevel2.h"
#include <cstdlib>
#include <time.h>


ComputerLevel2::ComputerLevel2(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel2::makeMove(Board &board) {
    auto legal_moves = board.listLegalMoves(side);
    auto preferred_moves = skimMoves(board, legal_moves);
    int len = preferred_moves.size();
    return preferred_moves.at(std::rand() % len);
}

vector<Move> ComputerLevel2::skimMoves(Board& board, vector<Move>& moves) {
    vector<Move> preferred_moves;
    for (auto mv: moves) {
        if (mv.is_attack || board.isChecking(mv, board.opponent(side))) {
            preferred_moves.push_back(mv);
        }
    }
    if (preferred_moves.size() == 0) {
        return moves;
    }
    return preferred_moves;
}
