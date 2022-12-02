#include "rook.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Rook::Rook(int color, int name) : Piece(color, name) {}

bool Rook::canMove(Board& board, Move& mv) {
  // cannot move to square with a piece with same color
  if ((!mv.end->isEmpty()) && (color == mv.end->getPiece()->getColor())) {
    return false;
  }
  int start_col = mv.start->getCol();
  int start_row = mv.start->getRow();
  int end_col = mv.end->getCol();
  int end_row = mv.end->getRow();
  // can only move horizontally or vertically in a straight line
  if (!(isStraight(start_col, start_row, end_col, end_row))) {
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
  mv.is_pseudo_legal = true;
  return true;
}

vector<Move> Rook::listPseudoLegalMoves(Board& board) {
  vector<Move> pseudo_legal_moves;
  int row = position->getRow();
  int col = position->getCol();
  for (int i = 0; i < board.getRows(); ++i) {
    Move mv = Move(position, board.getSquare(i, col));
    if (canMove(board, mv)) {
      pseudo_legal_moves.push_back(std::move(mv));
    }
  }
  for (int j = 0; j < board.getCols(); ++j) {
    Move mv = Move(position, board.getSquare(row, j));
    if (canMove(board, mv)) {
      pseudo_legal_moves.push_back(std::move(mv));
    }
  }
  return pseudo_legal_moves;
}

string Rook::printText() {
  if (color == WHITE) {
    return "R";
  } else {
    return "r";
  }
}
