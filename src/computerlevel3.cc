#include "computerlevel3.h"
#include <cstdlib>
#include <ctime>


ComputerLevel3::ComputerLevel3(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel3::makeMove(Board &board) {
    std::string input;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }
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




