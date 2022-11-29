#ifndef KNIGHT_H
#define KNIGHT_H
#include "piece.h"

class Knight : public Piece {
 public:
  Knight(int color, int name);
  string printText() override;
  bool canMove(Board& board, Move& mv) override;
  vector<Move> listPossibleMoves(Board& board) override;
};

#endif
