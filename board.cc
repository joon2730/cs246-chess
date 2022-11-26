#include "board.h"
#include "queen.h"
#include "king.h"
#include <stdexcept>
#include <iostream>

Board::Board() {
    std::cout << "Board() entered\n";
    for(int i = 0; i < ROWS; ++i) {
        vector<Square> row;
		board.emplace_back(row);
		for (int j = 0; j < COLS; ++j) {
			board.at(i).push_back(Square(i, j));
		}
    }
    init();
    std::cout << "Board() exited\n";
}

Board::~Board() {
    empty();
}

void Board::empty() {
    std::cout << "empty entered\n";
    for (int i = 0; i < NUM_COLORS; ++i) {
        while (pieces[i].size() > 0) {
            Piece *tmp = pieces[i].back();
            pieces[i].pop_back();
            delete tmp;
        }
    }
    while (moves_played.size() > 0) {
        Move *tmp = moves_played.back();
        moves_played.pop_back();
        delete tmp;
    }
    std::cout << "empty exited\n";
}

int Board::opponent(int color) {
    if (color == 0) {
        return 1;
    } else {
        return 0;
    }
}

void Board::addPiece(int piece, int color, string coord) {
    Piece *new_piece;
    // if (piece == PAWN) {

    // } else if (piece == KNIGHT){

    // } else if (piece == BISHOP){

    // } else if (piece == ROOK){

    // } else
    if (piece == QUEEN){
        new_piece = new Queen(color, QUEEN);
    } else if (piece == KING){
        new_piece = new King(color, KING);
        kings[color] = new_piece;
    } else {
        throw std::invalid_argument("invalid type of piece");
    }
    pieces[color].push_back(new_piece);
    Square *sq = getSquare(coord);
    sq->place(new_piece);
}

void Board::removePiece(string coord) {

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

}

bool Board::isLegal(Move *mv) {
    applyMove(mv);
    bool checked = detectChecked(mv->getMovingPiece()->getColor());
    pop();
    if (checked) {
        return false;
    }
    return true;
}

bool Board::detectChecked(int color) {
    // bool checked = false;
    // for (auto piece : pieces[opponent(color)]) {
        
    // }
    return false;
}

void Board::updateState() {
    for (int color = WHITE; color < NUM_COLORS; ++color) {
        checked[color] = false;
        checkmated[color] = false;
        stalemated[color] = false;
        // checked = false;
        // for (auto pieces : pieces[opponent(cl)]) {
	    //     if ()
        // }
    }
}

void Board::applyMove(Move *mv) {
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    // Start square is not empty
    if (!start->isEmpty()) {
        Piece *moving_piece = start->getPiece();
        // Move is valie
        if (moving_piece->canMove(mv)) {
            mv->setMovingPiece(moving_piece);
            // if there is a piece being attacked
            if (!end->isEmpty()) {
                mv->setIsAttack();
                Piece *killed_piece = end->getPiece();
                mv->setKilledPiece(killed_piece);
            }
            // move the moving piece
            start->empty();
            end->place(moving_piece);
            // record the move
            moves_played.push_back(mv);
            return;
        } else {
            delete mv;
            throw std::invalid_argument("move not possible");
        }
    } else {
        delete mv;
        throw std::invalid_argument("piece not found");
    }
}

void Board::push(Move *mv) {
    applyMove(mv);
    updateState();
    if (isChecked(mv->getMovingPiece()->getColor())) {
        pop();
        throw std::invalid_argument("illegal move");
    }
}

void Board::pop() {
    // recall the last move
    Move *mv = moves_played.back();
    moves_played.pop_back();
    Square *start = mv->getStartSquare();
    Square *end = mv->getEndSquare();
    Piece *moving_piece = mv->getMovingPiece();
    // move the moved piece backward
    start->place(moving_piece);
    // if there was a killed piece revoke it
    if (mv->isAttack()) {
        Piece *killed_piece = mv->getKilledPiece();
        end->place(killed_piece);
    // else empty the end square
    } else {
        end->empty();
    }
    updateState();
    delete mv;
}

Square* Board::getSquare(string coord) {
    bool is_length_two = (coord.size() == 2);
    if (is_length_two) {
        char file = coord[0];
        char rank = coord[1];
        bool is_file_valid = ('a' <= file) && (file <= 'h');
        bool is_rank_valid = ('1' <= rank) && (rank <= '8');
        if (is_file_valid && is_rank_valid) {
            int row = ROWS - (rank - '1') - 1;
            int col = file - 'a';
            return &board[row][col];
        }
    }
    throw std::invalid_argument("invalid coordinate");
}

Square* Board::getSquare(int row, int col) {
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
