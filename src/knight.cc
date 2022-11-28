#include "knight.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

Knight::Knight(int color, int name) : Piece(color, name) {}

bool Knight::canMove(Board *board, Move *mv) {
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

  int hoz_move = dist(start->getCol(), end->getCol());
  int ver_move = dist(start->getRow(), end->getRow());

  if (!(hoz_move + ver_move == 3) && (hoz_move == 3 || ver_move == 3)) {
    return false;
  }

  return true;
}
//
std::vector<Move *> Knight::listPossibleMoves(Board *board) {
  // std::cout << printText() << "\n";
  std::vector<Move *> possible_moves;

  return possible_moves;
}

std::string Knight::printText() {
  if (color == WHITE) {
    return "N";
  } else {
    return "n";
  }
}
