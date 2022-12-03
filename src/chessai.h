#ifndef CHESSAI_H
#define CHESSAI_H

#include <vector>
#include <climits>

class Board;
struct Move;

using std::vector;

class ChessAI {
  protected:
    enum {WHITE=0, BLACK};
    const int MAXIMIZING_PLAYER = WHITE;
    const int MINIMIZING_PLAYER = BLACK;
    vector<Move> searchMoves(Board& board, int side);
    int sign(int side);
    virtual int evaluateBoard(Board& board) = 0;
    virtual ~ChessAI() = default;

};
#endif


