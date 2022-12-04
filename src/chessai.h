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
    int sign(int side);
    vector<Move> searchMoves(Board& board, int side, int depth);
    int alphabetaMax(Board& board, int alpha, int beta, int depthleft);
    int alphabetaMin(Board& board, int alpha, int beta, int depthleft);
    virtual int evaluateBoard(Board& board) = 0;
    virtual ~ChessAI() = default;

};
#endif


