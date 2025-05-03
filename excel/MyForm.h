#pragma once
#include <Windows.h>
#include <gcroot.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

// Singleton Pattern for Game Rules
ref class GameRules
{
private:
    static GameRules^ instance;
    String^ rulesText;

    GameRules()
    {
        rulesText = "Правила игры в Нарды:\n\n";
        rulesText += "Цель игры: Переместить все свои шашки в свой дом и затем 'снять' их с доски.\n\n";
        rulesText += "Начальная расстановка: Каждая сторона имеет 15 шашек. Игрок 1 (черные) начинает с 2 шашек на 24-м поле, 5 на 13-м, 3 на 8-м и 5 на 6-м. Игрок 2 (белые) начинает с 2 шашек на 1-м поле, 5 на 12-м, 3 на 17-м и 5 на 19-м.\n\n";
        rulesText += "Ход: Игроки по очереди бросают два кубика и перемещают шашки на количество полей, выпавшее на каждом кубике. Можно использовать оба значения для одной шашки или переместить две разные шашки.\n\n";
        rulesText += "Дубль: Если на кубиках выпадает одинаковое число (дубль), игрок может сделать четыре хода на это число.\n\n";
        rulesText += "Занятое поле: Шашку можно переместить на поле, если на нем нет шашек противника, или есть только одна шашка противника (блокада).\n\n";
        rulesText += "Взятие: Если шашка попадает на поле, занятое одной шашкой противника, шашка противника 'бьется' и помещается на 'бар'.\n\n";
        rulesText += "Бар: Шашки на баре должны быть введены обратно на доску с начальных полей противника (для игрока 1 - поля 1-6, для игрока 2 - поля 19-24). Для этого нужно выбросить число на кубике, соответствующее свободному полю в доме противника. Пока на баре есть шашки, игрок обязан ходить только ими.\n\n";
        rulesText += "Дом: Дом игрока 1 - поля 1-6, дом игрока 2 - поля 19-24.\n\n";
        rulesText += "Снятие шашек: Когда все шашки игрока находятся в его доме, он может начать 'снимать' их с доски. Для этого нужно выбросить на кубике число, равное расстоянию от шашки до края доски. Если выпавшее число больше необходимого, можно снять шашку с самого дальнего поля, на котором есть шашка.\n\n";
        rulesText += "Победа: Побеждает игрок, который первым снимет все свои шашки с доски.";
    }

public:
    static GameRules^ GetInstance()
    {
        if (instance == nullptr)
        {
            instance = gcnew GameRules();
        }
        return instance;
    }

    String^ GetRules()
    {
        return rulesText;
    }
};

// Abstract Factory Pattern for Checker Creation
ref class CheckerFactory abstract
{
public:
    virtual Color CreateColor() abstract;
};

ref class Player1CheckerFactory : public CheckerFactory
{
public:
    virtual Color CreateColor() override
    {
        return Color::FromArgb(50, 50, 50); // Dark color for Player 1
    }
};

ref class Player2CheckerFactory : public CheckerFactory
{
public:
    virtual Color CreateColor() override
    {
        return Color::FromArgb(240, 240, 240); // Light color for Player 2
    }
};


public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkerPositions;
    List<Color>^ checkerColors;
    List<int>^ checkerPoints;
    int checkerRadius;

    Random^ random;

    // Game state
    int currentPlayer;
    int player1Dice1, player1Dice2;
    int player2Dice1, player2Dice2;
    List<int>^ availableMoves;
    List<int>^ highlightedPoints;
    int selectedCheckerIndex;
    Point initialCheckerPosition;
    bool hasMovedFromBar;
    bool mustMoveFromBar;  // Новый флаг: обязан ходить из бара

    // UI elements
    Button^ rollDiceButton;
    Button^ endTurnButtonP1;
    Button^ endTurnButtonP2;
    Button^ showRulesButton;

    // Board dimensions
    int boardWidth;
    int boardHeight;
    int triangleWidth;
    int triangleHeight;
    int boardOffsetX;

    // Move-related variables
    int moveCounter;
    List<int>^ checkerMoveCount;

    // Bar positions
    List<int>^ barCheckersP1;
    List<int>^ barCheckersP2;
    Point barPositionP1;
    Point barPositionP2;

    // Vertical offsets
    const int VERTICAL_OFFSET = 50;
    const int UI_VERTICAL_OFFSET = 100;

public:
    property Point BarPositionP1
    {
        Point get() { return barPositionP1; }
        void set(Point value) { barPositionP1 = value; }
    }

        property Point BarPositionP2
    {
        Point get() { return barPositionP2; }
        void set(Point value) { barPositionP2 = value; }
    }

public:
    BackgammonForm()
    {
        this->Text = "Backgammon";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1200, 750);
        this->BackColor = Color::SkyBlue;

        checkerRadius = 20;
        boardWidth = 700;
        boardHeight = 500;
        triangleWidth = boardWidth / 12;
        triangleHeight = boardHeight / 2;
        boardOffsetX = (this->ClientSize.Width - boardWidth) / 2;

        checkerPositions = gcnew List<Point>();
        checkerColors = gcnew List<Color>();
        checkerPoints = gcnew List<int>();
        availableMoves = gcnew List<int>();
        highlightedPoints = gcnew List<int>();
        selectedCheckerIndex = -1;
        random = gcnew Random();
        currentPlayer = 1;
        player1Dice1 = player1Dice2 = 0;
        player2Dice1 = player2Dice2 = 0;
        moveCounter = 0;
        hasMovedFromBar = false;
        mustMoveFromBar = false;

        checkerMoveCount = gcnew List<int>();
        barCheckersP1 = gcnew List<int>();
        barCheckersP2 = gcnew List<int>();

        this->BarPositionP1 = Point(boardOffsetX + boardWidth - checkerRadius, boardHeight / 2 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET + 50);
        this->BarPositionP2 = Point(boardOffsetX + boardWidth + checkerRadius, boardHeight / 2 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET + 50);

        InitializeUI();
        InitializeCheckers();

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            checkerMoveCount->Add(0);
        }

        this->MouseDown += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &BackgammonForm::Form_Paint);
    }

private:
    void InitializeUI()
    {
        rollDiceButton = gcnew Button();
        rollDiceButton->Text = "Бросить кубики";
        rollDiceButton->Size = System::Drawing::Size(120, 40);
        rollDiceButton->Location = System::Drawing::Point(
            this->ClientSize.Width / 2 - rollDiceButton->Width / 2 + 350,
            this->ClientSize.Height - UI_VERTICAL_OFFSET + 100);
        rollDiceButton->BackColor = Color::LightSkyBlue;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        endTurnButtonP1 = gcnew Button();
        endTurnButtonP1->Text = "Завершить ход";
        endTurnButtonP1->Size = System::Drawing::Size(100, 40);
        endTurnButtonP1->Location = System::Drawing::Point(
            this->ClientSize.Width - 1100,
            this->ClientSize.Height - UI_VERTICAL_OFFSET);
        endTurnButtonP1->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP1->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
        endTurnButtonP1->Enabled = false;
        this->Controls->Add(endTurnButtonP1);

        endTurnButtonP2 = gcnew Button();
        endTurnButtonP2->Text = "Завершить ход";
        endTurnButtonP2->Size = System::Drawing::Size(100, 40);
        endTurnButtonP2->Location = System::Drawing::Point(
            this->ClientSize.Width + 500,
            this->ClientSize.Height - UI_VERTICAL_OFFSET);
        endTurnButtonP2->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP2->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP2->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP2_Click);
        endTurnButtonP2->Enabled = false;
        this->Controls->Add(endTurnButtonP2);

        showRulesButton = gcnew Button();
        showRulesButton->Text = "Правила игры";
        showRulesButton->Size = System::Drawing::Size(120, 40);
        showRulesButton->Location = System::Drawing::Point(
            10, 10);
        showRulesButton->BackColor = Color::LightGreen;
        showRulesButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        showRulesButton->Click += gcnew EventHandler(this, &BackgammonForm::ShowRulesButton_Click);
        this->Controls->Add(showRulesButton);
    }

    void AddCheckersForPoint(int pointIndex, int count, CheckerFactory^ factory)
    {
        Point position = GetPointPosition(pointIndex);
        int direction = (pointIndex < 12) ? -1 : 1;
        int spacing = 20;

        int existingCount = 0;
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerPoints[i] == pointIndex)
            {
                existingCount++;
            }
        }

        Color checkerColor = factory->CreateColor();

        for (int i = 0; i < count; i++)
        {
            int y = position.Y + ((existingCount + i) * spacing * direction);
            checkerPositions->Add(Point(position.X, y));
            checkerColors->Add(checkerColor);
            checkerPoints->Add(pointIndex);
            checkerMoveCount->Add(0);
        }
    }

    Point GetPointPosition(int pointIndex)
    {
        int x, y;

        if (pointIndex < 12)
        {
            x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
            y = boardHeight - 30 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;
        }
        else
        {
            x = boardOffsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
            y = 70 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;
        }

        return Point(x, y);
    }

    void InitializeCheckers()
    {
        checkerPositions->Clear();
        checkerColors->Clear();
        checkerPoints->Clear();
        checkerMoveCount->Clear();
        barCheckersP1->Clear();
        barCheckersP2->Clear();

        // Player 1 (dark) checkers using Abstract Factory
        CheckerFactory^ player1Factory = gcnew Player1CheckerFactory();
        AddCheckersForPoint(0, 2, player1Factory);
        AddCheckersForPoint(11, 5, player1Factory);
        AddCheckersForPoint(16, 3, player1Factory);
        AddCheckersForPoint(18, 5, player1Factory);

        // Player 2 (light) checkers using Abstract Factory
        CheckerFactory^ player2Factory = gcnew Player2CheckerFactory();
        AddCheckersForPoint(5, 5, player2Factory);
        AddCheckersForPoint(7, 3, player2Factory);
        AddCheckersForPoint(12, 5, player2Factory);
        AddCheckersForPoint(23, 2, player2Factory);
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (moveCounter >= 2 && availableMoves->Count > 0)
        {
            MessageBox::Show("Вы уже переместили две шашки. Завершите свой ход.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        availableMoves->Clear();

        if (currentPlayer == 1)
        {
            player1Dice1 = random->Next(1, 7);
            player1Dice2 = random->Next(1, 7);
            if (player1Dice1 == player1Dice2)
            {
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
            }
            else
            {
                availableMoves->Add(player1Dice1);
                availableMoves->Add(player1Dice2);
            }
            endTurnButtonP1->Enabled = true;
        }
        else
        {
            player2Dice1 = random->Next(1, 7);
            player2Dice2 = random->Next(1, 7);
            if (player2Dice1 == player2Dice2)
            {
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
            }
            else
            {
                availableMoves->Add(player2Dice1);
                availableMoves->Add(player2Dice2);
            }
            endTurnButtonP2->Enabled = true;
        }

        // Проверяем, есть ли шашки в баре у текущего игрока
        mustMoveFromBar = (currentPlayer == 1 && barCheckersP1->Count > 0) ||
            (currentPlayer == 2 && barCheckersP2->Count > 0);

        if (mustMoveFromBar)
        {
            MessageBox::Show("Вы должны сначала переместить шашку из бара!", "Внимание", MessageBoxButtons::OK, MessageBoxIcon::Information);
        }

        this->Text = String::Format("Backgammon - Ход игрока {0} | Ходы: {1} и {2}",
            currentPlayer, availableMoves[0], availableMoves[1]);

        if (availableMoves->Count > 2)
        {
            this->Text += " (Дубль!)";
        }

        rollDiceButton->Enabled = false;
        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        // Проверяем, есть ли шашки в баре у следующего игрока
        bool nextPlayerHasBar = (barCheckersP2->Count > 0);
        ResetDiceAndEnableNextPlayer(2, nextPlayerHasBar);
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        // Проверяем, есть ли шашки в баре у следующего игрока
        bool nextPlayerHasBar = (barCheckersP1->Count > 0);
        ResetDiceAndEnableNextPlayer(1, nextPlayerHasBar);
    }

    void ResetDiceAndEnableNextPlayer(int nextPlayer, bool nextPlayerHasBar)
    {
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;
        hasMovedFromBar = false;
        mustMoveFromBar = nextPlayerHasBar; // Устанавливаем флаг для следующего игрока

        if (nextPlayer == 1) {
            player1Dice1 = player1Dice2 = 0;
        }
        else {
            player2Dice1 = player2Dice2 = 0;
        }

        rollDiceButton->Enabled = true;
        currentPlayer = nextPlayer;
        moveCounter = 0;

        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0;
        }

        this->Text = String::Format("Backgammon - Ход игрока {0}", currentPlayer);
        this->Invalidate();
    }

    void ShowRulesButton_Click(Object^ sender, EventArgs^ e)
    {
        GameRules^ rules = GameRules::GetInstance();
        MessageBox::Show(rules->GetRules(), "Правила игры в Нарды", MessageBoxButtons::OK, MessageBoxIcon::Information);
    }

    void CalculatePossibleMoves(int checkerIndex)
    {
        highlightedPoints->Clear();

        Color checkerColor = checkerColors[checkerIndex];
        bool isPlayer1 = (checkerColor.R < 100);
        bool isInBar = (isPlayer1 && barCheckersP1->Contains(checkerIndex)) ||
            (!isPlayer1 && barCheckersP2->Contains(checkerIndex));

        if (isInBar || mustMoveFromBar)
        {
            for each(int move in availableMoves)
            {
                int targetPoint = isPlayer1 ? (24 - move) : (move - 1);
                if (targetPoint >= 0 && targetPoint < 24)
                {
                    int opponentCount = 0;
                    Color opponentColor = isPlayer1 ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                    for (int i = 0; i < checkerPoints->Count; i++)
                    {
                        if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                        {
                            opponentCount++;
                        }
                    }

                    if (opponentCount < 2)
                    {
                        highlightedPoints->Add(targetPoint);
                    }
                }
            }
            return;
        }

        int currentPoint = checkerPoints[checkerIndex];
        for each(int move in availableMoves)
        {
            int targetPoint;
            if (isPlayer1)
            {
                targetPoint = currentPoint + move;
            }
            else
            {
                targetPoint = currentPoint - move;
            }

            if (targetPoint >= 0 && targetPoint < 24)
            {
                CheckAndAddHighlightedPoints(targetPoint, isPlayer1);
            }
        }
    }

    void CheckAndAddHighlightedPoints(int targetPoint, bool isPlayer1)
    {
        int opponentCount = 0;
        int ownCount = 0;
        Color opponentColor = isPlayer1 ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);
        Color ownColor = isPlayer1 ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240);

        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerPoints[i] == targetPoint)
            {
                if (checkerColors[i] == opponentColor)
                {
                    opponentCount++;
                }
                else if (checkerColors[i] == ownColor)
                {
                    ownCount++;
                }
            }
        }

        if (opponentCount < 2 && ownCount < 2)
        {
            highlightedPoints->Add(targetPoint);
        }
    }


    void DrawBoard(Graphics^ g)
    {
        Color boardColor = Color::FromArgb(210, 180, 140);

        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);
        g->DrawRectangle(Pens::Black, boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);

        for (int i = 0; i < 12; i++)
        {
            DrawPoint(g, i, true);
            DrawPoint(g, i, false);
        }

        DrawPointNumbers(g);

        g->DrawString("БАР", gcnew Drawing::Font("Arial", 12, FontStyle::Bold),
            Brushes::Black, PointF(barPositionP1.X - 20, barPositionP1.Y - 50));
    }

    void DrawPoint(Graphics^ g, int pointIndex, bool isTop)
    {
        int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
        int y = (isTop ? 50 : 50 + triangleHeight) + VERTICAL_OFFSET + UI_VERTICAL_OFFSET;

        array<Point>^ triangle = gcnew array<Point>(3);
        if (isTop)
        {
            triangle[0] = Point(x - triangleWidth, y);
            triangle[1] = Point(x, y + triangleHeight);
            triangle[2] = Point(x + triangleWidth, y);
        }
        else
        {
            triangle[0] = Point(x - triangleWidth, y + triangleHeight);
            triangle[1] = Point(x, y);
            triangle[2] = Point(x + triangleWidth, y + triangleHeight);
        }

        Color darkColor = Color::FromArgb(139, 69, 19);
        Color lightColor = Color::FromArgb(245, 222, 179);

        Brush^ pointBrush = ((pointIndex % 2 == 0) ^ isTop) ? gcnew SolidBrush(darkColor) : gcnew SolidBrush(lightColor);
        g->FillPolygon(pointBrush, triangle);
        g->DrawPolygon(Pens::Black, triangle);
    }

    void DrawPointNumbers(Graphics^ g)
    {
        Drawing::Font^ font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
        Brush^ brush = Brushes::Black;

        for (int i = 0; i < 12; i++)
        {
            int upperNumber = 13 + i;
            int upperX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(upperNumber.ToString(), font, brush, PointF(upperX, 50 - 25 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET));

            int lowerNumber = 12 - i;
            int lowerX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(lowerNumber.ToString(), font, brush, PointF(lowerX, 50 + boardHeight + 10 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET));
        }
    }

    void DrawDice(Graphics^ g, int x, int y, int value, Color dotColor)
    {
        g->FillRectangle(Brushes::White, x, y, 60, 60);
        g->DrawRectangle(Pens::Black, x, y, 60, 60);
        Brush^ dotBrush = gcnew SolidBrush(dotColor);
        int dotSize = 10;
        int dotOffset = 15;

        switch (value)
        {
        case 1:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            break;
        case 2:
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 3:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 4:
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 5:
            g->FillEllipse(dotBrush, x + 30 - dotSize / 2, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 6:
            g->FillEllipse(dotBrush, x + dotOffset, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 50, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 30 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 50, dotSize, dotSize);
            break;
        }
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;
        g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

        g->FillRectangle(Brushes::SkyBlue, 0, 0, this->ClientSize.Width, this->ClientSize.Height);
        DrawBoard(g);

        // Рисуем шашки в баре для игрока 1 (черные)
        for (int i = 0; i < barCheckersP1->Count; i++)
        {
            g->FillEllipse(Brushes::Black, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        // Рисуем шашки в баре для игрока 2 (белые)
        for (int i = 0; i < barCheckersP2->Count; i++)
        {
            g->FillEllipse(Brushes::White, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        // Подсвечиваем возможные ходы
        for each(int point in highlightedPoints)
        {
            Point pos = GetPointPosition(point);
            g->FillEllipse(Brushes::Yellow, pos.X - checkerRadius - 5, pos.Y - checkerRadius - 5,
                checkerRadius * 2 + 10, checkerRadius * 2 + 10);
        }

        // Рисуем все шашки на доске
        for (int i = 0; i < checkerPositions->Count; i++)
        {
            if (barCheckersP1->Contains(i) || barCheckersP2->Contains(i))
                continue;

            Color color = checkerColors[i];
            Brush^ brush = gcnew SolidBrush(color);
            g->FillEllipse(brush,
                checkerPositions[i].X - checkerRadius,
                checkerPositions[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);

            Pen^ borderPen = gcnew Pen(Color::FromArgb(100, 0, 0, 0), 1.5f);
            g->DrawEllipse(borderPen,
                checkerPositions[i].X - checkerRadius,
                checkerPositions[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);
        }

        // Рисуем кубики для текущего игрока
        if (currentPlayer == 1)
        {
            DrawDice(g, 100, 100 + UI_VERTICAL_OFFSET, player1Dice1, Color::Black);
            DrawDice(g, 180, 100 + UI_VERTICAL_OFFSET, player1Dice2, Color::Black);
        }
        else
        {
            DrawDice(g, this->ClientSize.Width - 220, 100 + UI_VERTICAL_OFFSET, player2Dice1, Color::Red);
            DrawDice(g, this->ClientSize.Width - 140, 100 + UI_VERTICAL_OFFSET, player2Dice2, Color::Red);
        }

        // Отображаем информацию о текущем игроке
        String^ turnText = String::Format("Текущий ход: Игрок {0}", currentPlayer);
        g->DrawString(turnText, gcnew Drawing::Font("Arial", 16, FontStyle::Bold),
            Brushes::DarkBlue, PointF(this->ClientSize.Width / 2 - 100, 10 + UI_VERTICAL_OFFSET));

        // Отображаем количество шашек в баре
        if (barCheckersP1->Count > 0)
        {
            g->DrawString(String::Format("Бар (Черные): {0}", barCheckersP1->Count),
                gcnew Drawing::Font("Arial", 10, FontStyle::Bold),
                Brushes::Black, PointF(barPositionP1.X - 50, barPositionP1.Y - 30));
        }

        if (barCheckersP2->Count > 0)
        {
            g->DrawString(String::Format("Бар (Белые): {0}", barCheckersP2->Count),
                gcnew Drawing::Font("Arial", 10, FontStyle::Bold),
                Brushes::Black, PointF(barPositionP2.X - 50, barPositionP2.Y - 30));
        }

        // Если игрок должен ходить из бара, выводим сообщение
        if (mustMoveFromBar)
        {
            String^ message = "Вы должны ходить шашкой из бара!";
            g->DrawString(message, gcnew Drawing::Font("Arial", 12, FontStyle::Bold),
                Brushes::Red, PointF(this->ClientSize.Width / 2 - 150, 50 + UI_VERTICAL_OFFSET));
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (availableMoves->Count == 0) return;

        // Если игрок должен ходить из бара, разрешаем выбирать только шашки из бара
        if (mustMoveFromBar)
        {
            for (int i = 0; i < checkerPositions->Count; i++)
            {
                bool isPlayer1 = (checkerColors[i].R < 100);
                if ((currentPlayer == 1 && isPlayer1 && barCheckersP1->Contains(i)) ||
                    (currentPlayer == 2 && !isPlayer1 && barCheckersP2->Contains(i)))
                {
                    int dx = e->X - (barCheckersP1->Contains(i) ? barPositionP1.X : barPositionP2.X);
                    int dy = e->Y - (barCheckersP1->Contains(i) ? barPositionP1.Y : barPositionP2.Y);
                    if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
                    {
                        selectedCheckerIndex = i;
                        initialCheckerPosition = checkerPositions[i];
                        CalculatePossibleMoves(i);
                        this->Invalidate();
                        return;
                    }
                }
            }
            return;
        }

        // Обычный выбор шашки
        for (int i = 0; i < checkerPositions->Count; i++)
        {
            bool isPlayer1 = (checkerColors[i].R < 100);
            if ((currentPlayer == 1 && isPlayer1) || (currentPlayer == 2 && !isPlayer1))
            {
                // Пропускаем шашки в баре, если не обязаны ходить из него
                if ((isPlayer1 && barCheckersP1->Contains(i)) || (!isPlayer1 && barCheckersP2->Contains(i)))
                {
                    continue;
                }

                int dx = e->X - checkerPositions[i].X;
                int dy = e->Y - checkerPositions[i].Y;
                if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                {
                    selectedCheckerIndex = i;
                    initialCheckerPosition = checkerPositions[i];
                    CalculatePossibleMoves(i);
                    this->Invalidate();
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (selectedCheckerIndex != -1)
        {
            Point currentPos = checkerPositions[selectedCheckerIndex];
            currentPos.X = e->X - (checkerRadius);
            currentPos.Y = e->Y - (checkerRadius);
            checkerPositions[selectedCheckerIndex] = currentPos;
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (selectedCheckerIndex == -1) return;

        if (e->Button == System::Windows::Forms::MouseButtons::Right)
        {
            checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
            selectedCheckerIndex = -1;
            this->Invalidate();
            return;
        }

        bool isPlayer1 = (checkerColors[selectedCheckerIndex].R < 100);
        bool isInBar = (isPlayer1 && barCheckersP1->Contains(selectedCheckerIndex)) ||
            (!isPlayer1 && barCheckersP2->Contains(selectedCheckerIndex));

        for each(int targetPoint in highlightedPoints)
        {
            Point targetPos = GetPointPosition(targetPoint);
            int dx = e->X - targetPos.X;
            int dy = e->Y - targetPos.Y;

            if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
            {
                if (isInBar)
                {
                    hasMovedFromBar = true;
                    mustMoveFromBar = false; // Снимаем флаг обязательного хода из бара
                    checkerPositions[selectedCheckerIndex] = targetPos;
                    checkerPoints[selectedCheckerIndex] = targetPoint;

                    if (isPlayer1)
                        barCheckersP1->Remove(selectedCheckerIndex);
                    else
                        barCheckersP2->Remove(selectedCheckerIndex);

                    int moveUsed = isPlayer1 ? (24 - targetPoint) : (targetPoint + 1);
                    availableMoves->Remove(moveUsed);
                    moveCounter++;
                }
                else
                {
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int moveLength = Math::Abs(targetPoint - currentPoint);

                    Color opponentColor = (currentPlayer == 1) ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);
                    int opponentCount = 0;

                    for (int i = 0; i < checkerPoints->Count; i++)
                    {
                        if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                        {
                            opponentCount++;
                        }
                    }

                    if (opponentCount < 2)
                    {
                        if (opponentCount > 0)
                        {
                            for (int i = 0; i < checkerPoints->Count; i++)
                            {
                                if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                                {
                                    if (currentPlayer == 1)
                                    {
                                        barCheckersP2->Add(i);
                                        checkerPositions[i] = barPositionP2;
                                    }
                                    else
                                    {
                                        barCheckersP1->Add(i);
                                        checkerPositions[i] = barPositionP1;
                                    }

                                    checkerPoints[i] = -1;
                                    break;
                                }
                            }
                        }

                        checkerPositions[selectedCheckerIndex] = targetPos;
                        checkerPoints[selectedCheckerIndex] = targetPoint;
                        checkerMoveCount[selectedCheckerIndex]++;

                        availableMoves->Remove(moveLength);
                        moveCounter++;
                    }
                }
                break;
            }
        }

        selectedCheckerIndex = -1;
        highlightedPoints->Clear();
        this->Invalidate();
    }
};