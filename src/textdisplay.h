#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "board.h"
#include <iostream>

class TextDisplay: public Observer {
  Board *subject;
  std::ostream &out = std::cout;
 public:
  TextDisplay(Board *s);
  void notify() override;
  ~TextDisplay();
};
#endif

