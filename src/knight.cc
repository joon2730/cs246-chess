#include "knight.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Knight::Knight(int color, int name) : Piece(color, name) {}

bool Knight::canMove(Board& board, Move& mv) {
  // cannot move to square with a piece with same color
  if ((!mv.end->isEmpty()) && (color == mv.end->getPiece()->getColor())) {
    return false;
  }

  int start_col = mv.start->getCol();
  int start_row = mv.start->getRow();

  int end_col = mv.end->getCol();
  int end_row = mv.end->getRow();

  int hoz_move = dist(mv.start->getCol(), mv.end->getCol());
  int ver_move = dist(mv.start->getRow(), mv.end->getRow());

  if (!(hoz_move * ver_move == 2)) {
    return false;
  }

  return true;
}
//
vector<Move> Knight::listPossibleMoves(Board& board) {
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

std::string Knight::printText() {
  if (color == WHITE) {
    return "N";
  } else {
    return "n";
  }
}
