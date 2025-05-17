#include "Board.h"

Board::Board(int width, int height, int offsetX)
{
    boardWidth = width;
    boardHeight = height;
    triangleWidth = boardWidth / 12;
    triangleHeight = boardHeight / 2;
    boardOffsetX = offsetX;
}

void Board::DrawBoard(Graphics^ g)
{
    Color boardColor = Color::FromArgb(210, 180, 140);
    g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50 + 100, boardWidth * 2, boardHeight);
    g->DrawRectangle(Pens::Black, boardOffsetX, 50 + 100, boardWidth * 2, boardHeight);

    for (int i = 0; i < 12; i++)
    {
        DrawPoint(g, i, true);
        DrawPoint(g, i, false);
    }

    DrawPointNumbers(g);
}

void Board::DrawPoint(Graphics^ g, int pointIndex, bool isTop)
{
    int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
    int y = (isTop ? 50 : 50 + triangleHeight) + 100;

    array<Point>^ triangle = gcnew array<Point>(3);
    if (isTop)
    {
        triangle[0] = Point(x - triangleWidth, y);
        triangle[1] = Point(x, y + triangleHeight);
        triangle[2] = Point(x + triangleWidth, y);
    }
    else
    {
        triangle[0] = Point(x - triangleWidth, y + triangleHeight);
        triangle[1] = Point(x, y);
        triangle[2] = Point(x + triangleWidth, y + triangleHeight);
    }

    Color darkColor = Color::FromArgb(139, 69, 19);
    Color lightColor = Color::FromArgb(245, 222, 179);

    Brush^ pointBrush = ((pointIndex % 2 == 0) ^ isTop) ? gcnew SolidBrush(darkColor) : gcnew SolidBrush(lightColor);
    g->FillPolygon(pointBrush, triangle);
    g->DrawPolygon(Pens::Black, triangle);
}

Point Board::GetPointPosition(int pointIndex)
{
    int x, y;

    if (pointIndex < 12)
    {
        x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
        y = boardHeight - 30 + 150;
    }
    else
    {
        x = boardOffsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
        y = 70 + 150;
    }

    return Point(x, y);
}

Point Board::GetCheckerPositionOnPoint(int pointIndex, int stackIndex)
{
    Point basePosition = GetPointPosition(pointIndex);
    int direction = (pointIndex < 12) ? -1 : 1;
    int spacing = 20;
    return Point(basePosition.X, basePosition.Y + (stackIndex * spacing * direction));
}

void Board::DrawPointNumbers(Graphics^ g)
{
    Drawing::Font^ font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
    Brush^ brush = Brushes::Black;

    for (int i = 0; i < 12; i++)
    {
        int upperNumber = 13 + i;
        int upperX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
        g->DrawString(upperNumber.ToString(), font, brush, PointF(upperX, 50 - 25 + 150));

        int lowerNumber = 12 - i;
        int lowerX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
        g->DrawString(lowerNumber.ToString(), font, brush, PointF(lowerX, 50 + boardHeight + 10 + 150));
    }
}