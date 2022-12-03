#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include "observer.h"
#include "board.h"
#include <iostream>
#include <utility>

using std::unique_ptr;

class TextDisplay: public Observer {
  Board *subject;
  std::ostream &out = std::cout;
 public:
  explicit TextDisplay(Board *s);
  void notify() override;
  ~TextDisplay();
};
#endif

