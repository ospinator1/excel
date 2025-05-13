#include "CheckerFactory.h"

class Checker
{
private:
    POINT position;
    COLORREF color; // Используем COLORREF для цвета
    bool isCaptured; // в баре или снята
    int pointIndex;  // текущий пункт

public:
    // Конструктор с фабрикой для установки цвета
    Checker(POINT pos, const CheckerFactory& factory, int pointIdx);
    // Остальные методы остаются без изменений
    POINT GetPosition() const;
    void SetPosition(POINT pos);
    COLORREF GetColor() const;
    int GetPointIndex() const;
    void SetPointIndex(int idx);
    bool IsCaptured() const;
    void SetCaptured(bool captured);
};