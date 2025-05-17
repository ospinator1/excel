#pragma once
#include <Windows.h>
#include "GUI.h"
ref class Bar
{
private:
    List<int>^ player1Checkers; // —писок индексов шашек игрока 1, наход€щихс€ на баре.
    List<int>^ player2Checkers; // —писок индексов шашек игрока 2, наход€щихс€ на баре.
    System::Drawing::Point player1Position; // ѕозици€ бара дл€ игрока 1.
    System::Drawing::Point player2Position; // ѕозици€ бара дл€ игрока 2.

public:
    Bar(System::Drawing::Point p1Pos, System::Drawing::Point p2Pos); //  онструктор: инициализирует позиции бара дл€ игроков.
    void AddToBar(int checkerIndex, bool isPlayer1); // ƒобавл€ет индекс шашки на бар, указыва€, какому игроку она принадлежит.
    void RemoveFromBar(int checkerIndex, bool isPlayer1); // ”дал€ет индекс шашки с бара, указыва€, какому игроку она принадлежала.
    int GetBarCount(bool isPlayer1); // ¬озвращает количество шашек на баре дл€ указанного игрока.
    bool Contains(int checkerIndex, bool isPlayer1); // ѕровер€ет, содержит ли бар указанную шашку дл€ указанного игрока.
    void Clear(); // ќчищает бар, удал€€ все шашки обоих игроков.

    property List<int>^ Player1Checkers { List<int>^ get() { return player1Checkers; } } // —войство: возвращает список индексов шашек игрока 1 на баре.
    property List<int>^ Player2Checkers { List<int>^ get() { return player2Checkers; } } // —войство: возвращает список индексов шашек игрока 2 на баре.
    property System::Drawing::Point Player1Position { System::Drawing::Point get() { return player1Position; } } // —войство: возвращает позицию бара дл€ игрока 1.
    property System::Drawing::Point Player2Position { System::Drawing::Point get() { return player2Position; } } // —войство: возвращает позицию бара дл€ игрока 2.
};