#include "textdisplay.h"

#include <string>
TextDisplay::TextDisplay(Board *s) : subject{s} { subject->attach(this); }

TextDisplay::~TextDisplay() { subject->detach(this); }

void TextDisplay::notify() {
  for (int i = 0; i < 8; ++i) {
    out << 8 - i << " ";
    for (int j = 0; j < 8; ++j) {
      out << *subject->getSquare(i, j);
    }
    out << "\n";
  }
  out << "\n  abcdefgh\n";
  std::string names[2] = {"White", "Black"};
  std::string opnames[2] = {"Black", "White"};
  for (int color = subject->WHITE; color < subject->NUM_COLORS; ++color) {
    if (subject->isChecked(color)) {
      if (subject->isCheckmated(color)) {
        out << "Checkmate! ";
        out << opnames[color] << " wins!\n";
      } else {
        out << names[color] << " is in Check.\n";
      }
    } else if (subject->isStalemated(color)) {
      out << "Stalemate\n";
    }
  }
  out << "\n";
}
