#ifndef QUEEN_H
#define QUEEN_H
#include "piece.h"

class Queen: public Piece {
  public:
    Queen(int color, int name);
    string printText() override;
    bool canMove(Board& board, shared_ptr<Move>& mv) override;
    vector<shared_ptr<Move>> listPossibleMoves(Board& board) override;
};

#endif
