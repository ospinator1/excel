#pragma once
#include <Windows.h>
#include "MyForm.h"


ref class Checkers
{
private:
    List<System::Drawing::Point>^ positions; // ������ ��������� �����.
    List<System::Drawing::Color>^ colors;   // ������ ������ �����.
    List<int>^ points;   // ������ �����, ��������� � ������ ������.
    int checkerRadius;    // ������ �����.

public:
    Checkers(int radius); // �����������: �������������� ������ �����.
    void InitializeCheckers(CheckerFactory^ player1Factory, CheckerFactory^ player2Factory); // �������������� ����� ��� ����� �������.
    void MoveChecker(int checkerIndex, System::Drawing::Point newPosition, int newPoint); // ���������� ����� �� ����� ������� � ��������� ����.
    int GetCheckerCount(int pointIndex, System::Drawing::Color color); // ���������� ���������� ����� ������������� ����� � �������� �����.
    System::Drawing::Point GetCheckerPosition(int index); // ���������� ������� ����� �� �������.
    System::Drawing::Color GetCheckerColor(int index); // ���������� ���� ����� �� �������.
    int GetCheckerPoint(int index); // ���������� ����, ��������� � ������ �� �������.
    void AddChecker(System::Drawing::Point position, System::Drawing::Color color, int point); // ��������� ����� ����� � �������� ��������, ������ � ������.
    void RemoveChecker(int index); // ������� ����� �� �������.

    property List<System::Drawing::Point>^ Positions { List<System::Drawing::Point>^ get() { return positions; } } // ��������: ���������� ������ ��������� �����.
    property List<System::Drawing::Color>^ Colors { List<System::Drawing::Color>^ get() { return colors; } } // ��������: ���������� ������ ������ �����.
    property List<int>^ Points { List<int>^ get() { return points; } } // ��������: ���������� ������ �����, ��������� � �������.
    property int CheckerRadius { int get() { return checkerRadius; } } // ��������: ���������� ������ �����.
};