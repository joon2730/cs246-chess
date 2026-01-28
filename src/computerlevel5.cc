#include "computerlevel5.h"

#include "board.h"
#include "move.h"
#include "piece.h"
#include "square.h"

#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

ComputerLevel5::ComputerLevel5(int side): Computer{side} {
    std::srand(std::time(0));
}

int ComputerLevel5::evaluateBoard(Board& board) {
    int value = 0;
    for (int type = 0; type < board.NUM_PIECE_TYPES; ++type) {
        value += piece_value[type] * board.getNumAlivePieces(MAXIMIZING_PLAYER, type);
        value -= piece_value[type] * board.getNumAlivePieces(MINIMIZING_PLAYER, type);
    }
    for (int color = 0; color < board.NUM_COLORS; ++color) {
        for (auto pc: getPieces(board, color)) {
            if (!pc->isDead()) {
                Square *sq = pc->getPosition();
                int type = pc->getName();
                if (pc->getColor() == board.WHITE) {
                    value += position_values[type][sq->getRow()][sq->getCol()];
                } else {
                    value -= position_values[type][board.ROWS - 1 - sq->getRow()][sq->getCol()];
                }
            }
        }
    }
    return value;
}

static std::string toCoord(Square *sq) {
    char file = static_cast<char>('a' + sq->getCol());
    char rank = static_cast<char>('8' - sq->getRow());
    std::string s;
    s += file;
    s += rank;
    return s;
}

bool ComputerLevel5::tryOpeningBook(Board& board, Move& out) {
    // "First 2 plies" book:
    // - If this is the very first move (white to play), choose one of common openings.
    // - If this is black's first move, respond based on white's first move.
    const int ply = board.getNumMovesPlayed();
    if (ply == 0 && side == board.WHITE) {
        // candidate openings for White
        std::vector<std::pair<std::string, std::string>> cands = {
            {"e2", "e4"},
            {"d2", "d4"},
            {"c2", "c4"},
            {"g1", "f3"},
        };
        auto pick = cands.at(std::rand() % cands.size());
        out = Move(board.getSquare(pick.first), board.getSquare(pick.second));
        if (!board.isLegal(out)) return false;
        return true;
    }

    if (ply == 1 && side == board.BLACK) {
        Move last = board.getLastMove();
        std::string w_from = toCoord(last.start);
        std::string w_to = toCoord(last.end);

        std::vector<std::pair<std::string, std::string>> replies;
        if (w_from == "e2" && w_to == "e4") {
            // vs 1.e4: e5 / c5 / c6
            replies = {{"e7", "e5"}, {"c7", "c5"}, {"c7", "c6"}};
        } else if (w_from == "d2" && w_to == "d4") {
            // vs 1.d4: d5 / Nf6
            replies = {{"d7", "d5"}, {"g8", "f6"}};
        } else if (w_from == "c2" && w_to == "c4") {
            // vs 1.c4: e5 / c5
            replies = {{"e7", "e5"}, {"c7", "c5"}};
        } else if (w_from == "g1" && w_to == "f3") {
            // vs 1.Nf3: d5 / Nf6
            replies = {{"d7", "d5"}, {"g8", "f6"}};
        } else {
            return false;
        }

        // Try a few random picks until a legal one is found.
        for (int attempt = 0; attempt < 6 && !replies.empty(); ++attempt) {
            auto pick = replies.at(std::rand() % replies.size());
            out = Move(board.getSquare(pick.first), board.getSquare(pick.second));
            if (board.isLegal(out)) return true;
        }
    }

    return false;
}

Move ComputerLevel5::makeMove(Board &board) {
    std::string input;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }

    // Opening book (only first 2 plies)
    Move book_mv = Move(board.getSquare(0,0), board.getSquare(0,0));
    if (tryOpeningBook(board, book_mv)) {
        // auto-mode logging (consistent with other levels)
        if (auto_move) {
            std::cout << "computer moved: " << toCoord(book_mv.start) << " " << toCoord(book_mv.end) << std::endl;
        }
        return book_mv;
    }

    // Time-limited iterative deepening
    using clock = std::chrono::steady_clock;
    const auto deadline = clock::now() + std::chrono::milliseconds(time_budget_ms);

    std::vector<Move> best_moves;
    int depth = 1;
    setTimeLimit(deadline);
    while (true) {
        // If we're already out of time, stop.
        if (clock::now() >= deadline) break;
        try {
            auto cand = searchMoves(board, side, depth);
            if (!cand.empty()) {
                best_moves = std::move(cand);
            }
            ++depth;
        } catch (const TimeUp&) {
            break; // keep last completed best_moves
        }
    }
    clearTimeLimit();

    // Fallback: if nothing completed (edge case), do a shallow search without time limit.
    if (best_moves.empty()) {
        best_moves = searchMoves(board, side, 1);
    }

    int idx = std::rand() % best_moves.size();
    Move mv = best_moves.at(idx);

    if (auto_move) {
        if (mv.is_kingside_castling) {
            std::cout << "computer moved: O-O" << std::endl;
        } else if (mv.is_queenside_castling) {
            std::cout << "computer moved: O-O-O" << std::endl;
        } else {
            std::cout << "computer moved: " << toCoord(mv.start) << " " << toCoord(mv.end) << std::endl;
        }
    }

    return mv;
}

