#ifndef SQUARE_H
#define SQUARE_H
#include <iostream>

class Piece;

class Square {
    const int ROW;
    const int COL;
    Piece *piece;
  public:
    Square(int row, int col);
    // Place pc on square
    void place(Piece *pc);
    // Empties square
    void empty();
    // Return whether square is empty
    bool isEmpty();
    Piece* getPiece();
    int getRow();
    int getCol();
};

std::ostream& operator<<(std::ostream& out, Square& sq);

#endif
