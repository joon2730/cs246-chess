#ifndef KING_H
#define KING_H
#include "piece.h"
#include <iostream>

class King: public Piece {
  public:
    King(int color, int name);
    std::string printText() override;
    bool canMove(Move *mv) override;
};

#endif
