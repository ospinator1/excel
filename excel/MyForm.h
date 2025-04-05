#pragma once

#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkers;
    List<bool>^ isDraggingFlags;
    List<int>^ moveCounts;
    int checkerRadius;
    int draggedCheckerIndex;
    Button^ diceButton;
    Random^ random;
    int currentDiceValue;
    int remainingMoves;
    int boardWidth;
    int boardHeight;
    int columnWidth;
    int pointHeight;

public:
    BackgammonForm()
    {
        this->Text = "Backgammon Game";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1000, 700);

        // Board dimensions
        boardWidth = 800;
        boardHeight = 500;
        columnWidth = boardWidth / 12;
        pointHeight = boardHeight / 2;
        checkerRadius = 18;

        checkers = gcnew List<Point>();
        isDraggingFlags = gcnew List<bool>();
        moveCounts = gcnew List<int>();
        draggedCheckerIndex = -1;
        random = gcnew Random();
        currentDiceValue = 0;
        remainingMoves = 0;

        // Initialize dice button
        diceButton = gcnew Button();
        diceButton->Text = "Roll Dice";
        diceButton->Size = System::Drawing::Size(120, 50);
        diceButton->Location = Point(this->ClientSize.Width / 2 - 60, boardHeight + 30);
        diceButton->BackColor = Color::LightGray;
        diceButton->Font = gcnew System::Drawing::Font("Arial", 12, FontStyle::Bold);
        diceButton->Click += gcnew EventHandler(this, &BackgammonForm::DiceButton_Click);
        this->Controls->Add(diceButton);

        // Initialize checkers in backgammon setup
        InitializeBackgammonCheckers();

        // Set up event handlers
        this->MouseDown += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &BackgammonForm::Form_Paint);
    }

private:
    void InitializeBackgammonCheckers()
    {
        checkers->Clear();
        isDraggingFlags->Clear();
        moveCounts->Clear();

        // Standard backgammon starting setup (15 white, 15 black checkers)

        // White checkers (Player 1)
        AddCheckerStack(1, 2, true);   // Point 1 - 2 checkers
        AddCheckerStack(12, 5, true);  // Point 12 - 5 checkers
        AddCheckerStack(17, 3, true);  // Point 17 - 3 checkers
        AddCheckerStack(19, 5, true); // Point 19 - 5 checkers

        // Black checkers (Player 2)
        AddCheckerStack(6, 5, false);  // Point 6 - 5 checkers
        AddCheckerStack(8, 3, false);  // Point 8 - 3 checkers
        AddCheckerStack(13, 5, false); // Point 13 - 5 checkers
        AddCheckerStack(24, 2, false); // Point 24 - 2 checkers
    }

    void AddCheckerStack(int pointNumber, int checkerCount, bool isWhite)
    {
        int column;
        if (pointNumber <= 12)
        {
            column = 12 - pointNumber; // Right side
        }
        else
        {
            column = pointNumber - 13; // Left side
        }

        int x = 100 + column * columnWidth + columnWidth / 2;
        bool isTop = (pointNumber <= 12) ? isWhite : !isWhite;

        int startY = isTop ? 50 : 50 + pointHeight;
        int direction = isTop ? 1 : -1;

        // Add checkers to the stack
        for (int i = 0; i < checkerCount; i++)
        {
            int y = startY + direction * i * (checkerRadius * 1.6);
            checkers->Add(Point(x, y));
            isDraggingFlags->Add(false);
            moveCounts->Add(0);
        }
    }

    void DiceButton_Click(Object^ sender, EventArgs^ e)
    {
        currentDiceValue = random->Next(1, 7);
        remainingMoves = currentDiceValue;
        this->Text = String::Format("Backgammon - Dice: {0} | Moves left: {1}", currentDiceValue, remainingMoves);

        for (int i = 0; i < moveCounts->Count; i++)
        {
            moveCounts[i] = currentDiceValue;
        }

        this->Invalidate();
    }

    void DrawBoard(Graphics^ g)
    {
        // Board background
        g->FillRectangle(Brushes::BurlyWood, 100, 50, boardWidth, boardHeight);

        // Draw points (triangles)
        for (int i = 0; i < 12; i++)
        {
            int x = 100 + i * columnWidth;

            // Top points
            array<Point>^ topPoints = gcnew array<Point>(3);
            topPoints[0] = Point(x, 50);
            topPoints[1] = Point(x + columnWidth, 50);
            topPoints[2] = Point(x + columnWidth / 2, 50 + pointHeight);
            g->FillPolygon(gcnew SolidBrush((i % 2 == 0) ? Color::DarkRed : Color::BurlyWood), topPoints);

            // Bottom points
            array<Point>^ bottomPoints = gcnew array<Point>(3);
            bottomPoints[0] = Point(x, 50 + boardHeight);
            bottomPoints[1] = Point(x + columnWidth, 50 + boardHeight);
            bottomPoints[2] = Point(x + columnWidth / 2, 50 + pointHeight);
            g->FillPolygon(gcnew SolidBrush((i % 2 == 1) ? Color::DarkRed : Color::BurlyWood), bottomPoints);
        }

        // Middle bar
        g->FillRectangle(Brushes::Sienna, 100 + boardWidth / 2 - 15, 50, 30, boardHeight);

        // Board border
        g->DrawRectangle(gcnew Pen(Color::Black, 3), 100, 50, boardWidth, boardHeight);
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

        // Draw board
        DrawBoard(g);

        // Draw dice info
        String^ diceText = String::Format("Dice: {0}", currentDiceValue);
        g->DrawString(diceText, gcnew System::Drawing::Font("Arial", 16, FontStyle::Bold), Brushes::Black, 120, boardHeight + 80);

        String^ movesText = String::Format("Moves left: {0}", remainingMoves);
        g->DrawString(movesText, gcnew System::Drawing::Font("Arial", 16, FontStyle::Bold), Brushes::Black, 120, boardHeight + 110);

        // Draw all checkers
        for (int i = 0; i < checkers->Count; i++)
        {
            bool isWhite = (i < 15); // First 15 are white
            Color checkerColor = isWhite ? Color::White : Color::Black;

            if (moveCounts[i] > 0)
                checkerColor = Color::FromArgb(220, checkerColor.R, checkerColor.G, checkerColor.B);
            else
                checkerColor = Color::FromArgb(150, checkerColor.R, checkerColor.G, checkerColor.B);

            if (i == draggedCheckerIndex) checkerColor = Color::Red;

            Brush^ brush = gcnew SolidBrush(checkerColor);
            Pen^ pen = gcnew Pen(isWhite ? Color::DarkGray : Color::DimGray, 1);

            g->FillEllipse(brush,
                checkers[i].X - checkerRadius,
                checkers[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);

            g->DrawEllipse(pen,
                checkers[i].X - checkerRadius,
                checkers[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);

            // Draw move count
            if (moveCounts[i] > 0)
            {
                String^ moveCountText = moveCounts[i].ToString();
                System::Drawing::Font^ font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
                SizeF textSize = g->MeasureString(moveCountText, font);
                g->DrawString(moveCountText, font, isWhite ? Brushes::Black : Brushes::White,
                    checkers[i].X - textSize.Width / 2,
                    checkers[i].Y - textSize.Height / 2);
            }
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (remainingMoves <= 0) return;

        for (int i = 0; i < checkers->Count; i++)
        {
            if (moveCounts[i] > 0)
            {
                int dx = e->X - checkers[i].X;
                int dy = e->Y - checkers[i].Y;
                if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                {
                    isDraggingFlags[i] = true;
                    draggedCheckerIndex = i;
                    this->Cursor = Cursors::Hand;
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCheckerIndex != -1 && isDraggingFlags[draggedCheckerIndex] && moveCounts[draggedCheckerIndex] > 0)
        {
            checkers[draggedCheckerIndex] = e->Location;
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCheckerIndex != -1 && isDraggingFlags[draggedCheckerIndex])
        {
            isDraggingFlags[draggedCheckerIndex] = false;
            if (moveCounts[draggedCheckerIndex] > 0)
            {
                moveCounts[draggedCheckerIndex]--;
                remainingMoves--;
                this->Text = String::Format("Backgammon - Dice: {0} | Moves left: {1}", currentDiceValue, remainingMoves);
            }
            draggedCheckerIndex = -1;
            this->Cursor = Cursors::Default;
            this->Invalidate();
        }
    }
};