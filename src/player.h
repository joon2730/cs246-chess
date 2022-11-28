#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "move.h"

class Player {
    int side;
  public:
    Player();
    virtual Move *makeMove(Board &board);
};
#endif
