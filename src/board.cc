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
      if (!pieces[i].back()->isDead()) {
        pieces[i].back()->getPosition()->empty();
      }
      pieces[i].pop_back();
    }
  }
  while (moves_played.size() > 0) {
    moves_played.pop_back();
  }
  for (int color = 0; color < NUM_COLORS; ++color) {
      for (int type = 0; type < NUM_PIECE_TYPES; ++type) {
          num_alive_pieces[color][type] = 0;
      }
  }
}

int Board::opponent(int color) {
  if (color == 0) {
    return 1;
  } else {
    return 0;
  }
}

void Board::addPiece(int color, int piece, Square *sq) {
  if (color != WHITE && color != BLACK) {
    throw std::invalid_argument("addPiece: Invalid color");
  }
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
  num_alive_pieces[color][piece] += 1;
  sq->empty();
  sq->place(new_piece);
  pieces[color].push_back(std::move(new_piece));
}

void Board::removePiece(shared_ptr<Piece>& pc) {
    pc->getPosition()->empty();
    num_alive_pieces[pc->getColor()][pc->getName()] -= 1;
    int color = pc->getColor();
    int len = pieces[color].size();
    for (int i = len - 1; i >= 0; --i) {
        if (pc == pieces[color].at(i)) {
            pieces[color].erase(pieces[color].begin() + i);
            return;
        }
    }
}

void Board::init() {
  empty();
  int backrow[COLS] = {ROOK, KNIGHT, BISHOP, QUEEN, KING, BISHOP, KNIGHT, ROOK};
  for (int col = 0; col < COLS; ++col) {
    addPiece(BLACK, backrow[col], getSquare(0, col));
    addPiece(BLACK, PAWN, getSquare(1, col));
    addPiece(WHITE, PAWN, getSquare(6, col));
    addPiece(WHITE, backrow[col], getSquare(7, col));
  }
  for (int color = WHITE; color < NUM_COLORS; ++color) {
    resigned[color] = false;
  }
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
  if (isPseudoLegal(mv) && !isChecking(mv, mv.moving_piece->getColor())) {
    mv.is_pseudo_legal = true;
    mv.is_legal = true;
    return true;
  }
  return false;
}

bool Board::isChecking(Move& mv, int color) {
  if (!mv.is_pseudo_legal) {
    std::invalid_argument("isChecked: Move not pseudo-legal");
  }
  doMove(mv);
  bool checked;
  checked = detectChecked(color);
  undoMove(mv);
  if (!checked) {
    mv.is_legal = true;
    return false;
  }
  return true;
}

bool Board::isDangerousFor(Square *sq, shared_ptr<Piece>& piece) {
  int color = piece->getColor();
  for (auto op : pieces[opponent(color)]) {
    if (op->isDead()) {
      continue;
    } else {
      Move mv = Move(op->getPosition(), sq);
      if (piece->getName() == KING) {
        if (isPseudoLegal(mv)) {
          return true;
        }
      } else {
        Square *piece_pos = piece->getPosition();
        piece_pos->empty();
        bool res = isLegal(mv);
        piece_pos->place(piece);
        if (res) {
          return true;
        }
      }
    }
  }
  return false;
}

vector<Move> Board::listLegalMoves(int color) {
  vector<Move> res;
  int len = pieces[color].size();
  for (int i = 0; i < len; ++i) {
    auto piece = pieces[color].at(i);
    if (piece->isDead()) {
      continue;
    }
    vector<Move> pseudo_legal_moves = piece->listPseudoLegalMoves(*this);
    while (pseudo_legal_moves.size() > 0) {
      Move mv = pseudo_legal_moves.back();
      pseudo_legal_moves.pop_back();
      if (!isChecking(mv, mv.start->getPiece()->getColor())) {
        res.push_back(std::move(mv));
      }
    }
  }
  return res;
}

bool Board::detectChecked(int color) {
  Square* king_pos = kings[color]->getPosition();
  bool res = isDangerousFor(king_pos, kings[color]);
  return res;
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
  }
}

void Board::movePiece(shared_ptr<Piece>& piece, Square* from, Square* to) {
  from->empty();
  to->place(piece);
}

void Board::doMove(Move& mv) {
  if (!mv.is_pseudo_legal) {
    throw std::invalid_argument("doMove: Move's Pseudo-legality not checked");
  }
  // set moving piece
  mv.moving_piece = mv.start->getPiece();
  // castling
  if (mv.is_kingside_castling) {
    int f_file = 'f' - 'a';
    int g_file = 'g' - 'a';
    auto rook = mv.end->getPiece();
    movePiece(mv.moving_piece, mv.start, getSquare(mv.start->getRow(), g_file));
    movePiece(rook, mv.end, getSquare(mv.end->getRow(), f_file));
    // set king and rook's has_moved true
    rook->setHasMoved(true);
  } else if (mv.is_queenside_castling) {
    int c_file = 'c' - 'a';
    int d_file = 'd' - 'a';
    auto rook = mv.end->getPiece();
    movePiece(mv.moving_piece, mv.start, getSquare(mv.start->getRow(), c_file));
    movePiece(rook, mv.end, getSquare(mv.end->getRow(), d_file));
    // set king and rook's has_moved true
    rook->setHasMoved(true);
  // not a castling
  } else {
    // set move variables
    // en passant
    if (mv.is_enpassant) {
        mv.is_attack = true;
        Square *target = getSquare(mv.start->getRow(), mv.end->getCol());
        mv.killed_piece = target->getPiece();
        target->empty();
    // normal attack
    } else if (!mv.end->isEmpty()) {
        mv.is_attack = true;
        mv.killed_piece = mv.end->getPiece();
    }
    // moving the piece
    // promotion
    if (mv.is_promotion) {
      mv.retired_piece = mv.moving_piece;
      mv.start->empty();
      mv.end->empty();
      if (!(KNIGHT <= mv.promote_to && mv.promote_to <= QUEEN)) {
        throw std::invalid_argument("doMove: Cannot promote to this piece");
      }
      addPiece(mv.moving_piece->getColor(), mv.promote_to, mv.end);
      mv.moving_piece = mv.end->getPiece();
    // not promotion
    } else {
      // move the moving piece
      movePiece(mv.moving_piece, mv.start, mv.end);
    }
  }

  // update relative variables
  // if the moving piece hasn't moved set is_first_move true
  if (!mv.moving_piece->getHasMoved()) {
    mv.is_first_move = true;
    mv.moving_piece->setHasMoved(true);
  }
  // keep track of the number of pieces alive
  if (mv.is_attack) {
    num_alive_pieces[mv.killed_piece->getColor()][mv.killed_piece->getName()] -= 1;
  }
  if (mv.is_promotion) {
    num_alive_pieces[mv.retired_piece->getColor()][PAWN] -= 1;
  }
}

void Board::push(Move& mv) {
  if (mv.is_white_resign) {
    resigned[WHITE] = true;
    return;
  } else if (mv.is_black_resign) {
    resigned[BLACK] = true;
    return;
  }
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
  // castling
  if (mv.is_kingside_castling) {
    int f_file = 'f' - 'a';
    int g_file = 'g' - 'a';
    auto rook = getSquare(mv.end->getRow(), f_file)->getPiece();
    movePiece(mv.moving_piece, getSquare(mv.start->getRow(), g_file), mv.start);
    movePiece(rook, getSquare(mv.end->getRow(), f_file), mv.end);
    // set rook's has_moved back to false
    rook->setHasMoved(false);
  } else if (mv.is_queenside_castling) {
    int c_file = 'c' - 'a';
    int d_file = 'd' - 'a';
    auto rook = getSquare(mv.end->getRow(), d_file)->getPiece();
    movePiece(mv.moving_piece, getSquare(mv.start->getRow(), c_file), mv.start);
    movePiece(rook, getSquare(mv.end->getRow(), d_file), mv.end);
    // set rook's has_moved back to false
    rook->setHasMoved(false);
  // not a castling
  } else {
    if (mv.end->isEmpty()) {
      throw std::invalid_argument("undoMove: no piece to move back found");
    }
    // if it was promotion
    if (mv.is_promotion) {
      mv.start->empty();
      removePiece(mv.moving_piece);
      if (mv.retired_piece == nullptr) {
        throw std::logic_error("no retired piece found");
      }
      mv.moving_piece = mv.retired_piece;
      mv.start->place(mv.retired_piece);
    // not a promotion
    } else {
      // move the moved piece backward
      movePiece(mv.moving_piece, mv.end, mv.start);
    }
    // if there was a killed piece revoke it
    if (mv.is_enpassant) {
      Square *target = getSquare(mv.start->getRow(), mv.end->getCol());
      target->place(mv.killed_piece);
    } else if (mv.is_attack) {
      mv.end->place(mv.killed_piece);
    }
  }
  
  // set reletive variables
  // set has_moved back to false if it was first move
  if (mv.is_first_move) {
    mv.moving_piece->setHasMoved(false);
  }
  // keep track of the number of pieces alive
  if (mv.is_attack) {
    num_alive_pieces[mv.killed_piece->getColor()][mv.killed_piece->getName()] += 1;
  }
  // 
  if (mv.is_promotion) {
    num_alive_pieces[mv.retired_piece->getColor()][PAWN] += 1;
  }
}

void Board::pop() {
  // recall the last move
  if (moves_played.size() == 0) {
    std::logic_error("getLastMove: No move to pop found");
  }
  Move& mv = moves_played.back();
  moves_played.pop_back();
  undoMove(mv);
  updateState();
}

int Board::getNumMovesPlayed() {
  return moves_played.size();
}

Move Board::getLastMove() {
  if (moves_played.size() == 0) {
    std::logic_error("getLastMove: No move to pop found");
  }
  return moves_played.back();
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
  throw std::invalid_argument("getSquare(string): Square out of range\n");
}

Square* Board::getSquare(int row, int col) {
  if (row < 0 || ROWS <= row || col < 0 || COLS <= col) {
    throw std::invalid_argument("getSquare(int, int): Square out of range\n");
  }
  return &board[row][col];
}

int Board::getNumAlivePieces(int color, int type) {
  if (color >= NUM_COLORS || type >= NUM_PIECE_TYPES) {
    throw std::invalid_argument("getNumAlivePieces: out of range");
  }
  return num_alive_pieces[color][type];
}

bool Board::inRange(int row, int col) {
  if (row < 0 || ROWS <= row || col < 0 || COLS <= col) {
    return false;
  }
  return true;
}

bool Board::isValidSetup(int mode) {
  bool king_already_found[NUM_COLORS] = {false, false};
  for (int color = WHITE; color < NUM_COLORS; ++color) {
    for (auto pc: pieces[color]) {
      // if king
      if (pc->getName() == KING) {
        if (king_already_found[pc->getColor()]) {
          return false;
        }
        king_already_found[pc->getColor()] = true;

        // rule setup
        kings[color] = pc;
        if (mode == STANDARD) {
          int e_file = 'e' - 'a';
          int rank_1 = '8' - '1'; 
          int rank_8 = '8' - '8';
          Square *pos = pc->getPosition();
          if (pc->getColor() == WHITE) {
            if (!(pos->getCol() == e_file && pos->getRow() == rank_1)) {
              pc->setHasMoved(true);
            }
          } else if (pc->getColor() == BLACK) {
            if (!(pos->getCol() == e_file && pos->getRow() == rank_8)) {
              pc->setHasMoved(true);
            }
          }
        }
      // if pawn
      } else if (pc->getName() == PAWN) {
        int row = pc->getPosition()->getRow();
        if (row == 0 || row == (ROWS - 1)) {
          return false;
        }

        // rule setup
        if (mode == STANDARD) {
          int rank_2 = '8' - '2';
          int rank_7 = '8' - '7';
          if (pc->getColor() == WHITE && pc->getPosition()->getRow() != rank_2) {
            pc->setHasMoved(true);
          } else if (pc->getColor() == BLACK && pc->getPosition()->getRow() != rank_7) {
            pc->setHasMoved(true);
          }
        }
      }
    }
    if (!king_already_found[color]) {
      return false;
    }
    if (detectChecked(color)) {
      return false;
    }
  }
  return true;
}

bool Board::isChecked(int color) { return checked[color]; }

bool Board::isCheckmated(int color) { return checkmated[color]; }

bool Board::isStalemated(int color) { return stalemated[color]; }

bool Board::hasResigned(int color) { return resigned[color]; }

bool Board::isInsufficientMaterial() {
    if (num_alive_pieces[WHITE][PAWN] + num_alive_pieces[WHITE][QUEEN] 
        + num_alive_pieces[WHITE][ROOK] > 0) {
        return false;
    } else if (num_alive_pieces[BLACK][PAWN] + num_alive_pieces[BLACK][QUEEN] 
        + num_alive_pieces[BLACK][ROOK] > 0) {
        return false;
    } else if (num_alive_pieces[WHITE][KNIGHT] + num_alive_pieces[WHITE][BISHOP] > 1) {
        return false;
    } else if (num_alive_pieces[BLACK][KNIGHT] + num_alive_pieces[BLACK][BISHOP] > 1) {
        return false;
    }
    return true;
}

int Board::getRows() { return ROWS; }

int Board::getCols() { return COLS; }
