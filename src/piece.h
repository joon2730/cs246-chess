#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <string>
#include <utility>
#include <memory>
#include <vector>

class Square;
class Board;
struct Move;

using std::shared_ptr;
using std::string;
using std::vector;

class Piece {
 protected:
  int color;
  int name;
  Square* position;
  bool has_moved = false;
 public:
  enum { WHITE = 0, BLACK };
  Piece(int color, int name);
  int getColor();
  int getName();
  void setPosition(Square *sq);
  Square* getPosition();
  bool getHasMoved();
  void setHasMoved(bool has_moved);
  bool isDead();
  virtual string printText() = 0;
  virtual bool canMove(Board& board, Move& mv) = 0;
  virtual vector<Move> listPseudoLegalMoves(Board& board) = 0;
  virtual ~Piece() = default;
};

#endif

