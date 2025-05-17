#include "Timer.h"

GameTimer::GameTimer()
{
    turnTimer = gcnew System::Windows::Forms::Timer();
    turnTimer->Interval = 1000;
    totalSeconds = 0;
    turnSeconds = 30;
    isActive = false;
}

void GameTimer::StartTurnTimer()
{
    turnSeconds = 30;
    isActive = true;
    turnTimer->Start();
}

void GameTimer::StopTimer()
{
    isActive = false;
    turnTimer->Stop();
}

void GameTimer::UpdateTimeDisplay(System::Windows::Forms::Label^ label)
{
    label->Text = System::String::Format("Время: {0} сек", turnSeconds);
}

void GameTimer::TickHandler(System::Object^ sender, System::EventArgs^ e)
{
    turnSeconds--;
    if (turnSeconds <= 0)
    {
        StopTimer();
    }
}