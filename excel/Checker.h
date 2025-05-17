#pragma once
#include <Windows.h>
#include "MyForm.h"


ref class Checkers
{
private:
    List<System::Drawing::Point>^ positions; // Список координат шашек.
    List<System::Drawing::Color>^ colors;   // Список цветов шашек.
    List<int>^ points;   // Список очков, связанных с каждой шашкой.
    int checkerRadius;    // Радиус шашек.

public:
    Checkers(int radius); // Конструктор: Инициализирует радиус шашек.
    void InitializeCheckers(CheckerFactory^ player1Factory, CheckerFactory^ player2Factory); // Инициализирует шашки для обоих игроков.
    void MoveChecker(int checkerIndex, System::Drawing::Point newPosition, int newPoint); // Перемещает шашку на новую позицию и обновляет очки.
    int GetCheckerCount(int pointIndex, System::Drawing::Color color); // Возвращает количество шашек определенного цвета в заданной точке.
    System::Drawing::Point GetCheckerPosition(int index); // Возвращает позицию шашки по индексу.
    System::Drawing::Color GetCheckerColor(int index); // Возвращает цвет шашки по индексу.
    int GetCheckerPoint(int index); // Возвращает очки, связанные с шашкой по индексу.
    void AddChecker(System::Drawing::Point position, System::Drawing::Color color, int point); // Добавляет новую шашку с заданной позицией, цветом и очками.
    void RemoveChecker(int index); // Удаляет шашку по индексу.

    property List<System::Drawing::Point>^ Positions { List<System::Drawing::Point>^ get() { return positions; } } // Свойство: Возвращает список координат шашек.
    property List<System::Drawing::Color>^ Colors { List<System::Drawing::Color>^ get() { return colors; } } // Свойство: Возвращает список цветов шашек.
    property List<int>^ Points { List<int>^ get() { return points; } } // Свойство: Возвращает список очков, связанных с шашками.
    property int CheckerRadius { int get() { return checkerRadius; } } // Свойство: Возвращает радиус шашек.
};