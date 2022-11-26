#include "queen.h"

Queen::Queen(int color, int name): Piece(color, name) {}

bool Queen::canMove(Move *mv) {
    return true;
}

std::string Queen::printText() {
    if (color == WHITE) {
        return "Q";
    } else {
        return "q";
    }
}
