#include "square.h"
#include "piece.h"

Square::Square(int row, int col): ROW{row}, COL{col}, piece{nullptr} {}

void Square::place(Piece *pc) {
    empty();
    piece = pc;
    piece->setPosition(this);
}
void Square::empty() {
    if (!isEmpty()) {
        piece->setPosition(nullptr);
        piece = nullptr;
    }
}
bool Square::isEmpty() {
    return piece == nullptr;
}
Piece* Square::getPiece() {
    return piece;
}

int Square::getRow() {
    return ROW;
}
int Square::getCol() {
    return COL;
}

std::ostream& operator<<(std::ostream& out, Square& sq) {
    if (sq.isEmpty()) {
        out << "_";
    } else {
        out << sq.getPiece()->printText();
    }
    return out;
}

