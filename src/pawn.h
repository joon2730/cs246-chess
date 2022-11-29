#ifndef PAWN_H
#define PAWN_H
#include "piece.h"

class Pawn: public Piece {
 private:
  bool moved_two_steps;

 public:
  Pawn(int color, int name);
  string printText() override;
  bool canMove(Board& board, Move& mv) override;
  vector<Move> listPseudoLegalMoves(Board& board) override;
  bool getMoved();
};

#endif
