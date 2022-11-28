#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"
#include <string>

using std::string;

class Human: public Player {
    shared_ptr<Move> createMove(Board& board, string p1, string p2);
  public:
    Human(int side);
    shared_ptr<Move> makeMove(Board &board) override;
};
#endif
