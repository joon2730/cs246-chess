#include "computerlevel1.h"
#include <cstdlib>
#include <ctime>


ComputerLevel1::ComputerLevel1(int side): Computer{side} {
    std::srand(std::time(0));
}

Move ComputerLevel1::makeMove(Board &board) {
    std::string input;
    while (!auto_move) {
        std::cin >> input;
        if (input == "move") {
            break;
        } else if (input == "auto") {
            auto_move = true;
        }
    }
    auto legal_moves = board.listLegalMoves(side);
    int len = legal_moves.size();
    int randint = std::rand();
    return legal_moves.at(randint % len);
}

