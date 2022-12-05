#include "human.h"
#include "square.h"
#include "move.h"
#include "piece.h"
#include "math.h"
#include <iostream>
#include <stdexcept>

Human::Human(int side): Player{side} {}

Move Human::makeMove(Board &board) {
    string input;
    while (true) {
        try {
            if (std::cin >> input) {
                if (input == "move") {
                    std::string start_pos, end_pos;
                    std::cin >> start_pos >> end_pos;
                    Move mv = createMove(board, start_pos, end_pos);
                    if (mv.start->isEmpty()) {
                        throw std::invalid_argument("No piece to move found");
                    }
                    bool can_move = mv.start->getPiece()->canMove(board, mv);
                    if (!can_move) {
                        throw std::invalid_argument("Move not possible");
                    } else if (mv.is_promotion) {
                        std::string pc;
                        std::cin >> pc;
                        if (pc == "N" || pc == "n") {
                            mv.promote_to = board.KNIGHT;
                        } else if (pc == "B" || pc == "b") {
                            mv.promote_to = board.BISHOP;
                        } else if (pc == "R" || pc == "r") {
                            mv.promote_to = board.ROOK;
                        } else if (pc == "Q" || pc == "q") {
                            mv.promote_to = board.QUEEN;
                        } else {
                            throw std::invalid_argument("Invalid piece type");
                        }
                    }
                    if (!board.isLegal(mv)) {
                        throw std::invalid_argument("Illegal move");
                    } else if (side != mv.moving_piece->getColor()) {
                        if (side == WHITE) {
                            throw std::invalid_argument("White's turn");
                        } else {
                            throw std::invalid_argument("Black's turn");
                        }
                    } else {
                        return mv;
                    }
                } else if (input == "resign") {
                    Move mv = createMove(board, "a1", "a1");
                    if (side == board.WHITE) {
                        mv.is_white_resign = true;
                    } else {
                        mv.is_black_resign = true;
                    }
                    return mv;
                } else if (input == "takeback") {
                    if (board.getNumMovesPlayed() < 2) {
                        throw std::invalid_argument("No moves to takeback found");
                    }
                    std::cout << "Takeback accepted\n";
                    board.pop();
                    board.pop();
                    board.notifyObservers();
                }
            }
        } catch (std::invalid_argument& ex) {
            std::cout << ex.what() << std::endl;
        }
    } 
}

Move Human::createMove(Board& board, string p1, string p2) {
    Square *start = board.getSquare(p1);
    Square *end = board.getSquare(p2);
    // translating castling input
    if (!start->isEmpty() && start->getPiece()->getName() == board.KING) {
        if (dist(start->getCol(), end->getCol()) > 1) {
            int e_file = 'e' - 'a';    
            int g_file = 'g' - 'a';
            int c_file = 'c' - 'a';
            int a_file = 'a' - 'a';    
            int h_file = 'h' - 'a';
            if (start->getCol() == e_file && !start->getPiece()->getHasMoved()) {
                if (start->getRow() == end->getRow()) {
                    if (end->getCol() == g_file) {
                        end = board.getSquare(start->getRow(), h_file);
                    } else if (end->getCol() == c_file) {
                        end = board.getSquare(start->getRow(), a_file);
                    }
                }
            }
        }
    }
    return Move(start, end);
}
