#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"

class Queen: public Piece {
  public:
    Queen(int color, int name);
    std::string printText() override;
    bool canMove(Board *board, Move *mv) override;
    std::vector<Move*> listPossibleMoves(Board *board) override;
};

#endif
