
#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H
#include <iostream>

#include "board.h"
#include "observer.h"
#include "window.h"

class GraphicDisplay : public Observer {
  Board *subject;
  const int TILE_WIDTH = 80, TILE_HEIGHT = 80;
  Xwindow *window;

 public:
  GraphicDisplay(Board *s);
  void notify() override;
  ~GraphicDisplay();
};
#endif
