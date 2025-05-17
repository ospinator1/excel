#pragma once
#include <Windows.h>

ref class Board
{
private:
    int boardWidth; // Ширина доски.
    int boardHeight; // Высота доски.
    int triangleWidth; // Ширина треугольника (пирамиды).
    int triangleHeight; // Высота треугольника (пирамиды).
    int boardOffsetX; // Смещение доски по оси X.

public:
    Board(int width, int height, int offsetX); // Конструктор класса Board.
    void DrawBoard(System::Drawing::Graphics^ g); // Рисует игровую доску.
    void DrawPoint(System::Drawing::Graphics^ g, int pointIndex, bool isTop); // Рисует отдельный пункт (пирамиду) на доске.
    System::Drawing::Point GetPointPosition(int pointIndex); // Возвращает позицию пункта (пирамиды) на доске.
    System::Drawing::Point GetCheckerPositionOnPoint(int pointIndex, int stackIndex); // Возвращает позицию шашки на определенном пункте (пирамиде).
    void DrawPointNumbers(System::Drawing::Graphics^ g); // Рисует номера пунктов (пирамид) на доске.

    property int BoardWidth { int get() { return boardWidth; } } // Свойство для получения ширины доски.
    property int BoardHeight { int get() { return boardHeight; } } // Свойство для получения высоты доски.
    property int TriangleWidth { int get() { return triangleWidth; } } // Свойство для получения ширины треугольника.
    property int BoardOffsetX { int get() { return boardOffsetX; } } // Свойство для получения смещения доски по оси X.
};