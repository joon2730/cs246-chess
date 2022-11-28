#include "pawn.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Pawn::Pawn(int color, int name) : Piece(color, name) {}

bool Pawn::canMove(Board *board, Move *mv) {
  Square *start = mv->getStartSquare();
  Square *end = mv->getEndSquare();
  // can't move to square with a piece with same color
  if ((!end->isEmpty()) && (color == end->getPiece()->getColor())) {
    return false;
  }
  // can only move 1 or 2 squares.
  int hoz_move = disp(start->getCol(), end->getCol());
  int ver_move = disp(start->getRow(), end->getRow());
  if (pawn->getMoved()) {
    if (!(hoz_move <= 1 && hoz_move >= 0 && ver_move <= 1 && ver_move >= -1)) {
      return false;
    }
  } else {
    if (!(hoz_move == 1 && (ver_move == 0 || ver_move == 1) ||
        !(hoz_move == 2 && ver_move == 0)) {
      return false;
    }
  }

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

std::string Pawn::printText() {
  if (color == WHITE) {
    return "P";
  } else {
    return "p";
  }
}

std::vector<Move *> Pawn::listPossibleMoves(Board *board) {
  std::vector<Move *> possible_moves;
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
          possible_moves.push_back(
              new Move(position, board->getSquare(cur_row, cur_col)));
        }
      }
    }
  }
  return possible_moves;
}

bool Pawn::getMoved() { return moved_two_steps; }
