#include "math.h"

int abs(int x) {
    if (x >= 0) {
        return x;
    } else {
        return -1 * x;
    }
}

bool isStraight(int x1, int y1, int x2, int y2) {
    if (x1 == x2 || y1 == y2) {
        return true;
    }
    return false; 
}

bool isDiagonal(int x1, int y1, int x2, int y2) {
    int xd = dist(x1, x2);
    int yd = dist(y1, y2);
    if (xd == yd) {
        return true;
    }
    return false; 
}

int dist(int x1, int x2) {
    return abs(x2 - x1);
}

int sign(int x) {
    if (x == 0) {
        return 0;
    }
    return x / abs(x);
}
