#include "board.h"
#include "textdisplay.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <utility>

using std::string;
using std::shared_ptr;

shared_ptr<Move> createMove(Board& board, string p1, string p2) {
    Square *start = board.getSquare(p1);
    Square *end = board.getSquare(p2);
    return std::make_shared<Move>(start, end);
}

void playMove(Board& board, string p1, string p2) {
    try {
        shared_ptr<Move> mv = createMove(board, p1, p2);
        board.push(mv);
        board.render();
    } catch (std::invalid_argument ex) {
        std::cout << ex.what() << std::endl;
    }
}

int main() {
    Board board = Board();

    std::vector<Observer*> observers;
    TextDisplay *td = new TextDisplay(&board);
    observers.push_back(td);

    board.render();
    std::string input;


    playMove(board, "d8", "e7");
    playMove(board, "d1", "e2");
    playMove(board, "f8", "f1");

    while (std::cin >> input) {
        if (input == "move") {
            std::string start_pos, end_pos;
            std::cin >> start_pos >> end_pos;
            playMove(board, start_pos, end_pos);
        }
    }
    // playMove(board, "d1", "d8");
    // playMove(board, "d8", "f6");
    // playMove(board, "f6", "e7");
    // playMove(board, "e1", "e2");
    // playMove(board, "e2", "d4");
    // playMove(board, "d4", "d4");

    for (auto o : observers) {
        delete o;
    }
}
 
