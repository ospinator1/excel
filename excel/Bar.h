#pragma once
#include <Windows.h>
#include "GUI.h"
ref class Bar
{
private:
    List<int>^ player1Checkers; // ������ �������� ����� ������ 1, ����������� �� ����.
    List<int>^ player2Checkers; // ������ �������� ����� ������ 2, ����������� �� ����.
    System::Drawing::Point player1Position; // ������� ���� ��� ������ 1.
    System::Drawing::Point player2Position; // ������� ���� ��� ������ 2.

public:
    Bar(System::Drawing::Point p1Pos, System::Drawing::Point p2Pos); // �����������: �������������� ������� ���� ��� �������.
    void AddToBar(int checkerIndex, bool isPlayer1); // ��������� ������ ����� �� ���, ��������, ������ ������ ��� �����������.
    void RemoveFromBar(int checkerIndex, bool isPlayer1); // ������� ������ ����� � ����, ��������, ������ ������ ��� ������������.
    int GetBarCount(bool isPlayer1); // ���������� ���������� ����� �� ���� ��� ���������� ������.
    bool Contains(int checkerIndex, bool isPlayer1); // ���������, �������� �� ��� ��������� ����� ��� ���������� ������.
    void Clear(); // ������� ���, ������ ��� ����� ����� �������.

    property List<int>^ Player1Checkers { List<int>^ get() { return player1Checkers; } } // ��������: ���������� ������ �������� ����� ������ 1 �� ����.
    property List<int>^ Player2Checkers { List<int>^ get() { return player2Checkers; } } // ��������: ���������� ������ �������� ����� ������ 2 �� ����.
    property System::Drawing::Point Player1Position { System::Drawing::Point get() { return player1Position; } } // ��������: ���������� ������� ���� ��� ������ 1.
    property System::Drawing::Point Player2Position { System::Drawing::Point get() { return player2Position; } } // ��������: ���������� ������� ���� ��� ������ 2.
};