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
      // a8 (row 0, col 0) is dark
      bool dark = ((i + j) % 2 == 0);
      window->fillTile(x, y, TILE_WIDTH, TILE_HEIGHT, dark);
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
  int new_from_row = -1, new_from_col = -1;
  int new_to_row = -1, new_to_col = -1;
  bool new_to_is_capture = false;

  if (subject->getNumMovesPlayed() > 0) {
    Move last = subject->getLastMove();
    if (last.start && last.end) {
      new_from_row = last.start->getRow();
      new_from_col = last.start->getCol();
      new_to_row = last.end->getRow();
      new_to_col = last.end->getCol();
      new_to_is_capture = (last.killed_piece != nullptr) || last.is_attack || last.is_enpassant;
    }
  }

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

      const bool was_last_highlight =
          (row == last_from_row && col == last_from_col) || (row == last_to_row && col == last_to_col);
      const bool is_new_from = (row == new_from_row && col == new_from_col);
      const bool is_new_to = (row == new_to_row && col == new_to_col);
      const bool is_new_highlight = is_new_from || is_new_to;

      if (was_last_highlight || is_new_highlight) {
        update = true;
      }

      if (update) {

        int x = col * (TILE_WIDTH + HOZ_GAP) + HOZ_GAP + EDGE_LEFT;
        int y = row * (TILE_HEIGHT + VER_GAP) + VER_GAP;
        bool dark = ((row + col) % 2 == 0);
        // Always repaint the tile first so piece redraws don't leave artifacts.
        // Highlight last move with a border so the tile texture stays visible:
        // - from: teal-ish border
        // - to: teal-ish border, but sandy-orange border if it was a capture (incl. en passant)
        window->fillTile(x, y, TILE_WIDTH, TILE_HEIGHT, dark);
        if (is_new_from) {
          window->drawHighlightBorder(x, y, TILE_WIDTH, TILE_HEIGHT, window->MoveHighlight, 4);
        } else if (is_new_to) {
          window->drawHighlightBorder(x, y, TILE_WIDTH, TILE_HEIGHT,
                                      new_to_is_capture ? window->CaptureHighlight : window->MoveHighlight,
                                      new_to_is_capture ? 6 : 4);
        }
        if (sq->isEmpty()) {
          // tile already painted
        } else {
          char name = sq->getPiece()->printText()[0];
          // int piece_color = sq->getPiece()->getColor();

          // window->drawString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), text);
          // window->drawBigString(x + (TILE_WIDTH / 2), y + (TILE_HEIGHT / 2), "\u2654", window->Black);
          window->drawPiece(name, x, y, TILE_WIDTH, TILE_HEIGHT, dark);
        }
      }
    }
  }

  last_from_row = new_from_row;
  last_from_col = new_from_col;
  last_to_row = new_to_row;
  last_to_col = new_to_col;
}

GraphicDisplay::~GraphicDisplay() { subject->detach(this); }


