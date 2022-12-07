
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
  

 public:
  GraphicDisplay(Board *s);
  void notify() override;
  ~GraphicDisplay();
};
#endif


