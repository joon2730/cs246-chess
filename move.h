#ifndef MOVE_H
#define MOVE_H

class Square;
class Piece;

class Move {
    Square *start;
    Square *end;
    Piece *moving_piece;
    Piece *killed_piece;
    bool attack = false;
    bool enpassant = false;
    bool promotion = false;
    bool castling = false;
    bool resign = false;
  public:
    Move(Square *start, Square *end);
    void setMovingPiece(Piece *moving_piece);
    void setKilledPiece(Piece *killed_piece);
    void setIsAttack();
    void setIsEnpassant();
    void setIsPromotion();
    void setIsCastling();
    void setIsResign();
    Square* getStartSquare();
    Square* getEndSquare();
    Piece* getMovingPiece();
    Piece* getKilledPiece();
    bool isAttack();
    bool isEnpassant();
    bool isPromotion();
    bool isCastling();
    bool isResign();
};
#endif
