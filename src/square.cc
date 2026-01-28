#include "square.h"

#include "piece.h"

Square::Square(int row, int col) : ROW{row}, COL{col}, piece{nullptr} {}

void Square::place(shared_ptr<Piece>& pc) {
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
bool Square::isEmpty() { return piece == nullptr; }
bool Square::isEmpty() const { return piece == nullptr; }

shared_ptr<Piece> Square::getPiece() { 
  if (piece == nullptr) {
    throw std::logic_error("getPiece: Square is empty");
  }
  return piece; 
}

shared_ptr<Piece> Square::getPiece() const {
  if (piece == nullptr) {
    throw std::logic_error("getPiece: Square is empty");
  }
  return piece;
}

int Square::getRow() { return ROW; }
int Square::getCol() { return COL; }
int Square::getRow() const { return ROW; }
int Square::getCol() const { return COL; }

std::ostream& operator<<(std::ostream& out, Square& sq) {
  if (sq.isEmpty()) {
    if ((sq.getRow() + sq.getCol()) % 2 == 0) {
      out << " ";
    } else {
      out << "_";
    }

  } else {
    out << sq.getPiece()->printText();
  }
  return out;
}

