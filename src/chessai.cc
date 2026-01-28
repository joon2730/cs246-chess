#include "chessai.h"
#include "board.h"
#include "move.h"
#include "piece.h"
#include <iostream>
#include <stdexcept>

void ChessAI::checkTime() const {
    if (time_limited && (std::chrono::steady_clock::now() >= deadline)) {
        throw TimeUp{};
    }
}

void ChessAI::setTimeLimit(std::chrono::steady_clock::time_point dl) {
    time_limited = true;
    deadline = dl;
}

void ChessAI::clearTimeLimit() { time_limited = false; }

vector<Move> ChessAI::searchMoves(Board& board, int side, int depth) {
    checkTime();
    auto legal_moves = board.listLegalMoves(side);
    // std::cout << "len legal_mvs: " << legal_moves.size() << "\n";
    vector<Move> best_moves;
    if (side == MAXIMIZING_PLAYER) {
        int max_score = INT_MIN;
        for (auto mv: legal_moves) {
            checkTime();
            board.doMove(mv);
            int score = 0;
            try {
                score = alphabetaMin(board, INT_MIN, INT_MAX, depth, 1);
            } catch (const TimeUp&) {
                board.undoMove(mv);
                throw;
            }
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
            checkTime();
            board.doMove(mv);
            int score = 0;
            try {
                score = alphabetaMax(board, INT_MIN, INT_MAX, depth, 1);
            } catch (const TimeUp&) {
                board.undoMove(mv);
                throw;
            }
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

int ChessAI::alphabetaMax(Board& board, int alpha, int beta, int depthleft, int ply_from_root) {
    checkTime();
    if (depthleft == 0) {
        return evaluateBoard(board);
    }
    auto legal_moves = board.listLegalMoves(MAXIMIZING_PLAYER);
    if (legal_moves.size() == 0) {
        if (board.detectChecked(MAXIMIZING_PLAYER)) {
            // Max side is checkmated: sooner mate is worse for Max
            return -MATE_SCORE + ply_from_root;
        } else {
            return 0;
        }
    }
    int max_score = INT_MIN;
    for (auto mv: legal_moves) {
        checkTime();
        board.doMove(mv);
        int score = 0;
        try {
            score = alphabetaMin(board, alpha, beta, depthleft-1, ply_from_root + 1);
        } catch (const TimeUp&) {
            board.undoMove(mv);
            throw;
        }
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

int ChessAI::alphabetaMin(Board& board, int alpha, int beta, int depthleft, int ply_from_root) {
    checkTime();
    if (depthleft == 0) {
        return evaluateBoard(board);
    }
    auto legal_moves = board.listLegalMoves(MINIMIZING_PLAYER);
    if (legal_moves.size() == 0) {
        if (board.detectChecked(MINIMIZING_PLAYER)) {
            // Min side is checkmated: sooner mate is better for Max
            return MATE_SCORE - ply_from_root;
        } else {
            return 0;
        }
    }
    int min_score = INT_MAX;
    for (auto mv: legal_moves) {
        checkTime();
        board.doMove(mv);
        int score = 0;
        try {
            score = alphabetaMax(board, alpha, beta, depthleft-1, ply_from_root + 1);
        } catch (const TimeUp&) {
            board.undoMove(mv);
            throw;
        }
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

