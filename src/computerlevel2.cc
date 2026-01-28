#include "computerlevel2.h"
#include <cstdlib>
#include <ctime>


ComputerLevel2::ComputerLevel2(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel2::makeMove(Board &board) {
    std::string input;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }
    const bool was_auto = auto_move;
    auto legal_moves = board.listLegalMoves(side);
    auto preferred_moves = skimMoves(board, legal_moves);
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
        std::cout << "computer moved: " << toCoord(mv.start) << " " << toCoord(mv.end) << std::endl;
    }
    return mv;
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


