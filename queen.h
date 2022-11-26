#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"
#include <iostream>

class Queen: public Piece {
  public:
    Queen(int color, int name);
    std::string printText() override;
    bool canMove(Move *mv) override;
};

#endif
