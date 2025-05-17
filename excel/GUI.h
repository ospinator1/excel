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
    Board^ board; // Доска для игры в нарды
    Checkers^ checkers; // Коллекция шашек
    Dice^ dice; // Кубики для броска
    Game^ game; // Управление логикой игры
    Bar^ bar; // Плашка для выброшенных шашек
    GameTimer^ timer; // Таймер для отсчета времени хода
    GameRules^ rules; // Правила игры

    // UI элементы
    System::Windows::Forms::Button^ rollDiceButton; // Кнопка для броска кубиков
    System::Windows::Forms::Button^ endTurnButtonP1; // Кнопка для завершения хода игрока 1
    System::Windows::Forms::Button^ endTurnButtonP2; // Кнопка для завершения хода игрока 2
    System::Windows::Forms::Button^ showRulesButton; // Кнопка для отображения правил игры
    System::Windows::Forms::Label^ player1TimerLabel; // Метка для отображения времени игрока 1
    System::Windows::Forms::Label^ player2TimerLabel; // Метка для отображения времени игрока 2

    int selectedCheckerIndex; // Индекс выбранной шашки
    System::Drawing::Point initialCheckerPosition; // Начальная позиция шашки

    void InitializeUI(); // Инициализирует пользовательский интерфейс формы
    void InitializeGameComponents(); // Инициализирует компоненты игры
    void ResetGame(); // Перезагружает игру
    void CheckForWin(); // Проверяет условие победы
    void CheckForAutomaticBearOff(int player); // Проверяет возможность автоматического вывода шашек
    bool CheckIfBearingOffPossible(int player); // Проверяет возможность вывода шашек для игрока

    // Обработчики событий
    void RollDiceButton_Click(System::Object^ sender, System::EventArgs^ e); // Обработчик нажатия кнопки броска кубиков
    void EndTurnButtonP1_Click(System::Object^ sender, System::EventArgs^ e); // Обработчик нажатия кнопки завершения хода игрока 1
    void EndTurnButtonP2_Click(System::Object^ sender, System::EventArgs^ e); // Обработчик нажатия кнопки завершения хода игрока 2
    void ShowRulesButton_Click(System::Object^ sender, System::EventArgs^ e); // Обработчик нажатия кнопки отображения правил
    void Form_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // Обработчик нажатия кнопки мыши на форме
    void Form_MouseMove(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // Обработчик перемещения мыши по форме
    void Form_MouseUp(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e); // Обработчик отпускания кнопки мыши на форме
    void Form_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e); // Обработчик перерисовки формы
    void TurnTimer_Tick(System::Object^ sender, System::EventArgs^ e); // Обработчик тика таймера хода

public:
    BackgammonForm(); // Конструктор формы
};