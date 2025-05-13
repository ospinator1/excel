#pragma once

using namespace System;

public ref class GameRules
{
private:
    static GameRules^ instance;
    String^ rulesText;

    GameRules();

public:
    static GameRules^ GetInstance();

    String^ GetRules();
};