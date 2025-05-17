#pragma once
#include <Windows.h>
#include "MyForm.h"

ref class Dice
{
private:
    array<int>^ values;

public:
    Dice(); //  онструктор класса Dice.

    void RollDice(); // √енерирует случайные значени€ дл€ каждого кубика.

    array<int>^ GetDiceValues(); // ¬озвращает массив значений кубиков.

    void DrawDice(System::Drawing::Graphics^ g, int x, int y, int value, System::Drawing::Color dotColor); // –исует один кубик на графическом контексте.
    // g - контекст рисовани€, x, y - координаты верхнего левого угла кубика,
    // value - значение кубика (1-6), dotColor - цвет точек.
};