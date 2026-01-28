#include "computerlevel5.h"

#include "board.h"
#include "move.h"
#include "piece.h"
#include "square.h"
#include "openingbook.h"

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
    return OpeningBook::trySelectNextMove(board, side, OpeningBook::level5Lines(), out);
}

Move ComputerLevel5::makeMove(Board &board) {
    std::string input;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "study") {
            board.toggleStudyMode();
            std::cout << "study mode: " << (board.isStudyMode() ? "on" : "off") << std::endl;
        } else if (input == "text") {
            std::string arg;
            std::cin >> arg;
            if (arg == "on") {
                board.setTextDisplayEnabled(true);
            } else if (arg == "off") {
                board.setTextDisplayEnabled(false);
            } else {
                std::cout << "Usage: text on|off" << std::endl;
            }
            board.notifyObservers();
        } else if (input == "auto") {
            auto_move = true;
        }
    }

    if (board.isStudyMode()) {
        std::cout << "computer is thinking..." << std::endl;
        std::cout << "current score: " << evaluateBoard(board) << std::endl;
    }

    int max_completed_depth = 0;

    // Opening book (up to 3 ply)
    Move book_mv = Move(board.getSquare(0,0), board.getSquare(0,0));
    if (tryOpeningBook(board, book_mv)) {
        if (board.isStudyMode()) {
            std::cout << "max depth completed: " << max_completed_depth << std::endl;
        }
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
                max_completed_depth = depth;
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
        max_completed_depth = std::max(max_completed_depth, 1);
    }

    int idx = std::rand() % best_moves.size();
    Move mv = best_moves.at(idx);

    if (board.isStudyMode()) {
        std::cout << "max depth completed: " << max_completed_depth << std::endl;
    }

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

