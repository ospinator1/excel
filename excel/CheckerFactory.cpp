#include "CheckerFactory.h"

COLORREF Player1CheckerFactory::CreateColor() {
    return RGB(50, 50, 50); // Dark color for Player 1
}

COLORREF Player2CheckerFactory::CreateColor() {
    return RGB(240, 240, 240); // Light color for Player 2
}