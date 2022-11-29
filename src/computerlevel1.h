#ifndef COMPUTERLEVEL1_H
#define COMPUTERLEVEL1_H

#include "computer.h"
#include <string>

using std::string;

class ComputerLevel1: public Computer {
  public:
    ComputerLevel1(int side);
    shared_ptr<Move> makeMove(Board &board) override;
};
#endif


