#include "piece.h"

Piece::Piece(int color, int name): color{color}, name{name}, position{nullptr} {}

void Piece::setPosition(Square *sq) {
    position = sq;
}

Square* Piece::getPosition() {
    return position;
}

bool Piece::isDead() {
    return position == nullptr;
}

bool Piece::getHasMoved() {
    return has_moved;
}

void Piece::setHasMoved(bool has_moved) {
    this->has_moved = has_moved;
}

int Piece::getColor() {
    return color;
}

int Piece::getName() {
    return name;
}


