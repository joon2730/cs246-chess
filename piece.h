#ifndef PIECE_H
#define PIECE_H
#include <iostream>
#include <string>

class Square;
class Move;

class Piece {
  protected:
    int color;
    int name;
    Square *location;
    bool has_moved = false;
    bool is_dead = false;
  public:
    Piece(int color, int name);
    enum {WHITE=0, BLACK};
    void setLocation(Square *sq);
    bool isDead();
    bool hasMoved();
    int getColor();
    virtual std::string printText() = 0;
    virtual bool canMove(Move *mv) = 0;
    virtual ~Piece();
};

#endif
