#pragma once
#include "Board.h"
#include "Checker.h"
#include "Dice.h"
#include "Game.h"
#include "Bar.h"
#include "Timer.h"
#include "Rules.h"
#include <Windows.h>
using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

public ref class BackgammonForm : public System::Windows::Forms::Form
{
private:
    Board^ board; // ����� ��� ���� � �����
    Checkers^ checkers; // ��������� �����
    Dice^ dice; // ������ ��� ������
    Game^ game; // ���������� ������� ����
    Bar^ bar; // ������ ��� ����������� �����
    GameTimer^ timer; // ������ ��� ������� ������� ����
    GameRules^ rules; // ������� ����

    // UI ��������
    System::Windows::Forms::Button^ rollDiceButton; // ������ ��� ������ �������
    System::Windows::Forms::Button^ endTurnButtonP1; // ������ ��� ���������� ���� ������ 1
    System::Windows::Forms::Button^ endTurnButtonP2; // ������ ��� ���������� ���� ������ 2
    System::Windows::Forms::Button^ showRulesButton; // ������ ��� ����������� ������ ����
    System::Windows::Forms::Label^ player1TimerLabel; // ����� ��� ����������� ������� ������ 1
    System::Windows::Forms::Label^ player2TimerLabel; // ����� ��� ����������� ������� ������ 2

    int selectedCheckerIndex; // ������ ��������� �����
    System::Drawing::Point initialCheckerPosition; // ��������� ������� �����

    void InitializeUI(); // �������������� ���������������� ��������� �����
    void InitializeGameComponents(); // �������������� ���������� ����
    void ResetGame(); // ������������� ����
    void CheckForWin(); // ��������� ������� ������
    void CheckForAutomaticBearOff(int player); // ��������� ����������� ��������������� ������ �����
    bool CheckIfBearingOffPossible(int player); // ��������� ����������� ������ ����� ��� ������

    // ����������� �������
    void RollDiceButton_Click(System::Object^ sender, System::EventArgs^ e); // ���������� ������� ������ ������ �������
    void EndTurnButtonP1_Click(System::Object^ sender, System::EventArgs^ e); // ���������� ������� ������ ���������� ���� ������ 1
    void EndTurnButtonP2_Click(System::Object^ sender, System::EventArgs^ e); // ���������� ������� ������ ���������� ���� ������ 2
    void ShowRulesButton_Click(System::Object^ sender, System::EventArgs^ e); // ���������� ������� ������ ����������� ������
    void Form_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // ���������� ������� ������ ���� �� �����
    void Form_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // ���������� ����������� ���� �� �����
    void Form_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // ���������� ���������� ������ ���� �� �����
    void Form_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e); // ���������� ����������� �����
    void TurnTimer_Tick(System::Object^ sender, System::EventArgs^ e); // ���������� ���� ������� ����

public:
    BackgammonForm(); // ����������� �����
};