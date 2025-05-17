#pragma once
#include <Windows.h>
#include "MyForm.h"

ref class Dice
{
private:
    array<int>^ values;

public:
    Dice(); // ����������� ������ Dice.

    void RollDice(); // ���������� ��������� �������� ��� ������� ������.

    array<int>^ GetDiceValues(); // ���������� ������ �������� �������.

    void DrawDice(System::Drawing::Graphics^ g, int x, int y, int value, System::Drawing::Color dotColor); // ������ ���� ����� �� ����������� ���������.
    // g - �������� ���������, x, y - ���������� �������� ������ ���� ������,
    // value - �������� ������ (1-6), dotColor - ���� �����.
};