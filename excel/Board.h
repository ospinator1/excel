#pragma once
#include <Windows.h>
#include <vector>

class Board {
private:
    int boardWidth;
    int boardHeight;
    int triangleWidth;
    int triangleHeight;

public:
    Board(int width, int height, int triangleW);
    void DrawBoard(HDC hdc, int offsetX, int offsetY) const;
    void DrawPoint(HDC hdc, int pointIndex, bool isTop, int offsetX, int offsetY) const;
    POINT GetPointPosition(int pointIndex, int offsetX, int offsetY) const;
    POINT GetCheckerPositionOnPoint(int pointIndex, int stackIndex, int offsetX, int offsetY) const;
    void DrawPointNumbers(HDC hdc, int offsetX, int offsetY) const;
};