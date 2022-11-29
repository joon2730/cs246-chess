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
    vector<shared_ptr<Piece>> pieces[NUM_COLORS];
    vector<Move> moves_played;
    shared_ptr<Piece> kings[NUM_COLORS];
    bool checked[NUM_COLORS];
    bool checkmated[NUM_COLORS];
    bool stalemated[NUM_COLORS];
    // Empties pieces and moves_played
    void empty();
    void updateState();
    // checks if the color's king is currently checked
    bool detectChecked(int Color);
    // checks if the move complys the basic chess movement patterns
    bool isPseudoLegal(Move& mv);
    // checks if the move puts own king in check
    bool isPuttingOwnKingInCheck(Move& mv);
    void addPiece(int piece, int color, string pos);
    void removePiece(string coord);
    void movePiece(shared_ptr<Piece>& piece, Square *from, Square *to);
    void doMove(Move& mv);
    void undoMove(Move& mv);
    int opponent(int color);
  public:
    Board();
    ~Board();
    void init();
    void render();
    bool isChecked(int color);
    bool isCheckmated(int color);
    bool isStalemated(int color);
    void push(Move& mv);
    void pop();
    bool isLegal(Move& mv);
    vector<Move> listLegalMoves(int color);
    // Returns a pointer to Square in board at row and col corresponding to coord; ex: "d4", "e4" 
    Square* getSquare(string pos);
    Square* getSquare(int row, int col);
    int getRows();
    int getCols();
};
#endif
