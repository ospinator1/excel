#pragma once
#include <Windows.h>

ref class Board
{
private:
    int boardWidth; // ������ �����.
    int boardHeight; // ������ �����.
    int triangleWidth; // ������ ������������ (��������).
    int triangleHeight; // ������ ������������ (��������).
    int boardOffsetX; // �������� ����� �� ��� X.

public:
    Board(int width, int height, int offsetX); // ����������� ������ Board.
    void DrawBoard(System::Drawing::Graphics^ g); // ������ ������� �����.
    void DrawPoint(System::Drawing::Graphics^ g, int pointIndex, bool isTop); // ������ ��������� ����� (��������) �� �����.
    System::Drawing::Point GetPointPosition(int pointIndex); // ���������� ������� ������ (��������) �� �����.
    System::Drawing::Point GetCheckerPositionOnPoint(int pointIndex, int stackIndex); // ���������� ������� ����� �� ������������ ������ (��������).
    void DrawPointNumbers(System::Drawing::Graphics^ g); // ������ ������ ������� (�������) �� �����.

    property int BoardWidth { int get() { return boardWidth; } } // �������� ��� ��������� ������ �����.
    property int BoardHeight { int get() { return boardHeight; } } // �������� ��� ��������� ������ �����.
    property int TriangleWidth { int get() { return triangleWidth; } } // �������� ��� ��������� ������ ������������.
    property int BoardOffsetX { int get() { return boardOffsetX; } } // �������� ��� ��������� �������� ����� �� ��� X.
};