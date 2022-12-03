#ifndef CHESSGAME_H
#define CHESSGAME_H
#include "board.h"
#include "player.h"
#include "observer.h"
#include <utility>
#include <stdexcept>

using std::shared_ptr;
using std::unique_ptr;
using std::vector;

class ChessGame {
    static const int NUM_PLAYERS = 2;
    unique_ptr<Board> board;
    unique_ptr<Player> players[NUM_PLAYERS];
    vector<unique_ptr<Observer>> observers;
    int state;
    int player_in_turn;
  public:
    enum {TEXT_DISPLAY=0, GRAPHIC_DISPLAY};
    enum {ACTIVE=0, WHITE_WIN, BLACK_WIN, STALEMATE, INSUFFICIENT_MATERIAL};
    ChessGame();
    void setup();
    void begin(unique_ptr<Player>& white, unique_ptr<Player>& blac);
    void takeTurn();
    void resign(int player);
    void addDisplay(int display);
    void updateState();
    int getState();
};
#endif
