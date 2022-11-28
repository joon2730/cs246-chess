#include "board.h"
#include "queen.h"
#include "king.h"
#include "move.h"
#include "piece.h"
#include <stdexcept>
#include <iostream>

Board::Board() {
    for(int i = 0; i < ROWS; ++i) {
        vector<Square> row;
		board.emplace_back(row);
		for (int j = 0; j < COLS; ++j) {
			board.at(i).push_back(Square(i, j));
	    }
    }
}

Board::~Board() {
    empty();
}

void Board::render() {
    notifyObservers();
}

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
    // if (piece == PAWN) {

    // } else if (piece == KNIGHT){

    // } else if (piece == BISHOP){

    // } else if (piece == ROOK){

    // } else
    if (piece == QUEEN){
        new_piece = std::make_shared<Queen>(color, QUEEN);
    } else if (piece == KING){
        new_piece = std::make_shared<King>(color, KING);
        kings[color] = new_piece;
    } else {
        throw std::invalid_argument("invalid type of piece");
    }
    Square *sq = getSquare(pos);
    sq->place(new_piece);
    pieces[color].push_back(std::move(new_piece));
}

void Board::removePiece(string pos) {

}

void Board::init() {
    empty();
    // addPiece(Pawn, White, "a2");
    // addPiece(Pawn, White, "b2");
    // addPiece(Pawn, White, "c2");
    // addPiece(Pawn, White, "d2");
    // addPiece(Pawn, White, "e2");
    // addPiece(Pawn, White, "f2");
    // addPiece(Pawn, White, "g2");
    addPiece(QUEEN, WHITE, "d1");
    addPiece(QUEEN, BLACK, "d8");
    addPiece(KING, WHITE, "e1");
    addPiece(KING, BLACK, "e8");

    addPiece(QUEEN, WHITE, "c1");
    addPiece(QUEEN, BLACK, "c8");
    addPiece(QUEEN, WHITE, "f1");
    addPiece(QUEEN, BLACK, "f8");
    updateState();

}

bool Board::isPossible(shared_ptr<Move>& mv) {
    Square *start = mv->getStartSquare();
    if (start->isEmpty()) {
        throw std::invalid_argument("Board::isPossible: No piece to move found");
    }
    // piece must be able to reach the square follwing the rule
    shared_ptr<Piece> moving_piece = start->getPiece();
    if (moving_piece->canMove(*this, mv)) {
        return true;
    }
    return false;
}

bool Board::isLegal(shared_ptr<Move>& mv) {
    if (isPossible(mv) && !isPuttingOwnKingInCheck(mv)) {
        return true;
    }
    return false;
}

bool Board::isPuttingOwnKingInCheck(shared_ptr<Move>& mv) {
    doMove(mv);
    bool checked = detectChecked(mv->getMovingPiece()->getColor());
    undoMove(mv);
    if (checked) {
        return true;
    }
    return false;
}

vector<shared_ptr<Move>> Board::listLegalMoves(int color) {
    // std::cout << "list legal move entered\n";
    vector<shared_ptr<Move>> res;
    for (auto piece : pieces[color]) {
        // cannot move if piece is dead
        if (piece->isDead()) {
            continue;
        }
        // get all possible moves for the piece
        vector<shared_ptr<Move>> possible_moves = piece->listPossibleMoves(*this);
        while (possible_moves.size() > 0) {
            // std::cout << "loop left" << possible_moves.size() << "\n";
            shared_ptr<Move> mv = possible_moves.back();
            possible_moves.pop_back();
            if (!isPuttingOwnKingInCheck(mv)) {
                res.push_back(std::move(mv));
            }
        }
    }
    // std::cout << "list legal move ended\n";
    return res;
}

bool Board::detectChecked(int color) {
    Square* king_pos = kings[color]->getPosition();
    for (auto op : pieces[opponent(color)]) {
        if (op->isDead()) {
            continue;
        } else {
            shared_ptr<Move> mv = std::make_shared<Move>(op->getPosition(), king_pos);
            if (op->canMove(*this, mv)) {
                return true;
            }
        }
    }
    return false;
}

void Board::updateState() {
    for (int color = WHITE; color < NUM_COLORS; ++color) {
        checked[color] = detectChecked(color);
        vector<shared_ptr<Move>> legal_moves = listLegalMoves(color);
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
        // checked = false;
        // for (auto pieces : pieces[opponent(cl)]) {
	    //     if ()
        // }
    }
}

void Board::movePiece(shared_ptr<Piece>& piece, Square *from, Square *to) {
    from->empty();
    to->place(piece);
}

void Board::doMove(shared_ptr<Move>& mv) {
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    if (start->isEmpty()) {
        throw std::invalid_argument("Board::doMove: No piece to move found");
    } 
    shared_ptr<Piece> moving_piece = start->getPiece();
    // std::cout << "doMove from " << *start << " to " << *end << " entered\n";
    mv->setMovingPiece(moving_piece);
    // if there is a piece being attacked
    if (!end->isEmpty()) {
        mv->setIsAttack();
        shared_ptr<Piece> killed_piece = end->getPiece();
        mv->setKilledPiece(killed_piece);
    }
    // move the moving piece
    movePiece(moving_piece, start, end);
    // std::cout << "doMove ended\n";
}

void Board::push(shared_ptr<Move>& mv) {
    // Square *start = mv->getStartSquare();
    // //Square *end = mv->getEndSquare();
    // // Start square is not empty
    // if (start->isEmpty()) {
    //     throw std::invalid_argument("No moving piece found");
    // }
    // shared_ptr<Piece> moving_piece = start->getPiece();
    // piece must be able to reach the square according to rule
    if (!isPossible(mv)) {
        throw std::invalid_argument("Board::push: move not possible");
    }
    // make movement
    doMove(mv);
    // move must not put the same color's king in check
    if (detectChecked(mv->getMovingPiece()->getColor())) {
        undoMove(mv);
        throw std::invalid_argument("Board::push: move puts the same color's king in check");
    }
    // update state
    updateState();
    // record the move
    moves_played.push_back(std::move(mv));
}

void Board::undoMove(shared_ptr<Move>& mv) {
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    shared_ptr<Piece> moving_piece = mv->getMovingPiece();
    // move the moved piece backward
    movePiece(moving_piece, end, start);
    // if there was a killed piece revoke it
    if (mv->isAttack()) {
        shared_ptr<Piece> killed_piece = mv->getKilledPiece();
        end->place(killed_piece);
    }  
}

void Board::pop() {
    // recall the last move
    shared_ptr<Move> mv = moves_played.back();
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
    throw std::invalid_argument("Square out of range");
}

Square* Board::getSquare(int row, int col) {
    if (row < 0 || ROWS <= row || col < 0 || COLS <= col) {
        throw std::invalid_argument("Square out of range");
    }
    return &board[row][col];
}

bool Board::isChecked(int color) {
    return checked[color];
}

bool Board::isCheckmated(int color) {
    return checkmated[color];
}

bool Board::isStalemated(int color) {
    return stalemated[color];
}

int Board::getRows() {
    return ROWS;
}

int Board::getCols() {
    return COLS;
}
