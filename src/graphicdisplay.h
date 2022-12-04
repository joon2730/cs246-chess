
#ifndef GRAPHICDISPLAY_H
#define GRAPHICDISPLAY_H
#include <iostream>

#include "board.h"
#include "observer.h"
// #include "window.h"

class GraphicDisplay : public Observer {
  Board *subject;
  const int PIXEL_WIDTH = 20, PIXEL_HEIGHT = 20;
  Xwindow *window;

 public:
  GraphicDisplay(Board *s);
  void notify() override;
  ~GraphicDisplay();
};
#endif
