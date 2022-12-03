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
    std::shared_ptr<Piece> getPiece();
    int getRow();
    int getCol();
};

std::ostream& operator<<(std::ostream& out, Square& sq);

#endif
