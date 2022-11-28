#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <utility>

using std::string;
using std::vector;
using std::shared_ptr;
class Square;
class Piece;


class Move {
    Square *start;
    Square *end;
    shared_ptr<Piece> moving_piece;
    shared_ptr<Piece> killed_piece;
    bool attack = false;
    bool enpassant = false;
    bool promotion = false;
    bool castling = false;
    bool resign = false;
  public:
    Move(Square *start, Square *end);
    void setMovingPiece(shared_ptr<Piece> moving_piece);
    void setKilledPiece(shared_ptr<Piece> killed_piece);
    void setIsAttack();
    void setIsEnpassant();
    void setIsPromotion();
    void setIsCastling();
    void setIsResign();
    Square* getStartSquare();
    Square* getEndSquare();
    shared_ptr<Piece> getMovingPiece();
    shared_ptr<Piece> getKilledPiece();
    bool isAttack();
    bool isEnpassant();
    bool isPromotion();
    bool isCastling();
    bool isResign();
};
#endif
