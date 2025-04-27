#pragma once
#include <Windows.h>
#include <gcroot.h>
#include <iostream>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

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

    // UI elements
    Button^ rollDiceButton;
    Button^ endTurnButtonP1;
    Button^ endTurnButtonP2;

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
    List<int>^ barCheckersP1; // Black checkers in the bar
    List<int>^ barCheckersP2; // White checkers in the bar
    Point barPositionP1;
    Point barPositionP2;

    // Vertical offset for all elements
    const int VERTICAL_OFFSET = 50;
    const int UI_VERTICAL_OFFSET = 100; // Vertical offset for the UI

public:
    // Properties to get/set the bar positions
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

        // Constructor
public:
    BackgammonForm()
    {
        this->Text = "Backgammon";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1200, 750);

        // Set a blue background color for the form
        this->BackColor = Color::SkyBlue; // Choose your preferred blue color

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

        checkerMoveCount = gcnew List<int>();
        barCheckersP1 = gcnew List<int>();
        barCheckersP2 = gcnew List<int>();

        // Initialize bar positions - centered below the "BAR" text
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

        // End Turn Buttons
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
    }

    void AddCheckersForPoint(int pointIndex, int count, Color color)
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

        for (int i = 0; i < count; i++)
        {
            int y = position.Y + ((existingCount + i) * spacing * direction);
            checkerPositions->Add(Point(position.X, y));
            checkerColors->Add(color);
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

        // Player 1 (dark) checkers
        AddCheckersForPoint(0, 2, Color::FromArgb(50, 50, 50));
        AddCheckersForPoint(11, 5, Color::FromArgb(50, 50, 50));
        AddCheckersForPoint(16, 3, Color::FromArgb(50, 50, 50));
        AddCheckersForPoint(18, 5, Color::FromArgb(50, 50, 50));

        // Player 2 (light) checkers
        AddCheckersForPoint(5, 5, Color::FromArgb(240, 240, 240));
        AddCheckersForPoint(7, 3, Color::FromArgb(240, 240, 240));
        AddCheckersForPoint(12, 5, Color::FromArgb(240, 240, 240));
        AddCheckersForPoint(23, 2, Color::FromArgb(240, 240, 240));
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (moveCounter >= 2)
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

        this->Text = String::Format("Backgammon - Ход игрока {0} | Ходы: {1} и {2}",
            currentPlayer, availableMoves[0], availableMoves[1]);

        if (availableMoves->Count > 2)
        {
            this->Text += " (Дубль!)";
        }

        // Disable the Roll Dice button after rolling
        rollDiceButton->Enabled = false;
        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        // Reset the dice values after the turn and set currentPlayer to 2
        ResetDiceAndEnableNextPlayer(2);
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        // Reset the dice values after the turn and set currentPlayer to 1
        ResetDiceAndEnableNextPlayer(1);
    }

    void ResetDiceAndEnableNextPlayer(int nextPlayer)
    {
        // Reset the moves and selected checker index
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;

        // Reset dice values for the next player
        if (nextPlayer == 1) {
            player1Dice1 = player1Dice2 = 0;
        }
        else {
            player2Dice1 = player2Dice2 = 0;
        }

        rollDiceButton->Enabled = true; // Enable dice button for the next player
        currentPlayer = nextPlayer; // Set the current player

        moveCounter = 0;
        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0;
        }

        this->Text = String::Format("Backgammon - Ход игрока {0}", currentPlayer);
        this->Invalidate();
    }

    void ResetTurn()
    {
        availableMoves->Clear();
        highlightedPoints->Clear();
        selectedCheckerIndex = -1;
        endTurnButtonP1->Enabled = false;
        endTurnButtonP2->Enabled = false;
        rollDiceButton->Enabled = true;

        moveCounter = 0;
        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0;
        }
    }

    void CalculatePossibleMoves(int checkerIndex)
    {
        highlightedPoints->Clear();

        // Check if there are checkers in the bar that can be used
        bool hasBarCheckers = (currentPlayer == 1 && barCheckersP1->Count > 0) ||
            (currentPlayer == 2 && barCheckersP2->Count > 0);

        if (hasBarCheckers)
        {
            // If the current checker is not from the bar, it cannot be selected
            if ((currentPlayer == 1 && !barCheckersP1->Contains(checkerIndex)) ||
                (currentPlayer == 2 && !barCheckersP2->Contains(checkerIndex)))
            {
                return;
            }
        }

        int currentPoint = checkerPoints[checkerIndex];
        bool isPlayer1 = (checkerColors[checkerIndex].R < 100);

        // If the checker is from the bar, possible moves are entering the board
        if ((isPlayer1 && barCheckersP1->Contains(checkerIndex)) ||
            (!isPlayer1 && barCheckersP2->Contains(checkerIndex)))
        {
            for each(int move in availableMoves)
            {
                int targetPoint = isPlayer1 ? (24 - move) : (move - 1);

                // Check if it is possible to enter the target point
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
            return;
        }

        // Ordinary moves on the board
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
        }
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;
        g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

        // Fill the entire form's background with blue color
        g->FillRectangle(Brushes::SkyBlue, 0, 0, this->ClientSize.Width, this->ClientSize.Height);

        DrawBoard(g);

        // Draw bar checkers - positioned below the "BAR" text in the middle
        for (int i = 0; i < barCheckersP1->Count; i++)
        {
            g->FillEllipse(Brushes::Black, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,  // Increased spacing for better visibility
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP1.X - checkerRadius,
                barPositionP1.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        for (int i = 0; i < barCheckersP2->Count; i++)
        {
            g->FillEllipse(Brushes::White, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,  // Increased spacing for better visibility
                checkerRadius * 2, checkerRadius * 2);
            g->DrawEllipse(Pens::Gray, barPositionP2.X - checkerRadius,
                barPositionP2.Y - checkerRadius + i * 30,
                checkerRadius * 2, checkerRadius * 2);
        }

        for each(int point in highlightedPoints)
        {
            Point pos = GetPointPosition(point);
            g->FillEllipse(Brushes::Yellow, pos.X - checkerRadius - 5, pos.Y - checkerRadius - 5,
                checkerRadius * 2 + 10, checkerRadius * 2 + 10);
        }

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            // Don't draw checkers that are in the bar
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

        // Draw dice with vertical offset
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

        // Show current player text with vertical offset
        String^ turnText = String::Format("Текущий ход: Игрок {0}", currentPlayer);
        g->DrawString(turnText, gcnew Drawing::Font("Arial", 16, FontStyle::Bold),
            Brushes::DarkBlue, PointF(this->ClientSize.Width / 2 - 100, 10 + UI_VERTICAL_OFFSET));

        // Show bar checkers count
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
    }

    void DrawBoard(Graphics^ g)
    {
        Color boardColor = Color::FromArgb(210, 180, 140);

        // Fill the entire board with one color
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);
        g->DrawRectangle(Pens::Black, boardOffsetX, 50 + VERTICAL_OFFSET + UI_VERTICAL_OFFSET, boardWidth * 2, boardHeight);

        // Draw upper and lower triangles
        for (int i = 0; i < 12; i++)
        {
            DrawPoint(g, i, true);  // Upper triangles
            DrawPoint(g, i, false); // Lower triangles
        }

        // Draw point numbers
        DrawPointNumbers(g);

        // Draw the bar title in the middle of the board
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

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (availableMoves->Count == 0) return;

        // Check if there are checkers in the bar
        bool hasBarCheckers = (currentPlayer == 1 && barCheckersP1->Count > 0) ||
            (currentPlayer == 2 && barCheckersP2->Count > 0);

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            bool isPlayer1 = (checkerColors[i].R < 100);
            if ((currentPlayer == 1 && isPlayer1) || (currentPlayer == 2 && !isPlayer1))
            {
                // If there are checkers in the bar, only those can be selected
                if (hasBarCheckers)
                {
                    if ((currentPlayer == 1 && !barCheckersP1->Contains(i)) ||
                        (currentPlayer == 2 && !barCheckersP2->Contains(i)))
                    {
                        continue;
                    }
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
        if (e->Button == System::Windows::Forms::MouseButtons::Right)
        {
            if (selectedCheckerIndex != -1)
            {
                checkerPositions[selectedCheckerIndex] = initialCheckerPosition;
                selectedCheckerIndex = -1;
                this->Invalidate();
            }
            return;
        }

        if (selectedCheckerIndex != -1 && highlightedPoints->Count > 0)
        {
            for each(int targetPoint in highlightedPoints)
            {
                Point targetPos = GetPointPosition(targetPoint);
                int dx = e->X - targetPos.X;
                int dy = e->Y - targetPos.Y;

                if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
                {
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int moveLength = Math::Abs(targetPoint - currentPoint);

                    // Проверка захвата противника
                    if (checkerPoints[selectedCheckerIndex] != -1) // Если у шашки есть точка
                    {
                        int opponentCount = 0;
                        Color opponentColor = (currentPlayer == 1) ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                        // Подсчёт шашек противника на целевой точке
                        for (int i = 0; i < checkerPoints->Count; i++)
                        {
                            if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                            {
                                opponentCount++;
                            }
                        }

                        // Если на целевой точке менее двух шашек противника, можно захватывать
                        if (opponentCount < 2)
                        {
                            // Если там шашка противника, добавляем её в бар
                            if (opponentCount > 0)
                            {
                                for (int i = 0; i < checkerPoints->Count; i++)
                                {
                                    if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                                    {
                                        // Перемещения шашки противника в бар
                                        if (currentPlayer == 1)
                                            barCheckersP2->Add(i);  // Добавляем шашку игрока 2 в бар
                                        else
                                            barCheckersP1->Add(i);  // Добавляем шашку игрока 1 в бар

                                        // Убираем шашку противника с доски
                                        checkerPositions[i] = Point(-500, -500); // Убираем её с отображаемой доски
                                        checkerPoints[i] = -1; // Обозначаем, что шашка в баре
                                        break; // Выходим из цикла после захвата
                                    }
                                }
                            }

                            // Обычное движение шашки
                            checkerPositions[selectedCheckerIndex] = targetPos; // Перемещение на целевую позицию
                            checkerPoints[selectedCheckerIndex] = targetPoint; // Обновление точки позиционирования
                            checkerMoveCount[selectedCheckerIndex]++; // Увеличение счётчика перемещений для шашки

                            // If the checker was in the bar, remove it from the bar
                            if (currentPlayer == 1 && barCheckersP1->Contains(selectedCheckerIndex))
                            {
                                barCheckersP1->Remove(selectedCheckerIndex);
                            }
                            else if (currentPlayer == 2 && barCheckersP2->Contains(selectedCheckerIndex))
                            {
                                barCheckersP2->Remove(selectedCheckerIndex);
                            }

                            availableMoves->Remove(moveLength); // Удаляем использованное значение броска
                            moveCounter++; // Увеличиваем счетчик ходов
                        }
                    }

                    break; // Выход из цикла после обработки выбранной шашки
                }
            }

            selectedCheckerIndex = -1; // Сброс выделенной шашки
            highlightedPoints->Clear(); // Очистка выделенных точек
            this->Invalidate(); // Обновление формы
        }
    }
};