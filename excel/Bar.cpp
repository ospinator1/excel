#include "Bar.h"

Bar::Bar(Point p1Pos, Point p2Pos)
{
    player1Checkers = gcnew List<int>();
    player2Checkers = gcnew List<int>();
    player1Position = p1Pos;
    player2Position = p2Pos;
}

void Bar::AddToBar(int checkerIndex, bool isPlayer1)
{
    if (isPlayer1)
        player1Checkers->Add(checkerIndex);
    else
        player2Checkers->Add(checkerIndex);
}

void Bar::RemoveFromBar(int checkerIndex, bool isPlayer1)
{
    if (isPlayer1)
        player1Checkers->Remove(checkerIndex);
    else
        player2Checkers->Remove(checkerIndex);
}

int Bar::GetBarCount(bool isPlayer1)
{
    return isPlayer1 ? player1Checkers->Count : player2Checkers->Count;
}

bool Bar::Contains(int checkerIndex, bool isPlayer1)
{
    return isPlayer1 ? player1Checkers->Contains(checkerIndex) : player2Checkers->Contains(checkerIndex);
}

void Bar::Clear()
{
    player1Checkers->Clear();
    player2Checkers->Clear();
}