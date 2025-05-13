#include "Board.h"
#include <algorithm>

Board::Board(int width, int height, int triangleW)
    : boardWidth(width), boardHeight(height), triangleWidth(triangleW),
    triangleHeight(height / 2) {
}

void Board::DrawBoard(HDC hdc, int offsetX, int offsetY) const {
    COLORREF boardColor = RGB(210, 180, 140);
    HBRUSH hBrush = CreateSolidBrush(boardColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Rectangle(hdc, offsetX, 50 + offsetY, offsetX + boardWidth * 2, 50 + offsetY + boardHeight);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    for (int i = 0; i < 12; i++) {
        DrawPoint(hdc, i, true, offsetX, offsetY);
        DrawPoint(hdc, i, false, offsetX, offsetY);
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);

    DrawPointNumbers(hdc, offsetX, offsetY);
}

void Board::DrawPoint(HDC hdc, int pointIndex, bool isTop, int offsetX, int offsetY) const {
    int x = offsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
    int y = (isTop ? 50 : 50 + triangleHeight) + offsetY;

    POINT triangle[3];
    if (isTop) {
        triangle[0] = { x - triangleWidth, y };
        triangle[1] = { x, y + triangleHeight };
        triangle[2] = { x + triangleWidth, y };
    }
    else {
        triangle[0] = { x - triangleWidth, y + triangleHeight };
        triangle[1] = { x, y };
        triangle[2] = { x + triangleWidth, y + triangleHeight };
    }

    COLORREF darkColor = RGB(139, 69, 19);
    COLORREF lightColor = RGB(245, 222, 179);

    HBRUSH hBrush = CreateSolidBrush(((pointIndex % 2 == 0) ^ isTop) ? darkColor : lightColor);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

    Polygon(hdc, triangle, 3);

    HPEN hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);

    Polygon(hdc, triangle, 3);

    SelectObject(hdc, hOldBrush);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBrush);
    DeleteObject(hPen);
}

POINT Board::GetPointPosition(int pointIndex, int offsetX, int offsetY) const {
    int x, y;

    if (pointIndex < 12) {
        x = offsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
        y = boardHeight - 30 + offsetY;
    }
    else {
        x = offsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
        y = 70 + offsetY;
    }

    return { x, y };
}

POINT Board::GetCheckerPositionOnPoint(int pointIndex, int stackIndex, int offsetX, int offsetY) const {
    POINT basePosition = GetPointPosition(pointIndex, offsetX, offsetY);
    int direction = (pointIndex < 12) ? -1 : 1;
    int spacing = 20;
    return { basePosition.x, basePosition.y + (stackIndex * spacing * direction) };
}

void Board::DrawPointNumbers(HDC hdc, int offsetX, int offsetY) const {
    HFONT hFont = CreateFont(10, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
        OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
        VARIABLE_PITCH, TEXT("Arial"));
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));

    for (int i = 0; i < 12; i++) {
        int upperNumber = 13 + i;
        int upperX = offsetX + (i * triangleWidth * 2) + triangleWidth - 10;
        int lowerNumber = 12 - i;
        int lowerX = offsetX + (i * triangleWidth * 2) + triangleWidth - 10;

        char upperText[3];
        sprintf_s(upperText, "%d", upperNumber);
        TextOutA(hdc, upperX, 50 - 25 + offsetY, upperText, strlen(upperText));

        char lowerText[3];
        sprintf_s(lowerText, "%d", lowerNumber);
        TextOutA(hdc, lowerX, 50 + boardHeight + 10 + offsetY, lowerText, strlen(lowerText));
    }

    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}