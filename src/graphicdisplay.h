
#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H
#include <iostream>
#include <vector>
#include "board.h"
#include "observer.h"
#include "window.h"

class GraphicDisplay : public Observer {
  Board *subject;
  int width, height;
  const int TILE_WIDTH = 80, TILE_HEIGHT = 80, VER_GAP = 5, HOZ_GAP = 5,
     EDGE_LEFT = 20, EDGE_BOTTOM = 20;
  Xwindow *window;
  std::vector<std::vector<char>> current_state;
  // Last-move highlight tracking (so we can repaint old highlights).
  int last_from_row = -1;
  int last_from_col = -1;
  int last_to_row = -1;
  int last_to_col = -1;
  

 public:
  GraphicDisplay(Board *s);
  void notify() override;
  ~GraphicDisplay();
};
#endif


