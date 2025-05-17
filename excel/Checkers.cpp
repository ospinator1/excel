#include "Checker.h"
#include "CheckerFactory.h"

Checkers::Checkers(int radius)
{
    checkerRadius = radius;
    positions = gcnew List<System::Drawing::Point>();
    colors = gcnew List<System::Drawing::Color>();
    points = gcnew List<int>();
}

void Checkers::InitializeCheckers(CheckerFactory^ player1Factory, CheckerFactory^ player2Factory)
{
    positions->Clear();
    colors->Clear();
    points->Clear();

    // Player 1 (dark) checkers
    for (int i = 0; i < 2; i++) AddChecker(Point(0, 0), player1Factory->CreateColor(), 23);
    for (int i = 0; i < 5; i++) AddChecker(Point(0, 0), player1Factory->CreateColor(), 12);
    for (int i = 0; i < 3; i++) AddChecker(Point(0, 0), player1Factory->CreateColor(), 7);
    for (int i = 0; i < 5; i++) AddChecker(Point(0, 0), player1Factory->CreateColor(), 5);

    // Player 2 (light) checkers
    for (int i = 0; i < 2; i++) AddChecker(Point(0, 0), player2Factory->CreateColor(), 0);
    for (int i = 0; i < 5; i++) AddChecker(Point(0, 0), player2Factory->CreateColor(), 11);
    for (int i = 0; i < 3; i++) AddChecker(Point(0, 0), player2Factory->CreateColor(), 16);
    for (int i = 0; i < 5; i++) AddChecker(Point(0, 0), player2Factory->CreateColor(), 18);
}

void Checkers::MoveChecker(int checkerIndex, Point newPosition, int newPoint)
{
    if (checkerIndex >= 0 && checkerIndex < positions->Count)
    {
        positions[checkerIndex] = newPosition;
        points[checkerIndex] = newPoint;
    }
}

int Checkers::GetCheckerCount(int pointIndex, Color color)
{
    int count = 0;
    for (int i = 0; i < points->Count; i++)
    {
        if (points[i] == pointIndex && colors[i] == color)
        {
            count++;
        }
    }
    return count;
}

Point Checkers::GetCheckerPosition(int index)
{
    if (index >= 0 && index < positions->Count)
        return positions[index];
    return Point(0, 0);
}

Color Checkers::GetCheckerColor(int index)
{
    if (index >= 0 && index < colors->Count)
        return colors[index];
    return Color::Black;
}

int Checkers::GetCheckerPoint(int index)
{
    if (index >= 0 && index < points->Count)
        return points[index];
    return -1;
}

void Checkers::AddChecker(Point position, Color color, int point)
{
    positions->Add(position);
    colors->Add(color);
    points->Add(point);
}

void Checkers::RemoveChecker(int index)
{
    if (index >= 0 && index < positions->Count)
    {
        positions->RemoveAt(index);
        colors->RemoveAt(index);
        points->RemoveAt(index);
    }
}