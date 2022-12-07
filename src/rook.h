#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

class Rook : public Piece {
 public:
  Rook(int color, int name);
  string printText() override;
  bool canMove(Board& board, Move& mv) override;
  vector<Move> listPseudoLegalMoves(Board& board) override;
};

#endif

