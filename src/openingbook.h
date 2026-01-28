#ifndef OPENINGBOOK_H
#define OPENINGBOOK_H

#include <string>
#include <vector>

class Board;
struct Move;

namespace OpeningBook {

// Each move is a 4-char string: "e2e4" (from+to).
using Line = std::vector<std::string>;

// Return true and write chosen move to `out` if the current position
// matches a prefix of the book and a legal next move exists for `side`.
bool trySelectNextMove(Board& board, int side, const std::vector<Line>& lines, Move& out);

// Level 5 opening book lines (editable list).
const std::vector<Line>& level5Lines();

} // namespace OpeningBook

#endif

