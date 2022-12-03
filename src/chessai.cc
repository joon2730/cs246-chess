#include "chessai.h"
#include "board.h"
#include "move.h"

int ChessAI::sign(int side) {
    if (side == MAXIMIZING_PLAYER) {
        return 1;
    } else {
        return -1;
    }
}

vector<Move> ChessAI::searchMoves(Board& board, int side) {
    auto legal_moves = board.listLegalMoves(side);
    vector<Move> best_moves;
    if (side == MAXIMIZING_PLAYER) {
        int max_score = INT_MIN;
        for (auto mv: legal_moves) {
            board.doMove(mv);
            int score = evaluateBoard(board);
            board.undoMove(mv);
            if (score == max_score) {
                best_moves.push_back(mv);
            } else if (score > max_score) {
                best_moves.clear();
                best_moves.push_back(mv);
                max_score = score;
            }
        }
    } else if (side == MINIMIZING_PLAYER) {
        int min_score = INT_MAX;
        for (auto mv: legal_moves) {
            board.doMove(mv);
            int score = evaluateBoard(board);
            board.undoMove(mv);
            if (score == min_score) {
                best_moves.push_back(mv);
            } else if (score < min_score) {
                best_moves.clear();
                best_moves.push_back(mv);
                min_score = score;
            }
        }
    }
    return best_moves;
}

