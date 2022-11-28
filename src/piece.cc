#include "piece.h"

Piece::Piece(int color, int name): color{color}, name{name}, position{nullptr} {}

Piece::~Piece() {
    
}

void Piece::setPosition(Square *sq) {
    position = sq;
}

Square* Piece::getPosition() {
    return position;
}

bool Piece::isDead() {
    return position == nullptr;
}

bool Piece::hasMoved() {
    return has_moved;
}

int Piece::getColor() {
    return color;
}

