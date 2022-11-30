#ifndef MOVE_H
#define MOVE_H

#include <memory>
#include <utility>
class Square;
class Piece;

using std::shared_ptr;

struct Move {
    Move(Square *start, Square *end): start{start}, end{end} {}
    Square *start;
    Square *end;
    shared_ptr<Piece> moving_piece;
    shared_ptr<Piece> killed_piece;
    bool is_attack = false;
    bool is_enpassant = false;
    bool is_promotion = false;
    bool is_kingside_castling = false;
    bool is_queenside_castling = false;
    bool is_resign = false;
    bool is_first_move = false;
    bool is_pseudo_legal = false;
    bool is_legal = false;

};
#endif
