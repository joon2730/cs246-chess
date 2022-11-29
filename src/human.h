#ifndef HUMAN_H
#define HUMAN_H

#include "player.h"
#include <string>

using std::string;

class Human: public Player {
  Move createMove(Board& board, string p1, string p2);
 public:
  Human(int side);
  Move makeMove(Board &board) override;
};
#endif
