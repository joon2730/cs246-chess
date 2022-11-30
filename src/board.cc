#include "board.h"
#include <iostream>
#include <stdexcept>
#include "bishop.h"
#include "king.h"
#include "knight.h"
#include "move.h"
#include "pawn.h"
#include "piece.h"
#include "queen.h"
#include "rook.h"

Board::Board() {
  for (int i = 0; i < ROWS; ++i) {
    vector<Square> row;
    board.emplace_back(row);
    for (int j = 0; j < COLS; ++j) {
      board.at(i).push_back(Square(i, j));
    }
  }
}

Board::~Board() { empty(); }

void Board::render() { notifyObservers(); }

void Board::empty() {
  for (int i = 0; i < NUM_COLORS; ++i) {
    while (pieces[i].size() > 0) {
      pieces[i].pop_back();
    }
  }
  while (moves_played.size() > 0) {
    moves_played.pop_back();
  }
}

int Board::opponent(int color) {
  if (color == 0) {
    return 1;
  } else {
    return 0;
  }
}

void Board::addPiece(int piece, int color, string pos) {
  shared_ptr<Piece> new_piece;
  if (piece == PAWN) {
    new_piece = std::make_shared<Pawn>(color, PAWN);
  } else if (piece == KNIGHT) {
    new_piece = std::make_shared<Knight>(color, KNIGHT);
  } else if (piece == BISHOP) {
    new_piece = std::make_shared<Bishop>(color, BISHOP);
  } else if (piece == ROOK) {
    new_piece = std::make_shared<Rook>(color, ROOK);
  } else if (piece == QUEEN) {
    new_piece = std::make_shared<Queen>(color, QUEEN);
  } else if (piece == KING) {
    new_piece = std::make_shared<King>(color, KING);
    kings[color] = new_piece;
  } else {
    throw std::invalid_argument("invalid type of piece");
  }
  Square* sq = getSquare(pos);
  sq->place(new_piece);
  pieces[color].push_back(std::move(new_piece));
}

void Board::removePiece(string pos) {}

void Board::init() {
  empty();
  //WHITE
  addPiece(PAWN, WHITE, "a2");
  addPiece(PAWN, WHITE, "b2");
  addPiece(PAWN, WHITE, "c2");
  addPiece(PAWN, WHITE, "d2");
  addPiece(PAWN, WHITE, "e2");
  addPiece(PAWN, WHITE, "f2");
  addPiece(PAWN, WHITE, "g2");
  addPiece(PAWN, WHITE, "h2");

  addPiece(ROOK, WHITE, "a1");
  addPiece(KNIGHT, WHITE, "b1");
  addPiece(BISHOP, WHITE, "c1");
  addPiece(QUEEN, WHITE, "d1");
  addPiece(KING, WHITE, "e1");
  addPiece(BISHOP, WHITE, "f1");
  addPiece(KNIGHT, WHITE, "g1");
  addPiece(ROOK, WHITE, "h1");
  // BLACK
  addPiece(PAWN, BLACK, "a7");
  addPiece(PAWN, BLACK, "b7");
  addPiece(PAWN, BLACK, "c7");
  addPiece(PAWN, BLACK, "d7");
  addPiece(PAWN, BLACK, "e7");
  addPiece(PAWN, BLACK, "f7");
  addPiece(PAWN, BLACK, "g7");
  addPiece(PAWN, BLACK, "h7");

  addPiece(ROOK, BLACK, "a8");
  addPiece(KNIGHT, BLACK, "b8");
  addPiece(BISHOP, BLACK, "c8");
  addPiece(QUEEN, BLACK, "d8");
  addPiece(KING, BLACK, "e8");
  addPiece(BISHOP, BLACK, "f8");
  addPiece(KNIGHT, BLACK, "g8");
  addPiece(ROOK, BLACK, "h8");
  updateState();
}

bool Board::isPseudoLegal(Move& mv) {
  if (mv.start->isEmpty()) {
    throw std::invalid_argument("Board::isPseudoLegal: No piece to move found");
  }
  mv.moving_piece = mv.start->getPiece();
  // piece must be able to reach the square follwing the rule
  if (mv.moving_piece->canMove(*this, mv)) {
    mv.is_pseudo_legal = true;
    return true;
  }
  return false;
}

bool Board::isLegal(Move& mv) {
  if (isPseudoLegal(mv) && !isPuttingOwnKingInCheck(mv)) {
    mv.is_pseudo_legal = true;
    mv.is_legal = true;
    return true;
  }
  return false;
}

bool Board::isPuttingOwnKingInCheck(Move& mv) {
  if (!mv.is_pseudo_legal) {
    std::invalid_argument("isPuttingOwnKingInCheck: Move not pseudo-legal");
  }
  doMove(mv);
  bool own_king_checked = detectChecked(mv.moving_piece->getColor());
  undoMove(mv);
  if (!own_king_checked) {
    mv.is_legal = true;
    return false;
  }
  return true;
}

bool Board::isDangerousFor(Square *sq, int color) {
  for (auto op : pieces[opponent(color)]) {
    if (op->isDead()) {
      continue;
    } else {
      Move mv = Move(op->getPosition(), sq);
      if (isPseudoLegal(mv)) {
        return true;
      }
    }
  }
  return false;
}

vector<Move> Board::listLegalMoves(int color) {
  vector<Move> res;
  for (auto piece : pieces[color]) {
    // cannot move if piece is dead
    if (piece->isDead()) {
      continue;
    }
    // get all pseudo_legal moves for the piece
    vector<Move> pseudo_legal_moves = piece->listPseudoLegalMoves(*this);
    while (pseudo_legal_moves.size() > 0) {
      Move mv = pseudo_legal_moves.back();
      pseudo_legal_moves.pop_back();
      if (!isPuttingOwnKingInCheck(mv)) {
        res.push_back(std::move(mv));
      }
    }
  }
  return res;
}

bool Board::detectChecked(int color) {
  Square* king_pos = kings[color]->getPosition();
  return isDangerousFor(king_pos, color);
}

void Board::updateState() {
  for (int color = WHITE; color < NUM_COLORS; ++color) {
    checked[color] = detectChecked(color);
    vector<Move> legal_moves = listLegalMoves(color);
    if (legal_moves.size() == 0) {
      if (checked[color]) {
        checkmated[color] = true;
      } else {
        stalemated[color] = true;
      }
    } else {
      checkmated[color] = false;
      stalemated[color] = false;
    }
    stalemated[color] = false;
  }
}

void Board::movePiece(shared_ptr<Piece>& piece, Square* from, Square* to) {
  from->empty();
  to->place(piece);
}

void Board::doMove(Move& mv) {
  if (!mv.is_pseudo_legal) {
    // std::cout << *mv.start << " " << *mv.end << << " " << mv.start.getRow() << "\n";
    throw std::invalid_argument("doMove: Move's Pseudo-legality not checked");
  }
  mv.moving_piece = mv.start->getPiece();
  // if there is a piece being attacked
  if (!mv.end->isEmpty()) {
    mv.is_attack = true;
    mv.killed_piece = mv.end->getPiece();
  }

  // castling
  if (mv.is_kingside_castling) {
    int f_file = 'f' - 'a';
    int g_file = 'g' - 'a';
    auto rook = mv.end->getPiece();
    movePiece(mv.moving_piece, mv.start, getSquare(mv.start->getRow(), g_file));
    movePiece(rook, mv.end, getSquare(mv.end->getRow(), f_file));
  } else if (mv.is_queenside_castling) {
    int c_file = 'c' - 'a';
    int d_file = 'd' - 'a';
    auto rook = mv.end->getPiece();
    movePiece(mv.moving_piece, mv.start, getSquare(mv.start->getRow(), c_file));
    movePiece(rook, mv.end, getSquare(mv.end->getRow(), d_file));
  // normal move
  } else {
    // if the moving piece hasn't moved set is_first_move true
    if (!mv.moving_piece->getHasMoved()) {
      mv.is_first_move = true;
      mv.moving_piece->setHasMoved(true);
    }
    // move the moving piece
    movePiece(mv.moving_piece, mv.start, mv.end);
  }
}

void Board::push(Move& mv) {
  if (!mv.is_legal) {
    throw std::invalid_argument("push: Move's legality not checked");
  }
  // make movement
  doMove(mv);
  // update current status
  updateState();
  // record the move
  moves_played.push_back(mv);
}

void Board::undoMove(Move& mv) {
  if (mv.is_kingside_castling) {
    int f_file = 'f' - 'a';
    int g_file = 'g' - 'a';
    auto rook = getSquare(mv.end->getRow(), f_file)->getPiece();
    movePiece(mv.moving_piece, getSquare(mv.start->getRow(), g_file), mv.start);
    movePiece(rook, getSquare(mv.end->getRow(), f_file), mv.end);
  } else if (mv.is_queenside_castling) {
    int c_file = 'c' - 'a';
    int d_file = 'c' - 'a';
    auto rook = getSquare(mv.end->getRow(), d_file)->getPiece();
    movePiece(mv.moving_piece, getSquare(mv.start->getRow(), c_file), mv.start);
    movePiece(rook, getSquare(mv.end->getRow(), d_file), mv.end);
  } else {
    if (mv.end->isEmpty()) {
      throw std::invalid_argument("undoMove: no piece to move back found");
    }
    // move the moved piece backward
    movePiece(mv.moving_piece, mv.end, mv.start);
    // if there was a killed piece revoke it
    if (mv.is_attack) {
      mv.end->place(mv.killed_piece);
    }
    if (mv.is_first_move) {
      mv.moving_piece->setHasMoved(false);
    }
  }
}

void Board::pop() {
  if (moves_played.size() == 0) {
    std::logic_error("pop: No move to pop found");
  }
  // recall the last move
  Move& mv = moves_played.back();
  moves_played.pop_back();
  undoMove(mv);
  updateState();
}

Square* Board::getSquare(string pos) {
  bool is_length_two = (pos.size() == 2);
  if (is_length_two) {
    char file = pos[0];
    char rank = pos[1];
    bool is_file_valid = ('a' <= file) && (file <= 'h');
    bool is_rank_valid = ('1' <= rank) && (rank <= '8');
    if (is_file_valid && is_rank_valid) {
      int row = ROWS - (rank - '1') - 1;
      int col = file - 'a';
      return &board[row][col];
    }
  }
  throw std::invalid_argument("Square out of range\n");
}

Square* Board::getSquare(int row, int col) {
  if (row < 0 || ROWS <= row || col < 0 || COLS <= col) {
    throw std::invalid_argument("Square out of range\n");
  }
  return &board[row][col];
}

bool Board::isChecked(int color) { return checked[color]; }

bool Board::isCheckmated(int color) { return checkmated[color]; }

bool Board::isStalemated(int color) { return stalemated[color]; }

int Board::getRows() { return ROWS; }

int Board::getCols() { return COLS; }
