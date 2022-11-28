#include "textdisplay.h"
#include <string>
TextDisplay::TextDisplay(Board *s): subject{s} {
  subject->attach(this);
}

TextDisplay::~TextDisplay() {
  subject->detach(this);
}

void TextDisplay::notify() {
    for (int i = 0; i < 8; ++i) {
        out << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            out << *subject->getSquare(i, j);
        }
        out << "\n";
    }
    out << "  abcdefgh\n";
    std::string opnames[2] = {"Black", "White"};
    for (int color = subject->WHITE; color < subject->NUM_COLORS; ++color) {
        if (subject->isChecked(color)) {
            if (subject->isCheckmated(color)) {
                out << opnames[color] << " checkmate!\n";
            } else {
                out << opnames[color] << " check\n";
            }
        } else if (subject->isStalemated(color)) {
            out << " stalemate\n";
        }
     } 
    out << "\n";
}

