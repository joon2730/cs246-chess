#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <string>
#include <utility>
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
  Piece(int color, int name);
  enum { WHITE = 0, BLACK };
  void setPosition(Square *sq);
  int getColor();
  Square* getPosition();
  bool isDead();
  bool hasMoved();
  virtual string printText() = 0;
  virtual bool canMove(Board& board, Move& mv) = 0;
  virtual vector<Move> listPossibleMoves(Board& board) = 0;
  virtual ~Piece();
};

#endif
