#include "computerlevel2.h"
#include <cstdlib>
#include <ctime>


ComputerLevel2::ComputerLevel2(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel2::makeMove(Board &board) {
    auto legal_moves = board.listLegalMoves(side);
    auto preferred_moves = skimMoves(board, legal_moves);
    int len = preferred_moves.size();
    int randint = std::rand();
    return preferred_moves.at(randint % len);
}

vector<Move> ComputerLevel2::skimMoves(Board& board, vector<Move>& moves) {
    vector<Move> preferred_moves;
    int max_score = 0;
    for (auto mv: moves) {
        int score = evaluateMove(board, mv);
        if (score == max_score) {
            preferred_moves.push_back(mv);
        } else if (score > max_score) {
            preferred_moves.clear();
            preferred_moves.push_back(mv);
            max_score = score;
        }
    }
    return preferred_moves;
}

int ComputerLevel2::evaluateMove (Board& board, Move& mv) {
    int score = 0;
    if (mv.is_attack) {
        ++score;
    }
    if (board.isChecking(mv, board.opponent(side))) {
        ++score;
    }
    return score;
}




