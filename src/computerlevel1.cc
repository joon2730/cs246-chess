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
    const bool was_auto = auto_move;
    auto legal_moves = board.listLegalMoves(side);
    int len = legal_moves.size();
    int randint = std::rand();
    Move mv = legal_moves.at(randint % len);
    if (was_auto) {
        auto toCoord = [](Square *sq) {
            char file = static_cast<char>('a' + sq->getCol());
            char rank = static_cast<char>('8' - sq->getRow());
            std::string s;
            s += file;
            s += rank;
            return s;
        };
        std::cout << "computer moved: " << toCoord(mv.start) << " " << toCoord(mv.end) << std::endl;
    }
    return mv;
}

