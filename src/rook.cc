#include "rook.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Rook::Rook(int color, int name) : Piece(color, name) {}

bool Rook::canMove(Board *board, Move *mv) {
  Square *start = mv->getStartSquare();
  Square *end = mv->getEndSquare();

  // cannot move to square with a piece with same color
  if ((!end->isEmpty()) && (color == end->getPiece()->getColor())) {
    return false;
  }

  int start_col = start->getCol();
  int start_row = start->getRow();
  int end_col = end->getCol();
  int end_row = end->getRow();

  // can only move horizontally or vertically in a straight line
  if (!(isStraight(start_col, end_col, start_row, end_row)) {
    return false;
  }

  // cannot move if any square is blocked in between
  int col_step = sign(end_col - start_col);
  int row_step = sign(end_row - start_row);
  int cur_col = start_col + col_step;
  int cur_row = start_row + row_step;

  while (!(cur_col == end_col && cur_row == end_row)) {
    if (!board->getSquare(cur_row, cur_col)->isEmpty()) {
      return false;
    }
    cur_col += col_step;
    cur_row += row_step;
  }
  return true;
}

std::vector<Move *> Queen::listPossibleMoves(Board *board) {
  // std::cout << printText() << "\n";
  std::vector<Move *> possible_moves;
  for (int i = 0; i < board->getRows(); ++i) {
    for (int j = 0; j < board->getCols(); ++j) {
      // std::cout << i << " " << j << "\n";
      Move *mv = new Move(position, board->getSquare(i, j));
      if (canMove(board, mv)) {
        possible_moves.push_back(mv);
      } else {
        delete mv;
      }
    }
  }

  return possible_moves;
}

std::string Queen::printText() {
  if (color == WHITE) {
    return "R";
  } else {
    return "r";
  }
}
