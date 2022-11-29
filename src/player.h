#ifndef PLAYER_H
#define PLAYER_H

#include "board.h"
#include "move.h"
#include <utility>

using std::shared_ptr;

class Player {
  protected:
    int side;
  public:
    enum {WHITE=0, BLACK};
    Player(int side);
    virtual Move makeMove(Board &board) = 0;
    virtual ~Player() = default;
};
#endif
