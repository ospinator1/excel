#pragma once
#include <Windows.h>
#include "MyForm.h"

ref class GameTimer
{
private:
    System::Windows::Forms::Timer^ turnTimer;
    int totalSeconds;
    int turnSeconds;
    bool isActive;

public:
    GameTimer();
    void StartTurnTimer();
    void StopTimer();
    void UpdateTimeDisplay(System::Windows::Forms::Label^ label);
    void TickHandler(System::Object^ sender, System::EventArgs^ e);

    property int TurnSeconds{ int get() { return turnSeconds; } void set(int value) { turnSeconds = value; } }
    property bool IsActive{ bool get() { return isActive; } }
};