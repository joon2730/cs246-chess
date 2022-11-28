#ifndef BOARD_H
#define BOARD_H
#include "square.h"
#include "move.h"
#include "subject.h"
#include <vector>
#include <string>
#include <utility>

using std::string;
using std::vector;
using std::shared_ptr;

class Board: public Subject {
  public:
    static const int ROWS = 8;
    static const int COLS = 8;
    static const int NUM_COLORS = 2;
    enum {WHITE=0, BLACK};
    enum {PAWN=0, KNIGHT, BISHOP, ROOK, QUEEN, KING};
  private:
    vector<vector<Square>> board;
    vector<Piece*> pieces[NUM_COLORS];
    vector<shared_ptr<Move>> moves_played;
    Piece *kings[NUM_COLORS];
    bool checked[NUM_COLORS];
    bool checkmated[NUM_COLORS];
    bool stalemated[NUM_COLORS];
    // Empties pieces and moves_played
    void empty();
    bool checkChecked(int color);
    void updateState();
    bool detectChecked(int Color);
    void addPiece(int piece, int color, string pos);
    bool isLegal(shared_ptr<Move>& mv);
    vector<shared_ptr<Move>> listLegalMoves(int color);
    void removePiece(string coord);
    void movePiece(Piece *piece, Square *from, Square *to);
    void doMove(shared_ptr<Move>& mv);
    void undoMove(shared_ptr<Move>& mv);
    int opponent(int color);
  public:
    Board();
    ~Board();
    void init();
    void render();
    bool isChecked(int color);
    bool isCheckmated(int color);
    bool isStalemated(int color);
    void push(shared_ptr<Move>& mv);
    void pop();
    // Returns a pointer to Square in board at row and col corresponding to coord; ex: "d4", "e4" 
    Square* getSquare(string pos);
    Square* getSquare(int row, int col);
    int getRows();
    int getCols();
};
#endif
