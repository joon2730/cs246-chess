#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "board.h"
#include "player.h"

class ChessGame {
    static const int NUM_PLAYERS = 2;
    Board board;
    Player players[NUM_PLAYERS];
  public:
    void takeTurn(Player& player);

  
};
#endif
