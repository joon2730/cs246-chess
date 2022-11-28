#include "knight.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Knight::Knight(int color, int name) : Piece(color, name) {}

bool Knight::canMove(Board& board, shared_ptr<Move>& mv) {
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

  int hoz_move = dist(start->getCol(), end->getCol());
  int ver_move = dist(start->getRow(), end->getRow());

  if (!(hoz_move * ver_move == 2)) {
    return false;
  }

  return true;
}
//
vector<shared_ptr<Move>> Knight::listPossibleMoves(Board& board) {
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

std::string Knight::printText() {
  if (color == WHITE) {
    return "N";
  } else {
    return "n";
  }
}
