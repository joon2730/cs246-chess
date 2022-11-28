#ifndef KING_H
#define KING_H
#include "piece.h"

class King: public Piece {
  public:
    King(int color, int name);
    std::string printText() override;
    bool canMove(Board& board, shared_ptr<Move>& mv) override;
    std::vector<std::shared_ptr<Move>> listPossibleMoves(Board& board) override;

};

#endif
