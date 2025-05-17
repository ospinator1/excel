#pragma once
#include <Windows.h>


ref class CheckerFactory abstract
{
public:
    // ������� ���� ��� �����. ����������� �����, ������ ���� ���������� � ����������� �������.
    virtual System::Drawing::Color CreateColor() abstract;
};

ref class Player1CheckerFactory : public CheckerFactory
{
public:
    // ������� ���� ��� ����� ������� ������.
    virtual System::Drawing::Color CreateColor() override;
};

ref class Player2CheckerFactory : public CheckerFactory
{
public:
    // ������� ���� ��� ����� ������� ������.
    virtual System::Drawing::Color CreateColor() override;
};