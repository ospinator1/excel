#include "Timer.h"

Timer::Timer(Label^ p1Label, Label^ p2Label)
{
    totalSeconds = 0;
    turnSeconds = 30;
    isActive = false;
    player1TimerLabel = p1Label;
    player2TimerLabel = p2Label;
    currentPlayer = 1;

    turnTimer = gcnew System::Windows::Forms::Timer();
    turnTimer->Interval = 1000;
    turnTimer->Tick += gcnew EventHandler(this, &Timer::TurnTimer_Tick);
}

void Timer::StartTurnTimer()
{
    turnSeconds = 30;
    isActive = true;
    UpdateTimeDisplay();
    turnTimer->Start();
}

void Timer::StopTimer()
{
    isActive = false;
    turnTimer->Stop();
    UpdateTimeDisplay();
}

void Timer::UpdateTimeDisplay()
{
    if (currentPlayer == 1)
        player1TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
    else
        player2TimerLabel->Text = String::Format("Время: {0} сек", turnSeconds);
}

void Timer::TurnTimer_Tick(Object^ sender, EventArgs^ e)
{
    turnSeconds--;
    UpdateTimeDisplay();

    if (turnSeconds <= 0)
    {
        StopTimer();
        MessageBox::Show("Время на ход истекло!", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Warning);
    }
}

void Timer::SetCurrentPlayer(int player)
{
    currentPlayer = player;
    if (currentPlayer == 1)
    {
        player1TimerLabel->ForeColor = Color::Red;
        player2TimerLabel->ForeColor = Color::Black;
    }
    else
    {
        player2TimerLabel->ForeColor = Color::Red;
        player1TimerLabel->ForeColor = Color::Black;
    }
    UpdateTimeDisplay();
}