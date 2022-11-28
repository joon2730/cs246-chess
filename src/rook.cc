#include "rook.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Rook::Rook(int color, int name) : Piece(color, name) {}

bool Rook::canMove(Board& board, shared_ptr<Move>& mv) {
  Square* start = mv->getStartSquare();
  Square* end = mv->getEndSquare();

  // cannot move to square with a piece with same color
  if ((!end->isEmpty()) && (color == end->getPiece()->getColor())) {
    return false;
  }

  int start_col = start->getCol();
  int start_row = start->getRow();
  int end_col = end->getCol();
  int end_row = end->getRow();

  // can only move horizontally or vertically in a straight line
  if (!(isStraight(start_col, end_col, start_row, end_row))) {
    return false;
  }

  // cannot move if any square is blocked in between
  int col_step = sign(end_row - start_row);
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

vector<shared_ptr<Move>> Rook::listPossibleMoves(Board& board) {
  // cout << printText() << "\n";
  vector<shared_ptr<Move>> possible_moves;
  for (int i = 0; i < board.getRows(); ++i) {
    for (int j = 0; j < board.getCols(); ++j) {
      // cout << i << " " << j << "\n";
      shared_ptr<Move> mv =
          std::make_shared<Move>(position, board.getSquare(i, j));
      if (canMove(board, mv)) {
        possible_moves.push_back(std::move(mv));
      }
    }
  }

  return possible_moves;
}

string Rook::printText() {
  if (color == WHITE) {
    return "R";
  } else {
    return "r";
  }
}
