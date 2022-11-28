#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

class Rook : public Piece {
 public:
  Rook(int color, int name);
  string printText() override;
  bool canMove(Board& board, shared_ptr<Move>& mv) override;
  vector<shared_ptr<Move>> listPossibleMoves(Board& board) override;
};

#endif
