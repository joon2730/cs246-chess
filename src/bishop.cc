#include "bishop.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Bishop::Bishop(int color, int name) : Piece(color, name) {}

bool Bishop::canMove(Board& board, Move& mv) {
  // cannot move to square with a piece with same color
  if ((!mv.end->isEmpty()) && (color == mv.end->getPiece()->getColor())) {
    return false;
  }
  int start_col = mv.start->getCol();
  int start_row = mv.start->getRow();
  int end_col = mv.end->getCol();
  int end_row = mv.end->getRow();
  // can only move diagonally
  if (!(isDiagonal(start_col, start_row, end_col, end_row))) {
    return false;
  }
  // cannot move if any square is blocked in between
  int col_step = sign(end_col - start_col);
  int row_step = sign(end_row - start_row);
  int cur_col = start_col + col_step;
  int cur_row = start_row + row_step;
  while (!(cur_col == end_col && cur_row == end_row)) {
    if (!board.getSquare(cur_row, cur_col)->isEmpty()) {
      return false;
    }
    cur_col += col_step;
    cur_row += row_step;
  }
  return true;
}

vector<Move> Bishop::listPossibleMoves(Board& board) {
    // cout << printText() << "\n";
    vector<Move> possible_moves;
    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getCols(); ++j) {
            // cout << i << " " << j << "\n";
            Move mv = Move(position, board.getSquare(i, j));
            if (canMove(board, mv)) {
                possible_moves.push_back(mv);
            }
        } 
    }
    return possible_moves;
}

std::string Bishop::printText() {
  if (color == WHITE) {
    return "B";
  } else {
    return "b";
  }
}
