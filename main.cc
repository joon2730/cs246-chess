#include "board.h"
#include <iostream>
#include <string>

void render(Board &board) {
    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << " ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board.board.at(i).at(j);
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

Move* createMove(Board& board, std::string p1, std::string p2) {
    Square *start = board.getSquare(p1);
    Square *end = board.getSquare(p2);
    return new Move(start, end);
}

int main() {
    std::cout << "main entered\n";
    Board board = Board();
    render(board);

    board.push(createMove(board, "d1", "d8"));
    render(board);

    board.push(createMove(board, "d8", "f6"));
    render(board);

    board.pop();
    render(board);

    board.pop();
    render(board);
} 
 
