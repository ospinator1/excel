#include "Dice.h"

Dice::Dice() {
    srand(static_cast<unsigned>(time(nullptr)));
    values[0] = values[1] = 1;
}

void Dice::RollDice() {
    values[0] = rand() % 6 + 1;
    values[1] = rand() % 6 + 1;
}

const int* Dice::GetDiceValues() const {
    return values;
}

void Dice::DrawDice(HDC hdc, int x, int y, int value, COLORREF dotColor) const {
    COLORREF diceColor = RGB(255, 255, 255);
    HBRUSH hBrush = CreateSolidBrush(diceColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 2, RGB(47, 79, 79));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    Rectangle(hdc, x, y, x + 60, y + 60);

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

    HBRUSH hDotBrush = CreateSolidBrush(dotColor);
    HBRUSH hOldDotBrush = (HBRUSH)SelectObject(hdc, hDotBrush);

    int dotSize = 10;
    int centerX = x + 30;
    int centerY = y + 30;
    int offset = 15;

    switch (value) {
    case 1:
        g->FillEllipse(hdc, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        break;
    case 2:
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 3:
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 4:
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipsehdc, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 5:
        g->FillEllipse(hdc, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipsehdc, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 6:
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - offset - dotSize / 2, centerY + 20, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(hdc, centerX + offset - dotSize / 2, centerY + 20, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(hdc, centerX - dotSize / 2, centerY + 20, dotSize, dotSize);
        break;
    }

    SelectObject(hdc, hOldDotBrush);
    DeleteObject(hDotBrush);
}