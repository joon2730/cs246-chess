#include "chessai.h"
#include "board.h"
#include "move.h"

int ChessAI::sign(int side) {
    if (side == 0) {
        return 1;
    } else {
        return -1;
    }
}

vector<Move> ChessAI::searchMoves(Board& board, int side) {
    auto legal_moves = board.listLegalMoves(side);
    vector<Move> maximizing_moves;
    int max_score = INT_MIN;
    for (auto mv: legal_moves) {
        board.doMove(mv);
        int score = evaluateBoard(board);
        board.undoMove(mv);
        if (score == max_score) {
            maximizing_moves.push_back(mv);
        } else if (score > max_score) {
            maximizing_moves.clear();
            maximizing_moves.push_back(mv);
            max_score = score;
        }
    }
    return maximizing_moves;
}

