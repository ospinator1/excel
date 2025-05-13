#pragma once
#include <Windows.h>
#include <ctime>
#include <cstdlib>

class Dice {
private:
    int values[2];

public:
    Dice();
    void RollDice();
    const int* GetDiceValues() const;
    void DrawDice(HDC hdc, int x, int y, int value, COLORREF dotColor) const;
};