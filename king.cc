#include "king.h"

King::King(int color, int name): Piece(color, name) {}

bool King::canMove(Move *mv) {
    return true;
}

std::string King::printText() {
    if (color == WHITE) {
        return "K";
    } else {
        return "k";
    }
}

