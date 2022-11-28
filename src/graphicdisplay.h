
#ifndef GRAPHICOBSERVER_H
#define GRAPHICOBSERVER_H

#include "observer.h"
#include "board.h"
#include "window.h"

class GraphicObserver: public Observer {
  Studio *subject;
  int top, bottom, left, right;
  const int PIXEL_WIDTH = 10, PIXEL_HEIGHT = 10;
  Xwindow *window;
  int **current_state;
 public:
  GraphicObserver(Studio *s, int top, int bottom, int left, int right);
  void notify() override;
  ~GraphicObserver();
};
#endif

