#pragma once
#include <Windows.h>
#include "Game.h"
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;
public ref class BackgammonForm : public Form
{
private:
    Game* game;

    // UI элементы
    Button^ btnRollDice;
    Button^ btnEndTurn;
    Button^ btnRules;
    Timer^ turnTimer;
    Label^ lblTimePlayer1;
    Label^ lblTimePlayer2;

public:
    BackgammonForm();
    void Form_Paint(Object^ sender, PaintEventArgs^ e) override;
    void Form_MouseDown(Object^ sender, MouseEventArgs^ e);
    void Form_MouseMove(Object^ sender, MouseEventArgs^ e);
    void Form_MouseUp(Object^ sender, MouseEventArgs^ e);
    void ShowRulesButton_Click(Object^ sender, EventArgs^ e);
    void UpdateTurnTimerDisplay();

private:
    void InitializeUI();
};