#include "human.h"
#include "square.h"
#include "move.h"
#include "piece.h"
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
                        throw std::invalid_argument("No piece to move found\n");
                    } else if (!board.isLegal(mv)) {
                        throw std::invalid_argument("Illegal move\n");
                    } else if (side != mv.moving_piece->getColor()) {
                        if (side == WHITE) {
                            throw std::invalid_argument("White's turn\n");
                        } else {
                            throw std::invalid_argument("Black's turn\n");
                        }
                    } else {
                        return mv;
                    }
                }
            }
        } catch (std::invalid_argument ex) {
            std::cout << ex.what();
        }
    } 
}

Move Human::createMove(Board& board, string p1, string p2) {
    Square *start = board.getSquare(p1);
    Square *end = board.getSquare(p2);
    return Move(start, end);
}
