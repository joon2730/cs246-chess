#ifndef SQUARE_H
#define SQUARE_H
#include <iostream>
#include <utility>
#include <memory>

class Piece;

class Square {
    const int ROW;
    const int COL;
    std::shared_ptr<Piece> piece;
  public:
    Square(int row, int col);
    // Place pc on square
    void place(std::shared_ptr<Piece>& pc);
    // Empties square
    void empty();
    // Return whether square is empty
    bool isEmpty();
    bool isEmpty() const;
    std::shared_ptr<Piece> getPiece();
    std::shared_ptr<Piece> getPiece() const;
    int getRow();
    int getRow() const;
    int getCol();
    int getCol() const;
};

std::ostream& operator<<(std::ostream& out, Square& sq);

#endif

