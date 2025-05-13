#pragma once
#include <vector>
#include <Windows.h>

class Bar {
private:
    std::vector<int> player1Checkers;
    std::vector<int> player2Checkers;
    POINT positionP1;
    POINT positionP2;

public:
    Bar();
    void AddToBar(int checkerIndex, int player);
    void RemoveFromBar(int checkerIndex, int player);
    int GetBarCount(int player) const;
    bool Contains(int checkerIndex, int player) const;
    POINT GetPosition(int player) const;
    void SetPosition(int player, POINT position);
};