#ifndef BOARD_H
#define BOARD_H
#include "square.h"
#include "move.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class Board {
    static const int ROWS = 8;
    static const int COLS = 8;
    static const int NUM_COLORS = 2;
    vector<Piece*> pieces[NUM_COLORS];
    vector<Move*> moves_played;
    Piece *kings[NUM_COLORS];
    bool checked[NUM_COLORS];
    bool checkmated[NUM_COLORS];
    bool stalemated[NUM_COLORS];
    // Empties pieces and moves_played
    void empty();
    bool checkChecked(int color);
    void updateState();
    bool detectChecked(int Color);
    void addPiece(int piece, int color, string coord);
    bool isLegal(Move *mv);
    void removePiece(string coord);
    void applyMove(Move *mv);
    int opponent(int color);
  public:
    vector<vector<Square> > board;
    enum {WHITE=0, BLACK};
    enum {PAWN=0, KNIGHT, BISHOP, ROOK, QUEEN, KING};
    Board();
    ~Board();
    void init();
    bool isChecked(int color);
    bool isCheckmated(int color);
    bool isStalemated(int color);
    void push(Move *mv);
    void pop();
    // Returns a pointer to Square in board at row and col corresponding to coord; ex: "d4", "e4" 
    Square* getSquare(string coord);
    Square* getSquare(int row, int col);
};
#endif
