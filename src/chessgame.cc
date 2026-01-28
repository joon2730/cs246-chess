#include "chessgame.h"
#include "human.h"
#include "computerlevel1.h"
#include "computerlevel2.h"
#include "computerlevel3.h"
#include "computerlevel4.h"
#include "computerlevel5.h"
#include "textdisplay.h"
#include "graphicdisplay.h"
#include <iostream>
#include <string>

ChessGame::ChessGame() {
    state = INITIAL;
    mode = board.STANDARD;
}

void ChessGame::reset(int white_player_type, int black_player_type) {
    if (!setted) {
        board.init();
        player_in_turn = board.WHITE;
    } else {
        setted = false;
    }
    board.setSideToMove(player_in_turn);
    board.resetRepetition();
    state = ACTIVE;
    players[board.WHITE] = createPlayer(board.WHITE, white_player_type);
    players[board.BLACK] = createPlayer(board.BLACK, black_player_type);
    board.render();
}

void ChessGame::setup() {
    if (!setted) {
        board.init();
    }
    board.render();
    setted = true;
    std::string input;
    while (true) {
        try {
            std::cin >> input;
            if (input == "+") {
                std::string pc, pos;
                std::cin >> pc >> pos;
                if (pc.length() != 1) {
                    throw (std::invalid_argument("Piece type not found"));
                }
                int color, piece_type;
                readPieceFromChar(pc[0], color, piece_type);
                if (!board.getSquare(pos)->isEmpty()) {
                    auto pc = board.getSquare(pos)->getPiece();
                    board.removePiece(pc);
                }
                board.addPiece(color, piece_type, board.getSquare(pos));
            } else if (input == "-") {
                std::string pos;
                std::cin >> pos;
                if (!board.getSquare(pos)->isEmpty()) {
                    auto pc = board.getSquare(pos)->getPiece();
                    board.removePiece(pc);
                }
            } else if (input == "=") {
                std::string color;
                std::cin >> color;
                if (color == "white") {
                    player_in_turn = board.WHITE;
                } else if (color == "black") {
                    player_in_turn = board.BLACK;
                } else {
                    throw (std::invalid_argument("Invalid player"));
                }
            } else if (input == "done") {
                if (board.isValidSetup(mode)) {
                    board.setSideToMove(player_in_turn);
                    board.resetRepetition();
                    if (board.isStalemated(player_in_turn)) {
                        state = STALEMATE;
                    } else if (board.isInsufficientMaterial()) {
                        state = INSUFFICIENT_MATERIAL;
                    }
                    return;
                } else {
                    throw (std::invalid_argument("Invalid setup"));
                }
            }
            board.render();
        } catch (std::invalid_argument& ex) {
            std::cout << ex.what() << std::endl;
        }
    }
}

void ChessGame::takeTurn() {
    if (state != ACTIVE) {
        throw std::logic_error("Game is already done");
    }
    board.setSideToMove(player_in_turn);
    Move mv = players[player_in_turn]->makeMove(board);
    board.push(mv);
    if (board.hasResigned(player_in_turn)) {
        if (player_in_turn == board.WHITE) {
            state = BLACK_WIN;
        } else {
            state = WHITE_WIN;
        }
    }
    if (state == ACTIVE && board.isThreefoldRepetition()) {
        state = THREEFOLD_REPETITION;
        board.render();
        return;
    }
    board.render();
    player_in_turn = player_in_turn == board.WHITE ? board.BLACK : board.WHITE; // white if black, black if white
    if (board.isCheckmated(player_in_turn)) {
        if (player_in_turn == board.WHITE) {
            state = BLACK_WIN;
        } else {
            state = WHITE_WIN;
        }
    } else if (board.isStalemated(player_in_turn)) {
        state = STALEMATE;
    } else if (board.isInsufficientMaterial()) {
        state = INSUFFICIENT_MATERIAL;
    }
}

void ChessGame::addDisplay(int display) {
    unique_ptr<Observer> ob;
    if (display == TEXT_DISPLAY) {
        ob = std::make_unique<TextDisplay>(&board);
    } else if (display == GRAPHIC_DISPLAY) {
        ob = std::make_unique<GraphicDisplay>(&board);
    } else {
        throw std::invalid_argument("no such display");
    }
    observers.push_back(std::move(ob));
}

unique_ptr<Player> ChessGame::createPlayer(int side, int player_type) {
    // if ((side != board.WHITE) && (side != board.BLACK)) {
    //     throw std::invalid_argument("Invalid player side")
    // }
    if (player_type == HUMAN) {
        return std::make_unique<Human>(side);
    } else if (player_type == COMPUTERLEVEL1) {
        return std::make_unique<ComputerLevel1>(side);
    } else if (player_type == COMPUTERLEVEL2) {
        return std::make_unique<ComputerLevel2>(side);
    } else if (player_type == COMPUTERLEVEL3) {
        return std::make_unique<ComputerLevel3>(side);
    } else if (player_type == COMPUTERLEVEL4) {
        return std::make_unique<ComputerLevel4>(side);
    } else if (player_type == COMPUTERLEVEL5) {
        return std::make_unique<ComputerLevel5>(side);
    } else {
        throw std::invalid_argument("Invalid player type");
    }
}

void ChessGame::readPieceFromChar(char c, int& color, int& piece) {
    char name_of_pieces[2][6] 
         = {{'P', 'N', 'B', 'R', 'Q', 'K'},
            {'p', 'n', 'b', 'r', 'q', 'k'}};
    if ('A' <= c && c <= 'Z') {
        color = board.WHITE;
    } else {
        color = board.BLACK;
    }
    for (int index = 0; index < board.NUM_PIECE_TYPES; ++index) {
        if (c == name_of_pieces[color][index]) {
            piece = index;
            return; 
        }
    }
    throw std::invalid_argument("Piece type not found");
}

int ChessGame::getState() {
    return state;
}


