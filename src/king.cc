#include "king.h"

#include "board.h"
#include "math.h"
#include "move.h"
#include "square.h"

King::King(int color, int name) : Piece(color, name) {}

bool King::canMove(Board& board, Move& mv) {
  try {
  
  // normal move
  if (mv.end->isEmpty() || color != mv.end->getPiece()->getColor()) {
    // can move to neighbouring squares
    int hoz_move = dist(mv.start->getCol(), mv.end->getCol());
    int ver_move = dist(mv.start->getRow(), mv.end->getRow());
    if (!(hoz_move <= 1 && ver_move <= 1)) {
      return false;
    }
  // castling move
  // can only castle under specific conditions
  // follows fisher random chess: https://en.wikipedia.org/wiki/Fischer_random_chess#Castling_rules
  } else {
    // Rook with same color at the end square
    if (!(mv.end->getPiece()->getName() == board.ROOK && mv.end->getPiece()->getColor() == color)) {
      return false;
    // Must unmoved and unattacked(currently)
    } else if (has_moved || board.isChecked(color)) {
      return false;
    // Must be on the same row
    } else if (mv.end->getRow() != mv.start->getRow()) {
      return false;
    }
    // Determine whether it is kingside or queenside castling
    int rook_step, king_step, rook_end, king_end;
    if (mv.start->getCol() < mv.end->getCol()) { // if rook is on the rightside
      mv.is_kingside_castling = true;
      int f_file = 'f' - 'a';
      int g_file = 'g' - 'a';
      rook_end = f_file;
      king_end = g_file;
    } else { // if rook is on the leftside
      mv.is_queenside_castling = true;
      int c_file = 'c' - 'a';
      int d_file = 'd' - 'a';
      rook_end = d_file;
      king_end = c_file;
    }
    rook_step = sign(disp(mv.end->getCol(), rook_end));
    king_step = sign(disp(mv.start->getCol(), king_end));
    // All squares between rook's initial and final squares must be vacant
    int rook_cur_col = mv.end->getCol() + rook_step;
    while (rook_cur_col != rook_end) {
      if (!board.getSquare(mv.end->getRow(), rook_cur_col)->isEmpty()) {
        return false;
      }
      rook_cur_col += rook_step;
    }
    // All squares between king's initial and final squares must be vacant AND unattacked
    int king_cur_col = mv.start->getCol() + king_step;
    while (king_cur_col != king_end) {
      if (!board.getSquare(mv.start->getRow(), king_cur_col)->isEmpty()) {
        return false;
      } else if (board.isDangerousFor(board.getSquare(mv.start->getRow(), king_cur_col), color)) {
        return false;
      }
      king_cur_col += king_step;
    }
  }
  mv.is_pseudo_legal = true;
  } catch (...) {
            std::cout << "here\n";
        }
  return true;
}

vector<Move> King::listPseudoLegalMoves(Board& board) {
  vector<Move> pseudo_legal_moves;
    for (int i = 0; i < board.getRows(); ++i) {
        for (int j = 0; j < board.getCols(); ++j) {
            Move mv = Move(position, board.getSquare(i, j));
            if (canMove(board, mv)) {
                pseudo_legal_moves.push_back(mv);
            }
        } 
    }
    return pseudo_legal_moves;
  // vector<Move> pseudo_legal_moves;
  // int col = position->getCol();
  // int row = position->getRow();
  // int cols = board.COLS;
  // int rows = board.ROWS;
  // for (int col_step = -1; col_step <= 1; ++col_step) {
  //   for (int row_step = -1; row_step <= 1; ++row_step) {
  //     int cur_col = col + col_step;
  //     int cur_row = row + row_step;
  //     if (!(cur_row < 0 || rows <= cur_row || cur_col < 0 || cols <= cur_col)) {
  //       Square* cur_pos = board.getSquare(cur_row, cur_col);
  //       if (cur_pos->isEmpty() || color != cur_pos->getPiece()->getColor()) {
  //         pseudo_legal_moves.emplace_back(position, board.getSquare(cur_row, cur_col));
  //       }
  //     }
  //   }
  // }
  // return pseudo_legal_moves;
}

string King::printText() {
  if (color == WHITE) {
    return "K";
  } else {
    return "k";
  }
}
