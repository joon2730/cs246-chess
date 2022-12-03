#ifndef COMPUTERLEVEL3_H
#define COMPUTERLEVEL3_H

#include "computer.h"
#include <string>

using std::string;

class ComputerLevel3: public Computer {
  public:
    ComputerLevel3(int side);
    Move makeMove(Board &board) override;
    vector<Move> skimMoves(Board& board, vector<Move>& moves);
    int evaluateMove (Board& board, Move& mv);
};
#endif


