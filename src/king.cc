#include "king.h"
#include "move.h"
#include "square.h"
#include "math.h"
#include "board.h"

King::King(int color, int name): Piece(color, name) {}

bool King::canMove(Board *board, Move *mv) {
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    // can't move to square with a piece with same color 
    if ((!end->isEmpty()) && (color == end->getPiece()->getColor())) {
        return false;
    }
    // can only move to neighbouring squares
    int hoz_move = dist(start->getCol(), end->getCol());
    int ver_move = dist(start->getRow(), end->getRow());
    if (!(hoz_move <= 1 &&  ver_move <= 1)) {
        return false;
    }
    return true;
}

std::string King::printText() {
    if (color == WHITE) {
        return "K";
    } else {
        return "k";
    }
}

std::vector<Move*> King::listPossibleMoves(Board *board) {
    std::vector<Move*> possible_moves;
    int col = position->getCol();
    int row = position->getRow();
    int cols = board->getCols();
    int rows = board->getRows();
    for (int col_step = -1; col_step <= 1; ++col_step) {
        for (int row_step = -1; row_step <= 1; ++row_step) {
            int cur_col = col + col_step;
            int cur_row = row + row_step;
            if (!(cur_row < 0 || rows <= cur_row || cur_col < 0 || cols <= cur_col)) {
                Square *cur_pos = board->getSquare(cur_row, cur_col);
                if (cur_pos->isEmpty() || color != cur_pos->getPiece()->getColor()) {
                    possible_moves.push_back(new Move(position, board->getSquare(cur_row, cur_col)));
                }
            }
        }
    }
    return possible_moves;
} 
