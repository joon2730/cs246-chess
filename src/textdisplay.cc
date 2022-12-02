#include "textdisplay.h"
#include <string>

using std::string;

TextDisplay::TextDisplay(Board *s) : subject{s} { subject->attach(this); }

TextDisplay::~TextDisplay() { subject->detach(this); }

void TextDisplay::notify() {
  // for debugging
  string name_of_pieces[2][6] = {{"P", "N", "B", "R", "Q", "K"},
                           {"p", "n", "b", "r", "q", "k"}};
  for (int color = 0; color < 2; ++color) {
    for (int type = 0; type < subject->NUM_TYPE_PIECES; ++type) {
      out << name_of_pieces[color][type] << ": " <<
        subject->num_alive_pieces[color][type] <<
        " ";
    }
    out << "\n";
  }
  //
  for (int i = 0; i < 8; ++i) {
    out << 8 - i << " ";
    for (int j = 0; j < 8; ++j) {
      out << *subject->getSquare(i, j);
    }
    out << "\n";
  }
  out << "\n  abcdefgh\n";
  string names[2] = {"White", "Black"};
  string opnames[2] = {"Black", "White"};
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
