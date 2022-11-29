#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <utility>

using std::string;
using std::vector;
using std::shared_ptr;
class Square;
class Piece;

struct Move {
    Move(Square *start, Square *end): start{start}, end{end} {}
    Square *start;
    Square *end;
    shared_ptr<Piece> moving_piece;
    shared_ptr<Piece> killed_piece;
    bool is_attack = false;
    bool is_enpassant = false;
    bool is_promotion = false;
    bool is_castling = false;
    bool is_resign = false;
    bool is_fisrt_move = false;
    bool is_pseudo_legal = false;
};
#endif
