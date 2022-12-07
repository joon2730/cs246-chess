#include "pawn.h"
#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"
#include <iostream>

Pawn::Pawn(int color, int name) : Piece(color, name) {}

bool Pawn::canMove(Board& board, Move& mv) {
    // can't move to square with a piece with same color
    if ((!mv.end->isEmpty()) && (color == mv.end->getPiece()->getColor())) {
        return false;
    }
    int hoz_move = dist(mv.start->getCol(), mv.end->getCol());
    int ver_move = dist(mv.start->getRow(), mv.end->getRow());
    int ver_step = sign(disp(mv.start->getRow(), mv.end->getRow()));
    // normal move
    if (mv.end->isEmpty() && hoz_move != ver_move) {
        // cannot move horizontally
        if (hoz_move != 0) {
            return false;
        } else if (ver_move == 2) {
            // can only move 2 squares when it's first move
            if (has_moved) {
                return false;
            }
            // when it moves 2 squares the square in between has to be empty
            int cur_col = mv.start->getCol();
            int cur_row = mv.start->getRow() + ver_step;
            if (!board.getSquare(cur_row, cur_col)->isEmpty()) {
                return false;
            }
            mv.is_pawns_two_square_move = true;
        } else if (ver_move > 2) {
            return false;
        }
    // if it is an attacking move
    } else if (!mv.end->isEmpty() && hoz_move == ver_move) {
        // have to move diagonally 
        if (hoz_move != ver_move) {
            return false;
        // can only move 1 square
        } else if (ver_move != 1) {
            return false;
        }
    // en passant
    } else if (mv.end->isEmpty() && hoz_move == ver_move) {
        Square *target = board.getSquare(mv.start->getRow(), mv.end->getCol());
        if (target->isEmpty()) {
            return false;
        } else if (target->getPiece()->getName() != board.PAWN) {
            return false;
        }
        if (board.getNumMovesPlayed() <= 0) {
            return false;
        }
        Move last_move = board.getLastMove();
        if (last_move.moving_piece != target->getPiece()) {
            return false;
        } else if (!last_move.is_pawns_two_square_move) {
            return false;
        }
        mv.is_enpassant = true;
    // can't move forward to square with piece with any color
    } else {
        return false;
    }
    // can only move toward opposite end of the board
    if (color == WHITE) {
        // if not move toward black's side 
        if (!(ver_step < 0)) {
            return false;
        }
        // promotion
        int rank_8 = '8' - '8';
        if (mv.end->getRow() == rank_8) {
            // if (!(board.KNIGHT <= mv.promote_to && mv.promote_to <= board.QUEEN)) {
            //     return false;
            // }
            mv.is_promotion = true;
        }
    } else if (color == BLACK) {
        // if not move toward white's side
        if (!(ver_step > 0)) {
            return false;
        }
        // promotion
        int rank_1 = '8' - '1';
        if (mv.end->getRow() == rank_1) {
            // if (!(board.KNIGHT <= mv.promote_to && mv.promote_to <= board.QUEEN)) {
            //     return false;
            // }
            mv.is_promotion = true;
        }
    }
    mv.is_pseudo_legal = true;
    return true;
}


vector<Move> Pawn::listPseudoLegalMoves(Board& board) {
    vector<Move> possible_moves;
    int row = position->getRow();
    int col = position->getCol();
    int i;
    if (color == WHITE) {
        i = -1;
    } else {
        i = 1;
    }
    for (int j = -1; j <= 1; ++j) {
        if (board.inRange(row+i, col+j)) {
            Move mv = Move(position, board.getSquare(row+i, col+j));
            if (canMove(board, mv)) {
                if (mv.is_promotion) {
                    for (int t = board.KNIGHT; t <= board.QUEEN; ++t) {
                        Move prom_mv = Move(position, board.getSquare(row+i, col+j));
                        prom_mv.promote_to = t;
                        prom_mv.is_promotion = true;
                        prom_mv.is_pseudo_legal = true;
                        possible_moves.push_back(prom_mv);                      
                    }
                } else {
                    possible_moves.push_back(mv);
                }
            }
        }
    }
    if (board.inRange(row+(i*2), col)) {
        Move mv = Move(position, board.getSquare(row+(i*2), col));
        if (canMove(board, mv)) {
            if (mv.is_promotion) {
                for (int t = board.KNIGHT; t <= board.QUEEN; ++t) {
                    Move prom_mv = Move(position, board.getSquare(row+(i*2), col));
                    prom_mv.promote_to = t;
                    prom_mv.is_promotion = true;
                    prom_mv.is_pseudo_legal = true;
                    possible_moves.push_back(prom_mv);      
                }
            } else {
                possible_moves.push_back(mv);
            }
        }
    }
    return possible_moves;
}

string Pawn::printText() {
  if (color == WHITE) {
    return "P";
  } else {
    return "p";
  }
}

bool Pawn::getMoved() { return moved_two_steps; }

