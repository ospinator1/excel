#pragma once
#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;

ref class Timer
{
private:
    int totalSeconds;
    int turnSeconds;
    bool isActive;
    System::Windows::Forms::Timer^ turnTimer; // явно указано System::Windows::Forms
    Label^ player1TimerLabel;
    Label^ player2TimerLabel;
    int currentPlayer;

public:
    Timer(Label^ p1Label, Label^ p2Label);
    void StartTurnTimer();
    void StopTimer();
    void UpdateTimeDisplay();
    void TurnTimer_Tick(Object^ sender, EventArgs^ e);
    void SetCurrentPlayer(int player);
    int GetTurnSeconds() { return turnSeconds; }
    bool IsActive() { return isActive; }
};