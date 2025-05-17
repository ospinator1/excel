#pragma once
#include <string>

ref class GameRules
{
private:
    static GameRules^ instance; // Статический экземпляр класса GameRules (Singleton).
    System::String^ rulesText; // Текст, содержащий правила игры.

    GameRules(); // Приватный конструктор (для реализации Singleton).

public:
    static GameRules^ GetInstance(); // Возвращает единственный экземпляр класса GameRules.
    System::String^ GetRules(); // Возвращает текст правил игры.
};