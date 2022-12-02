#include "move.h"
#include "piece.h"
#include "square.h"

std::ostream& operator<<(std::ostream& out, Move &mv) {
  if (mv.moving_piece) {
    out << "Move start: " << *mv.start << " end: " << *mv.end <<
      " from (" << mv.start->getCol() << "," << mv.start->getRow() << 
      ") to (" << mv.end->getCol() << "," << mv.end->getRow() << ")";
    if (mv.is_attack) {
      out << "attacking " << *mv.end;
    }
    if (mv.is_promotion) {
      if (mv.retired_piece) {
        out << " " << mv.retired_piece->getName() << "retired";
      } else {
        out << " no one retired yet";
      }
      out << " [promotion]";
    } else if (mv.is_enpassant) {
      out << " [enpassant]";
    } else if (mv.is_kingside_castling) {
      out << " [O-O]";
    } else if (mv.is_queenside_castling) {
      out << " [O-O-O]";
    }
  } else {
    out << "[no moving piece] Move start: " << *mv.start << " end: " << *mv.end <<
      " from (" << mv.start->getCol() << "," << mv.start->getRow() << 
      ") to (" << mv.end->getCol() << "," << mv.end->getRow() << ")";
  }
  out << std::endl;
  return out;
  
  
}