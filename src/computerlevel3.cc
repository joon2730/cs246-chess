#include "computerlevel3.h"
#include <cstdlib>
#include <ctime>


ComputerLevel3::ComputerLevel3(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel3::makeMove(Board &board) {
    auto legal_moves = board.listLegalMoves(side);
    auto preferred_moves = skimMoves(board, legal_moves);
    int len = preferred_moves.size();
    int randint = std::rand();
    return preferred_moves.at(randint % len);
}

vector<Move> ComputerLevel3::skimMoves(Board& board, vector<Move>& moves) {
    vector<Move> preferred_moves;
    int max_score = 0;
    for (auto mv: moves) {
        int score = evaluateMove(board, mv);
        // std::cout << "score: " << score << "\n";
        if (score == max_score) {
            preferred_moves.push_back(mv);
        // std::cout << "pushed" << "\n";

        } else if (score > max_score) {
            preferred_moves.clear();
            preferred_moves.push_back(mv);
            max_score = score;
        // std::cout << "cleared and pushed" << "\n";

        }
    }
    // for (auto mv: )
        // std::cout << "max_score: " << max_score << " len: " << preferred_moves.size() << "\n";
    return preferred_moves;
}

int ComputerLevel3::evaluateMove (Board& board, Move& mv) {
    int score = 0;
    // prefers capturing move
    if (mv.is_attack) {
        ++score;
    }
    // prefers checks
    if (board.isChecking(mv, board.opponent(side))) {
        ++score;
    }
    // prefers avoding capture
    // preferes moving to a safe square
    if (!board.isDangerousFor(mv.end, mv.moving_piece)) {
        ++score;
    }
    // prefers moving a piece in danger of being captured
    if (board.isDangerousFor(mv.start, mv.moving_piece)) {
        ++score;
    }
    return score;
}




