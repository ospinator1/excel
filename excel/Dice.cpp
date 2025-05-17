#include "Dice.h"


Dice::Dice()
{
    values = gcnew array<int>(2);
}

void Dice::RollDice()
{
    Random^ random = gcnew Random();
    values[0] = random->Next(1, 7);
    values[1] = random->Next(1, 7);
}

array<int>^ Dice::GetDiceValues()
{
    return values;
}

void Dice::DrawDice(Graphics^ g, int x, int y, int value, Color dotColor)
{
    Color diceColor = Color::FromArgb(255, 255, 255);
    Pen^ borderPen = gcnew Pen(Color::DarkSlateGray, 2);
    SolidBrush^ fillBrush = gcnew SolidBrush(diceColor);
    SolidBrush^ dotBrush = gcnew SolidBrush(dotColor);

    g->FillRectangle(fillBrush, x, y, 60, 60);
    g->DrawRectangle(borderPen, x, y, 60, 60);

    int dotSize = 10;
    int centerX = x + 30;
    int centerY = y + 30;
    int offset = 15;

    switch (value)
    {
    case 1:
        g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        break;
    case 2:
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 3:
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 4:
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 5:
        g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + offset - dotSize / 2, dotSize, dotSize);
        break;
    case 6:
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - offset - dotSize / 2, centerY + 20, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX + offset - dotSize / 2, centerY + 20, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY - 20, dotSize, dotSize);
        g->FillEllipse(dotBrush, centerX - dotSize / 2, centerY + 20, dotSize, dotSize);
        break;
    }
}