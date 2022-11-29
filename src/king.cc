#include "king.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

King::King(int color, int name) : Piece(color, name) {}

bool King::canMove(Board& board, Move& mv) {
  // can't move to square with a piece with same color
  if ((!mv.end->isEmpty()) && (color == mv.end->getPiece()->getColor())) {
    return false;
  }
  // can only move to neighbouring squares
  int hoz_move = dist(mv.start->getCol(), mv.end->getCol());
  int ver_move = dist(mv.start->getRow(), mv.end->getRow());
  if (!(hoz_move <= 1 && ver_move <= 1)) {
    return false;
  }

  // castling
  // int col = position->getCol();
  // int row = position->getRow();
  // int cols = board.COLS;
  // int rows = board.ROWS;
  // int end_sign = sign(mv.end->getCol() - mv.start->getCol());

  // if (mv.start->getPiece()->hasMoved() == false) {
  //   if ((hoz_move == 0) && (ver_move == 2)) {
  //     for (int row_step = -1; row_step <= 1; ++row_step) {
  //       int cur_row = row + row_step;
  //       if (end_sign > 0) {
  //       } else if (end_sign < 0) {
  //       }
  //     }
  //   }
  // }
  mv.is_pseudo_legal = true;
  return true;
}

vector<Move> King::listPseudoLegalMoves(Board& board) {
  vector<Move> pseudo_legal_moves;
    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getCols(); ++j) {
            Move mv = Move(position, board.getSquare(i, j));
            if (canMove(board, mv)) {
                pseudo_legal_moves.push_back(mv);
            }
        } 
    }
    return pseudo_legal_moves;
  // vector<Move> pseudo_legal_moves;
  // int col = position->getCol();
  // int row = position->getRow();
  // int cols = board.COLS;
  // int rows = board.ROWS;
  // for (int col_step = -1; col_step <= 1; ++col_step) {
  //   for (int row_step = -1; row_step <= 1; ++row_step) {
  //     int cur_col = col + col_step;
  //     int cur_row = row + row_step;
  //     if (!(cur_row < 0 || rows <= cur_row || cur_col < 0 || cols <= cur_col)) {
  //       Square* cur_pos = board.getSquare(cur_row, cur_col);
  //       if (cur_pos->isEmpty() || color != cur_pos->getPiece()->getColor()) {
  //         pseudo_legal_moves.emplace_back(position, board.getSquare(cur_row, cur_col));
  //       }
  //     }
  //   }
  // }
  // return pseudo_legal_moves;
}

string King::printText() {
  if (color == WHITE) {
    return "K";
  } else {
    return "k";
  }
}
