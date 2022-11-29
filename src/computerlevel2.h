#ifndef COMPUTERLEVEL2_H
#define COMPUTERLEVEL2_H

#include "computer.h"
#include <string>

using std::string;

class ComputerLevel2: public Computer {
  public:
    ComputerLevel2(int side);
    Move makeMove(Board &board) override;
};
#endif


