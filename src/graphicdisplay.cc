#include "graphicdisplay.h"

#include <iostream>
#include <string>
#include "board.h"
#include "piece.h"
#include "square.h"
#include "window.h"

GraphicDisplay::GraphicDisplay(Board *s): subject{s} { 
  subject->attach(this);
  window = new Xwindow(TILE_WIDTH * subject->getCols(), TILE_HEIGHT * subject->getCols());
  // for(int i = 0; i < rows; ++i) {
  // current_state[i] = new int[cols];
  // }
}

void GraphicDisplay::notify() {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      Square* sq = subject->getSquare(row, col);

      int tile_color;
      if ((row + col) % 2 == 0) {
        tile_color = window->AliceBlue;
      } else {
        tile_color = window->LightBlue;
      }
      int x = col * TILE_WIDTH;
      int y = row * TILE_HEIGHT;
      window->fillRectangle(x, y, TILE_WIDTH, TILE_HEIGHT, tile_color);

      if (!sq->isEmpty()) {
        char name = sq->getPiece()->printText()[0];
        // int piece_color = sq->getPiece()->getColor();

        // window->drawString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), text);
        // window->drawBigString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), "\u2654", window->Black);
        window->drawPiece(name, x, y, TILE_WIDTH, TILE_HEIGHT);
      }
    }
  }
}

GraphicDisplay::~GraphicDisplay() { subject->detach(this); }
