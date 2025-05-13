#pragma once
#include <Windows.h>

class CheckerFactory {
public:
    virtual COLORREF CreateColor() = 0;
    virtual ~CheckerFactory() {}
};

class Player1CheckerFactory : public CheckerFactory {
public:
    COLORREF CreateColor() override;
};

class Player2CheckerFactory : public CheckerFactory {
public:
    COLORREF CreateColor() override;
};