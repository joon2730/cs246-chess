#ifndef COMPUTER_H
#define COMPUTER_H

#include "player.h"
#include <string>

using std::string;

class Computer: public Player {
  public:
    Computer(int side): Player{side} {}
    virtual ~Computer() = default;
};
#endif
