#include "pawn.h"
#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"
#include <iostream>

Pawn::Pawn(int color, int name) : Piece(color, name) {}

bool Pawn::canMove(Board& board, shared_ptr<Move>& mv) {
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    // can't move to square with a piece with same color
    if ((!end->isEmpty()) && (color == end->getPiece()->getColor())) {
        return false;
    }
    int hoz_move = dist(start->getCol(), end->getCol());
    int ver_move = dist(start->getRow(), end->getRow());
    int ver_step = sign(disp(start->getRow(), end->getRow()));
    if (end->isEmpty()) {
        // cannot move horizontally
        if (hoz_move != 0) {
            return false;
        }
        // can move only 1 square when it's been moved before
        if (has_moved) {
            if (ver_move > 1) {
                return false;
            }
        } else {
            // can move no more than 2 square when it's first move
            if (ver_move > 2) {
              return false;
            // when it moves 2 squares the square in between has to be empty
            } else if (ver_move == 2) {
                int cur_col = start->getCol();
                int cur_row = start->getRow() + ver_step;
                if (!board.getSquare(cur_row, cur_col)->isEmpty()) {
                  return false;
                }
            }
        }
    // if it is an attacking move
    } else {
        // have to move diagonally 
        if (hoz_move != ver_move) {
            return false;
        // can only move 1 square
        } else if (ver_move != 1) {
            return false;
        }
    }
    // can only move toward opposite end of the board
    if (color == WHITE) {
        // if not move toward black's side 
        if (!(ver_step < 0)) {
            return false;
        }
    } else if (color == BLACK) {
      // if not move toward white's side
      if (!(ver_step > 0)) {
            return false;
        }
    }
    return true;
}


vector<shared_ptr<Move>> Pawn::listPossibleMoves(Board& board) {
    // cout << printText() << "\n";
    vector<shared_ptr<Move>> possible_moves;
    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getCols(); ++j) {
            // cout << i << " " << j << "\n";
            shared_ptr<Move> mv = std::make_shared<Move>(position, board.getSquare(i, j));
            if (canMove(board, mv)) {
                possible_moves.push_back(std::move(mv));
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
