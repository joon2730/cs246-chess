#include "openingbook.h"

#include "board.h"
#include "move.h"
#include "square.h"

#include <cstdlib>

namespace {

static std::string coord(Square* sq) {
    char file = static_cast<char>('a' + sq->getCol());
    char rank = static_cast<char>('8' - sq->getRow());
    std::string s;
    s += file;
    s += rank;
    return s;
}

static std::string moveToString(const Move& mv) {
    return coord(mv.start) + coord(mv.end);
}

static bool prefixMatches(const std::vector<std::string>& history, const OpeningBook::Line& line) {
    if (history.size() > line.size()) return false;
    for (size_t i = 0; i < history.size(); ++i) {
        if (history[i] != line[i]) return false;
    }
    return true;
}

} // namespace

bool OpeningBook::trySelectNextMove(Board& board, int side, const std::vector<Line>& lines, Move& out) {
    const int ply = board.getNumMovesPlayed();

    // Side-to-move must match ply parity: even ply -> White, odd ply -> Black.
    if ((ply % 2 == 0 && side != board.WHITE) || (ply % 2 == 1 && side != board.BLACK)) {
        return false;
    }

    std::vector<std::string> history;
    history.reserve(static_cast<size_t>(ply));
    for (int i = 0; i < ply; ++i) {
        history.push_back(moveToString(board.getMovePlayed(i)));
    }

    // Collect candidate next moves from all matching lines.
    std::vector<std::string> candidates;
    for (const auto& line : lines) {
        if (!prefixMatches(history, line)) continue;
        if (static_cast<int>(line.size()) <= ply) continue;
        candidates.push_back(line[static_cast<size_t>(ply)]);
    }
    if (candidates.empty()) return false;

    // Randomly try candidates until a legal one is found.
    for (int attempt = 0; attempt < 12 && !candidates.empty(); ++attempt) {
        const size_t idx = static_cast<size_t>(std::rand() % candidates.size());
        const std::string mv = candidates[idx];
        candidates.erase(candidates.begin() + static_cast<long>(idx));

        if (mv.size() != 4) continue;
        const std::string from = mv.substr(0, 2);
        const std::string to = mv.substr(2, 2);

        out = Move(board.getSquare(from), board.getSquare(to));
        if (board.isLegal(out)) return true;
    }

    return false;
}

const std::vector<OpeningBook::Line>& OpeningBook::level5Lines() {
    // Add new opening lines here (list form).
    // Each line is a sequence of "e2e4" strings (from+to), in ply order.
    static const std::vector<Line> lines = {
        // 1.e4 e5 2.Nf3
        {"e2e4", "g8f6", "g1f3"},
        // 1.d4 d5 2.c4
        {"d2d4", "d7d5", "c2c4"},
    };
    return lines;
}

