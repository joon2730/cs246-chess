#include "chessai.h"
#include "board.h"
#include "move.h"
#include "piece.h"
#include <iostream>
#include <stdexcept>

vector<Move> ChessAI::searchMoves(Board& board, int side, int depth) {
    auto legal_moves = board.listLegalMoves(side);
    // std::cout << "len legal_mvs: " << legal_moves.size() << "\n";
    vector<Move> best_moves;
    if (side == MAXIMIZING_PLAYER) {
        int max_score = INT_MIN;
        for (auto mv: legal_moves) {
            board.doMove(mv);
            int score = alphabetaMin(board, INT_MIN, INT_MAX, depth);
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
            int score = alphabetaMax(board, INT_MIN, INT_MAX, depth);
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
    board.updateState();
    return best_moves;
}

int ChessAI::alphabetaMax(Board& board, int alpha, int beta, int depthleft) {
    if (depthleft == 0) {
        return evaluateBoard(board);
    }
    auto legal_moves = board.listLegalMoves(MAXIMIZING_PLAYER);
    if (legal_moves.size() == 0) {
        if (board.detectChecked(MAXIMIZING_PLAYER)) {
            return -99999;
        } else {
            return 0;
        }
    }
    int max_score = INT_MIN;
    for (auto mv: legal_moves) {
        board.doMove(mv);
        int score = alphabetaMin(board, alpha, beta, depthleft-1);
        board.undoMove(mv);
        if (score > max_score) {
            max_score = score;
        }
        if (alpha < max_score) {
            alpha = max_score;
        }
        if (beta < alpha) {
            return beta;
        }
    }
    return max_score;
}

int ChessAI::alphabetaMin(Board& board, int alpha, int beta, int depthleft) {
    if (depthleft == 0) {
        return evaluateBoard(board);
    }
    auto legal_moves = board.listLegalMoves(MINIMIZING_PLAYER);
    if (legal_moves.size() == 0) {
        if (board.detectChecked(MINIMIZING_PLAYER)) {
            return 99999;
        } else {
            return 0;
        }
    }
    int min_score = INT_MAX;
    for (auto mv: legal_moves) {
        board.doMove(mv);
        int score = alphabetaMax(board, alpha, beta, depthleft-1);
        board.undoMove(mv);
        if (score < min_score) {
            min_score = score;
        }
        if (beta > min_score) {
            beta = min_score;
        }
        if (beta < alpha) {
            return alpha;
        }
    }
    return min_score;
}


vector<std::shared_ptr<Piece>>& ChessAI::getPieces(Board& board, int color) {
    return board.pieces[color];
}
