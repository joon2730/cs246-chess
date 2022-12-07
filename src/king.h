#ifndef KING_H
#define KING_H
#include "piece.h"

class King: public Piece {
  public:
    King(int color, int name);
    std::string printText() override;
    bool canMove(Board& board, Move& mv) override;
    vector<Move> listPseudoLegalMoves(Board& board) override;

};

#endif

