#pragma once
#include <string>

ref class GameRules
{
private:
    static GameRules^ instance; // ����������� ��������� ������ GameRules (Singleton).
    System::String^ rulesText; // �����, ���������� ������� ����.

    GameRules(); // ��������� ����������� (��� ���������� Singleton).

public:
    static GameRules^ GetInstance(); // ���������� ������������ ��������� ������ GameRules.
    System::String^ GetRules(); // ���������� ����� ������ ����.
};