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
    static const int NUM_PLAYER_TYPES = 5;
    Board board;
    unique_ptr<Player> players[NUM_PLAYERS];
    vector<unique_ptr<Observer>> observers;
    int state;
    int player_in_turn;
    bool setted = false;
  public:
    enum {TEXT_DISPLAY=0, GRAPHIC_DISPLAY};
    enum {INITIAL = 0, ACTIVE, WHITE_WIN, BLACK_WIN, STALEMATE, INSUFFICIENT_MATERIAL};
    enum {HUMAN = 0, COMPUTERLEVEL1, COMPUTERLEVEL2, COMPUTERLEVEL3, COMPUTERLEVEL4};
    ChessGame();
    void setup();
    void reset(int white_player_type, int black_player_type);
    void takeTurn();
    void resign(int player);
    void addDisplay(int display);
    void placePiece(char text, string pos);
    void removePiece(string pos);
    void getSquare(string pos);
    void readPieceFromChar(char c, int& color, int& piece);
    unique_ptr<Player> createPlayer(int side, int player_type);
    int getState();
};
#endif
