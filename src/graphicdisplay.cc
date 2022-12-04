#include "graphicdisplay.h"

#include <iostream>
#include <string>

#include "board.h"
#include "piece.cc"
#include "square.h"
#include "window.h"
using namespace std;

GraphicDisplay::GraphicDisplay(Board *s) subject{s} { subject->attach(this); }

void GraphicDisplay::notify() {
  int row;
  int col;
  int piece_color;
  int tile_color;
  int name;

  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      row = subject->getSquare(i, j)->getRow();
      col = subject->getSquare(i, j)->getCol();
      piece_color = subject->getSquare(i, j)->getPiece()->getColor();
      name = subject->getSquare(i, j)->getPiece()->getName();

      if ((row + col) % 2 == 0)
        tile_color = 0;
      else
        tile_color = 1;

      if (color == WHITE) {
        piece_color = color + 4
      } else {
        piece_color = color + 9
      }

      if (subject->getSquare(i, j)->isEmpty()) {
        window.fillRectangle(200 * col + 200, 200 * row + 20, 200, 200,
                             tile_color);
      } else {
        window.fillRectangle(200 * col + 200, 200 * row + 20, 200, 200,
                             tile_color);
        switch (name) {
          case PAWN:
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 30,
                              piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 30,
                              piece_color);
            break;
          case KNIGHT:
            window.fillRectangle(col * 200 + 40 + 200, row * 200 + 40 + 20, 20,
                                 40, piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 40 + 20, 40,
                              piece_color);
            break;
          case BISHOP:
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 30,
                              piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 40,
                              piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 20,
                              piece_color);
            window.fillRectangle(col * 200 + 40 + 200, row * 200 + 50 + 20, 5,
                                 2, piece_color);
            break;
          case ROOK:
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 30,
                              piece_color);
            window.fillRectangle(col * 200 + 40 + 200, row * 200 + 20 + 20, 20,
                                 5, piece_color);
            window.fillRectangle(col * 200 + 20 + 200, row * 200 + 40 + 20, 5,
                                 20, piece_color);
            window.fillRectangle(col * 200 + 40 + 200, row * 200 + 70 + 20, 20,
                                 5, piece_color);
            window.fillRectangle(col * 200 + 70 + 200, row * 200 + 40 + 20, 5,
                                 20, piece_color);
            break;
          case QUEEN:
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 30,
                              piece_color);

            window.fillCircle(col * 200 + 20 + 200, row * 200 + 50 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 50 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 20 + 200, row * 200 + 30 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 50 + 200, row * 200 + 20 + 20, 5,
                              piece_color);

            window.fillCircle(col * 200 + 40 + 200, row * 200 + 40 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 70 + 200, row * 200 + 70 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 70 + 200, row * 200 + 40 + 20, 5,
                              piece_color);
            window.fillCircle(col * 200 + 40 + 200, row * 200 + 70 + 20, 5,
                              piece_color);
            break;
          case KING:
            window.fillRectangle(col * 200 + 40 + 200, row * 200 + 20 + 20, 20,
                                 30, piece_color);
            window.fillRectangle(col * 200 + 20 + 200, row * 200 + 40 + 20, 30,
                                 20, piece_color);
            break;
        }
      }
    }
  }
}

GraphicDisplay::~GraphicDisplay() { subject->detach(this); }
