#ifndef ROOK_H
#define ROOK_H
#include "piece.h"

class Rook : public Piece {
 public:
  Rook(int color, int name);
  std::string printText() override;
  bool canMove(Board *board, Move *mv) override;
  std::vector<Move *> listPossibleMoves(Board *board) override;
};

#endif
