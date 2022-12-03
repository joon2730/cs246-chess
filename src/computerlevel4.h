#ifndef COMPUTERLEVEL4_H
#define COMPUTERLEVEL4_H

#include "computer.h"
#include "chessai.h"
#include <string>

using std::string;

class ComputerLevel4: public ChessAI, public Computer {
  public:
    static const int NUM_PIECE_TYPES = 6;
    const int piece_value[NUM_PIECE_TYPES] = {10, 30, 35, 50, 90, 900};
    ComputerLevel4(int side);
    Move makeMove(Board &board) override;
    int evaluateBoard(Board& board) override;
};
#endif


