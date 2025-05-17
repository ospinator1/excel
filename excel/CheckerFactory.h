#pragma once
#include <Windows.h>


ref class CheckerFactory abstract
{
public:
    // Создает цвет для шашки. Абстрактный метод, должен быть реализован в производных классах.
    virtual System::Drawing::Color CreateColor() abstract;
};

ref class Player1CheckerFactory : public CheckerFactory
{
public:
    // Создает цвет для шашки первого игрока.
    virtual System::Drawing::Color CreateColor() override;
};

ref class Player2CheckerFactory : public CheckerFactory
{
public:
    // Создает цвет для шашки второго игрока.
    virtual System::Drawing::Color CreateColor() override;
};