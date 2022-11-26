#include "piece.h"

Piece::Piece(int color, int name): color{color}, name{name}, location{nullptr} {}

Piece::~Piece() {
    
}

void Piece::setLocation(Square *sq) {
    location = sq;
}

bool Piece::isDead() {
    return location == nullptr;
}

bool Piece::hasMoved() {
    return has_moved;
}

int Piece::getColor() {
    return color;
}

