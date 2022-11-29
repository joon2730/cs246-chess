#include "chessgame.h"
#include "human.h"
#include "textdisplay.h"

ChessGame::ChessGame(unique_ptr<Player>& white, unique_ptr<Player>& black) {
    state = ACTIVE;
    player_in_turn = board.WHITE;
    players[0] = std::move(white);
    players[1] = std::move(black);
}

void ChessGame::begin() {
    board.init();
    board.render();
}

void ChessGame::setup() {

}

void ChessGame::takeTurn() {
    // if (state != ACTIVE) {
    //     throw std::logic_error("Game is already done")
    // }
    Move mv = players[player_in_turn]->makeMove(board);
    board.push(mv);
    board.render();
    player_in_turn = (player_in_turn + 1) % 2; // white if black, black if white
    if (board.isCheckmated(player_in_turn)) {
        if (player_in_turn == board.WHITE) {
            state = BLACK_WIN;
        } else {
            state = WHITE_WIN;
        }
    } else if (board.isStalemated(player_in_turn)) {
        state = STALEMATE;
    }
}

void ChessGame::resign(int player) {

}

void ChessGame::addDisplay(int display) {
    unique_ptr<Observer> ob;
    if (display == TEXT_DISPLAY) {
        ob = std::make_unique<TextDisplay>(&board);
    } else {
        throw std::invalid_argument("no such display");
    }
    observers.push_back(std::move(ob));
}

int ChessGame::getState() {
    return state;
}


