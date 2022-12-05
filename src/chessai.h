#ifndef CHESSAI_H
#define CHESSAI_H

#include <vector>
#include <climits>
#include <memory>
class Board;
class Piece;
struct Move;

using std::vector;

class ChessAI {
    int alphabetaMax(Board& board, int alpha, int beta, int depthleft);
    int alphabetaMin(Board& board, int alpha, int beta, int depthleft);
  protected:
    enum {WHITE=0, BLACK};
    const int MAXIMIZING_PLAYER = WHITE;
    const int MINIMIZING_PLAYER = BLACK;
    vector<Move> searchMoves(Board& board, int side, int depth);
    vector<std::shared_ptr<Piece>>& getPieces(Board& board, int color);
    virtual int evaluateBoard(Board& board) = 0;
    virtual ~ChessAI() = default;

};
#endif


