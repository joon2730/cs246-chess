#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include <string>
#include <iostream>

using std::string;

class Computer: public Player {
  protected:
    bool auto_move = false;
  public:
    Computer(int side): Player{side} {}
    virtual ~Computer() = default;
};
#endif
