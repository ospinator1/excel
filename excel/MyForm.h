#pragma once

#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkerPositions;
    List<bool>^ isDraggingFlags;
    List<Color>^ checkerColors;
    int checkerRadius;
    int draggedCheckerIndex;
    Random^ random;

    // Game state
    int currentPlayer; // 1 or 2
    int player1Dice1, player1Dice2;
    int player2Dice1, player2Dice2;
    int totalMoves;

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
    int x;
public:
    BackgammonForm()
    {
        this->Text = "Backgammon";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1200, 700);
        this->BackColor = Color::FromArgb(240, 240, 240);

        checkerRadius = 20;
        boardWidth = 700;
        boardHeight = 500;
        triangleWidth = boardWidth / 12;
        triangleHeight = boardHeight / 2;
        boardOffsetX = (this->ClientSize.Width - boardWidth) / 2;
        checkerPositions = gcnew List<Point>();
        isDraggingFlags = gcnew List<bool>();
        checkerColors = gcnew List<Color>();

        draggedCheckerIndex = -1;
        random = gcnew Random();

        currentPlayer = 1;
        player1Dice1 = player1Dice2 = 0;
        player2Dice1 = player2Dice2 = 0;
        totalMoves = 0;

        InitializeUI();
        InitializeCheckers();

        this->MouseDown += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &BackgammonForm::Form_Paint);
    }

private:
    void InitializeUI()
    {
        rollDiceButton = gcnew Button();
        rollDiceButton->Text = "Roll Dice";
        rollDiceButton->Size = System::Drawing::Size(120, 40);
        rollDiceButton->Location = System::Drawing::Point(
            this->ClientSize.Width / 2 - rollDiceButton->Width / 2,
            this->ClientSize.Height - 60);
        rollDiceButton->BackColor = Color::LightSkyBlue;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        endTurnButtonP1 = gcnew Button();
        endTurnButtonP1->Text = "End Turn";
        endTurnButtonP1->Size = System::Drawing::Size(100, 40);
        endTurnButtonP1->Location = System::Drawing::Point(50, this->ClientSize.Height - 60);
        endTurnButtonP1->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP1->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
        endTurnButtonP1->Enabled = false;
        this->Controls->Add(endTurnButtonP1);

        endTurnButtonP2 = gcnew Button();
        endTurnButtonP2->Text = "End Turn";
        endTurnButtonP2->Size = System::Drawing::Size(100, 40);
        endTurnButtonP2->Location = System::Drawing::Point(
            this->ClientSize.Width - 150,
            this->ClientSize.Height - 60);
        endTurnButtonP2->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP2->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP2->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP2_Click);
        endTurnButtonP2->Enabled = false;
        this->Controls->Add(endTurnButtonP2);
    }
    // �������� ��� ��������� � ������ ����� (��� � ���������)
    const int BOTTOM_OFFSET = -30;    // �������� ��� ������ �������������
    const int TOP_OFFSET = 70;       // �������� ��� ������� �������������
    const int CHECKER_SPACING = 20;  // ���������� ����� �������

    void AddCheckersForPoint(int pointIndex, int count, Color color)
    {
        // ������������ ������� X
        int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;

        // ������������� X ��� ������ ������������� (1-12)
        if (pointIndex < 12) {
            x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
        }

        // ������������ ���������
        int baseY, direction;

        if (pointIndex < 12) {
            // ������ ������������ - ����� ���� ����� �� ���������
            baseY = boardHeight - BOTTOM_OFFSET;
            direction = -1; // ������������� ����������� (�����)
        }
        else {
            // ������� ������������ - ����� ���� ���� �� ���������
            baseY = TOP_OFFSET;
            direction = 1; // ������������� ����������� (����)
        }

        // ��������� �����
        for (int i = 0; i < count; i++) {
            int y = baseY + (i * CHECKER_SPACING * direction);
            checkerPositions->Add(Point(x, y));
            isDraggingFlags->Add(false);
            checkerColors->Add(color);
        }
    }

    void InitializeCheckers()
    {
        checkerPositions->Clear();
        isDraggingFlags->Clear();
        checkerColors->Clear();

        // Player 1 (dark) checkers (������ �����)
        AddCheckersForPoint(0, 2, Color::FromArgb(50, 50, 50));   // ����� 1 (���)
        AddCheckersForPoint(11, 5, Color::FromArgb(50, 50, 50));  // ����� 12 (���)
        AddCheckersForPoint(16, 3, Color::FromArgb(50, 50, 50)); // ����� 17 (����)
        AddCheckersForPoint(18, 5, Color::FromArgb(50, 50, 50));  // ����� 19 (����)

        // Player 2 (light) checkers (����� �����)
        AddCheckersForPoint(5, 5, Color::FromArgb(240, 240, 240));  // ����� 6 (���)
        AddCheckersForPoint(7, 3, Color::FromArgb(240, 240, 240));  // ����� 8 (���)
        AddCheckersForPoint(12, 5, Color::FromArgb(240, 240, 240)); // ����� 13 (����)
        AddCheckersForPoint(23, 2, Color::FromArgb(240, 240, 240)); // ����� 24 (����)
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (currentPlayer == 1)
        {
            player1Dice1 = random->Next(1, 7);
            player1Dice2 = random->Next(1, 7);
            totalMoves = player1Dice1 + player1Dice2;
            endTurnButtonP1->Enabled = true;
            rollDiceButton->Enabled = false;
        }
        else
        {
            player2Dice1 = random->Next(1, 7);
            player2Dice2 = random->Next(1, 7);
            totalMoves = player2Dice1 + player2Dice2;
            endTurnButtonP2->Enabled = true;
            rollDiceButton->Enabled = false;
        }

        this->Text = String::Format("Backgammon - Player {0}'s Turn | Moves: {1}",
            currentPlayer, totalMoves);
        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 2;
        totalMoves = 0;
        endTurnButtonP1->Enabled = false;
        rollDiceButton->Enabled = true;
        this->Text = "Backgammon - Player 2's Turn";
        this->Invalidate();
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 1;
        totalMoves = 0;
        endTurnButtonP2->Enabled = false;
        rollDiceButton->Enabled = true;
        this->Text = "Backgammon - Player 1's Turn";
        this->Invalidate();
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;
        g->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAlias;

        DrawBoard(g);

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            Color color = checkerColors[i];
            if (i == draggedCheckerIndex) color = Color::FromArgb(180, color);

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

        if (player1Dice1 > 0 && player1Dice2 > 0 && currentPlayer == 1)
        {
            DrawDice(g, 100, 100, player1Dice1, Color::Black);
            DrawDice(g, 180, 100, player1Dice2, Color::Black);

            Drawing::Font^ playerFont = gcnew Drawing::Font("Arial", 14, FontStyle::Bold);
            g->DrawString("Player 1", playerFont, Brushes::Black, PointF(120, 60));
        }

        if (player2Dice1 > 0 && player2Dice2 > 0 && currentPlayer == 2)
        {
            DrawDice(g, this->ClientSize.Width - 220, 100, player2Dice1, Color::Red);
            DrawDice(g, this->ClientSize.Width - 140, 100, player2Dice2, Color::Red);

            Drawing::Font^ playerFont = gcnew Drawing::Font("Arial", 14, FontStyle::Bold);
            g->DrawString("Player 2", playerFont, Brushes::Black,
                PointF(this->ClientSize.Width - 180, 60));
        }

        Drawing::Font^ turnFont = gcnew Drawing::Font("Arial", 16, FontStyle::Bold);
        String^ turnText = String::Format("Current Turn: Player {0}", currentPlayer);
        g->DrawString(turnText, turnFont, Brushes::DarkBlue,
            PointF(this->ClientSize.Width / 2 - 100, 10));
    }

    void DrawBoard(Graphics^ g)
    {
        // ������-���������� ���� ��� ���� �����
        Color boardColor = Color::FromArgb(210, 180, 140); // ������-����������

        // �������� ����� �������� ����� ������-���������� ������
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX, 50, boardWidth, boardHeight);

        // �������� ������ �������� ����� ������-���������� ������
        g->FillRectangle(gcnew SolidBrush(boardColor), boardOffsetX + boardWidth, 50, boardWidth, boardHeight);

        // ������ ������� �����
        g->DrawRectangle(Pens::Black, boardOffsetX, 50, boardWidth, boardHeight);

        // ������ ����������� ������ (������)
        for (int i = 0; i < 12; i++)
        {
            DrawPoint(g, i, true);  // ������� ������������ (������ 1-12)
            DrawPoint(g, i, false); // ������ ������������ (������ 13-24)
        }

        // ������ ������ �������
        DrawPointNumbers(g);
    }
    void DrawPoint(Graphics^ g, int pointIndex, bool isTop)
    {
        // ����� ��� ������ ����� (7-12 � 13-18)
        int rightShift = triangleWidth/9; // ����� ������������ (��������, *1.5 ��� *2)

        int x;
        if (pointIndex < 6) {
            // ������������ 1-6 (����) � 19-24 (���) - ��� ������
            x = boardOffsetX + (pointIndex * triangleWidth * 2) + triangleWidth;
        }
        else {
            // ������������ 7-12 (����) � 13-18 (���) - ����� ������
            x = boardOffsetX + rightShift + (pointIndex * triangleWidth * 2) + triangleWidth;
        }

        int y = isTop ? 50 : 50 + triangleHeight;

        // ���������� ������� ������������
        array<Point>^ triangle = gcnew array<Point>(3);
        if (isTop)
        {
            // ������� ������������ (������ ����)
            triangle[0] = Point(x - triangleWidth, y);
            triangle[1] = Point(x, y + triangleHeight);
            triangle[2] = Point(x + triangleWidth, y);
        }
        else
        {
            // ������ ������������ (������ �����)
            triangle[0] = Point(x - triangleWidth, y + triangleHeight);
            triangle[1] = Point(x, y);
            triangle[2] = Point(x + triangleWidth, y + triangleHeight);
        }

        // �������� �����
        Color darkColor = Color::FromArgb(139, 69, 19); // Ҹ���-����������
        Color lightColor = Color::FromArgb(245, 222, 179); // ������-�������

        Brush^ pointBrush = ((pointIndex % 2 == 0) ^ isTop) ? gcnew SolidBrush(darkColor) : gcnew SolidBrush(lightColor);
        g->FillPolygon(pointBrush, triangle);
        g->DrawPolygon(Pens::Black, triangle);
    }
    void DrawPointNumbers(Graphics^ g)
    {
        Drawing::Font^ font = gcnew Drawing::Font("Arial", 10, FontStyle::Bold);
        Brush^ brush = Brushes::Black;

        // ������� ������������ (13-24 ����� �������)
        for (int i = 0; i < 12; i++)
        {
            int number = 13 + i;  // �������� � 13 � ���� �� 24
            int x = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            int y = 50 - 25; // ���� ������� �������������
            g->DrawString(number.ToString(), font, brush, PointF(x, y));
        }

        // ������ ������������ (1-12 ������ ������)
        for (int i = 0; i < 12; i++)
        {
            int number = 12 - i; // ��������� �������
            int x = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            int y = 50 + boardHeight + 10; // ���� ������ �������������
            g->DrawString(number.ToString(), font, brush, PointF(x, y));
        }
    }

    void DrawDice(Graphics^ g, int x, int y, int value, Color dotColor)
    {
        g->FillRectangle(gcnew SolidBrush(Color::FromArgb(30, 0, 0, 0)), x + 3, y + 3, 60, 60);
        Drawing::Rectangle diceRect(x, y, 60, 60);
        g->FillRectangle(Brushes::White, diceRect);
        g->DrawRectangle(Pens::Black, diceRect);

        Brush^ dotBrush = gcnew SolidBrush(dotColor);
        int dotSize = 10;
        int dotOffset = 15;
        int center = 30;

        switch (value)
        {
        case 1:
            g->FillEllipse(dotBrush, x + center - dotSize / 2, y + center - dotSize / 2, dotSize, dotSize);
            break;
        case 2:
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 3:
            g->FillEllipse(dotBrush, x + center - dotSize / 2, y + center - dotSize / 2, dotSize, dotSize);
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
            g->FillEllipse(dotBrush, x + center - dotSize / 2, y + center - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + dotOffset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 60 - dotOffset - dotSize, dotSize, dotSize);
            break;
        case 6:
            g->FillEllipse(dotBrush, x + dotOffset, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + center - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + dotOffset, y + 50, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 10, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + center - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + 60 - dotOffset - dotSize, y + 50, dotSize, dotSize);
            break;
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (totalMoves <= 0) return;

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            if ((currentPlayer == 1 && checkerColors[i].R < 100) ||
                (currentPlayer == 2 && checkerColors[i].R > 200))
            {
                int dx = e->X - checkerPositions[i].X;
                int dy = e->Y - checkerPositions[i].Y;
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
        if (draggedCheckerIndex != -1 && isDraggingFlags[draggedCheckerIndex])
        {
            checkerPositions[draggedCheckerIndex] = e->Location;
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCheckerIndex != -1 && isDraggingFlags[draggedCheckerIndex])
        {
            isDraggingFlags[draggedCheckerIndex] = false;
            if (totalMoves > 0)
            {
                totalMoves--;
                this->Text = String::Format("Backgammon - Player {0}'s Turn | Moves: {1}",
                    currentPlayer, totalMoves);
            }
            draggedCheckerIndex = -1;
            this->Cursor = Cursors::Default;
            this->Invalidate();
        }
    }
};