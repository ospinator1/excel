#include "CheckerFactory.h"

class Checker
{
private:
    POINT position;
    COLORREF color; // ���������� COLORREF ��� �����
    bool isCaptured; // � ���� ��� �����
    int pointIndex;  // ������� �����

public:
    
    Checker(POINT pos, const CheckerFactory& factory, int pointIdx);
    
    POINT GetPosition() const;
    void SetPosition(POINT pos);
    COLORREF GetColor() const;
    int GetPointIndex() const;
    void SetPointIndex(int idx);
    bool IsCaptured() const;
    void SetCaptured(bool captured);
};