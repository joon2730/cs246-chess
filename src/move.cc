#include "move.h"

Move::Move(Square *start, Square *end): start{start}, end{end} {}

// Move::make(Piece *moving_piece, Piece *killed_piece = nullptr) {
//     this->killed_piece = moving_piece;
//     this->killed_piece = killed_piece;
// }
void Move::setMovingPiece(Piece *pc) { moving_piece = pc; }
void Move::setKilledPiece(Piece *pc) { killed_piece = pc; }
void Move::setIsAttack() { attack = true; }
void Move::setIsEnpassant() { enpassant = true; }
void Move::setIsPromotion() { promotion = true; }
void Move::setIsCastling() { castling = true; }
void Move::setIsResign() { resign = true; }
Square* Move::getStartSquare() { return start; }
Square* Move::getEndSquare() { return end; }
Piece* Move::getMovingPiece() { return moving_piece; }
Piece* Move::getKilledPiece() { return killed_piece; }
bool Move::isAttack() { return attack; }
bool Move::isEnpassant() { return enpassant; }
bool Move::isPromotion() { return promotion; }
bool Move::isCastling() { return castling; }
bool Move::isResign() { return resign; }
