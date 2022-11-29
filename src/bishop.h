#ifndef BISHOP_H
#define BISHOP_H
#include "piece.h"

class Bishop : public Piece {
 public:
  Bishop(int color, int name);
  string printText() override;
  bool canMove(Board& board, Move& mv) override;
  vector<Move> listPossibleMoves(Board& board) override;
};

#endif
