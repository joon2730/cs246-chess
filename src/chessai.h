#ifndef CHESSAI_H
#define CHESSAI_H

#include <vector>
#include <climits>
#include <memory>
#include <chrono>
#include <exception>
class Board;
class Piece;
struct Move;

using std::vector;

class ChessAI {
    bool time_limited = false;
    std::chrono::steady_clock::time_point deadline;
    void checkTime() const;
    static const int MATE_SCORE = 100000;
    int alphabetaMax(Board& board, int alpha, int beta, int depthleft, int ply_from_root);
    int alphabetaMin(Board& board, int alpha, int beta, int depthleft, int ply_from_root);
  protected:
    // Thrown internally when the time limit expires.
    struct TimeUp final : public std::exception {
        const char* what() const noexcept override { return "ChessAI time limit exceeded"; }
    };
    enum {WHITE=0, BLACK};
    const int MAXIMIZING_PLAYER = WHITE;
    const int MINIMIZING_PLAYER = BLACK;
    vector<Move> searchMoves(Board& board, int side, int depth);
    vector<std::shared_ptr<Piece>>& getPieces(Board& board, int color);
    virtual int evaluateBoard(Board& board) = 0;
    void setTimeLimit(std::chrono::steady_clock::time_point dl);
    void clearTimeLimit();
    virtual ~ChessAI() = default;

};
#endif


