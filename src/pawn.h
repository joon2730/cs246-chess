#ifndef PAWN_H
#define PAWN_H
#include "piece.h"

class Pawn: public Piece {
 private:
  bool moved_two_steps;

 public:
  Pawn(int color, int name);
  std::string printText() override;
  bool canMove(Board& board, shared_ptr<Move>& mv) override;
  std::vector<std::shared_ptr<Move>> listPossibleMoves(Board& board) override;
  bool getMoved();
};

#endif
