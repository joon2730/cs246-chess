#include "graphicdisplay.h"

#include <iostream>
#include <string>
#include "board.h"
#include "piece.h"
#include "square.h"
#include "window.h"

GraphicDisplay::GraphicDisplay(Board *s): subject{s} { 
  subject->attach(this);

  width = (TILE_WIDTH + HOZ_GAP) * subject->getCols() + HOZ_GAP + EDGE_LEFT;
  height = (TILE_HEIGHT + VER_GAP) * subject->getRows() + VER_GAP + EDGE_BOTTOM;
  window = new Xwindow(width, height);

  window->fillRectangle(0, 0, width, height, window->DimGray);

  for(int i = 0; i < subject->getRows(); ++i) {
    std::vector<char> row;
    current_state.emplace_back(row);

    for (int j = 0; j < subject->getCols(); ++j) {
      current_state.at(i).push_back(' ');

      int x = j * (TILE_WIDTH + HOZ_GAP) + HOZ_GAP + EDGE_LEFT;
      int y = i * (TILE_HEIGHT + VER_GAP) + VER_GAP;
      window->fillRectangle(x, y, TILE_WIDTH, TILE_WIDTH, window->White);
      if (i == subject->getRows() - 1) {
        char msg = 'a' + j;
        std::string s = "";
        s = msg;
        window->drawBigString(x + (TILE_WIDTH / 2), height - (EDGE_BOTTOM / 4), s, window->White);
      }
      if (j == 0) {
        char msg = '8' - i;
        std::string s = "";
        s = msg;
        window->drawBigString((EDGE_LEFT / 4), y + TILE_HEIGHT, s, window->White);
      }
    }
  }
}

void GraphicDisplay::notify() {
  for (int row = 0; row < 8; ++row) {
    for (int col = 0; col < 8; ++col) {
      Square* sq = subject->getSquare(row, col);

      bool update = false;
      if (sq->isEmpty()) {
        if (current_state.at(row).at(col) != ' ') {
          update = true;
          current_state.at(row).at(col) = ' ';
        }
      } else if (current_state.at(row).at(col) != sq->getPiece()->printText()[0]) {
        update = true;
        current_state.at(row).at(col) = sq->getPiece()->printText()[0];
      } 

      if (update) {

        int x = col * (TILE_WIDTH + HOZ_GAP) + HOZ_GAP + EDGE_LEFT;
        int y = row * (TILE_HEIGHT + VER_GAP) + VER_GAP;
        if (sq->isEmpty()) {
          window->fillRectangle(x, y, TILE_WIDTH, TILE_HEIGHT, window->White);
        } else {
          char name = sq->getPiece()->printText()[0];
          // int piece_color = sq->getPiece()->getColor();

          // window->drawString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), text);
          // window->drawBigString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), "\u2654", window->Black);
          window->drawPiece(name, x, y, TILE_WIDTH, TILE_HEIGHT);
        }
      }
    }
  }
}

GraphicDisplay::~GraphicDisplay() { subject->detach(this); }


