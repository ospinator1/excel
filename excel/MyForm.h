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
    List<int>^ checkerPoints; // ������� ����� ��� ������ �����
    int checkerRadius;

    Random^ random;

    // Game state
    int currentPlayer;
    int player1Dice1, player1Dice2;
    int player2Dice1, player2Dice2;
    List<int>^ availableMoves;
    List<int>^ highlightedPoints;
    int selectedCheckerIndex;
    Point initialCheckerPosition; // ���������� ��� �������� ��������� ������� �����

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
    int moveCounter; // ������� ������������ �����
    List<int>^ checkerMoveCount; // ������� ����������� ��� ������ ����� � ������ ����

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
        checkerColors = gcnew List<Color>();
        checkerPoints = gcnew List<int>();
        availableMoves = gcnew List<int>();
        highlightedPoints = gcnew List<int>();
        selectedCheckerIndex = -1;
        random = gcnew Random();
        currentPlayer = 1;
        player1Dice1 = player1Dice2 = 0;
        player2Dice1 = player2Dice2 = 0;
        moveCounter = 0; // �������������� ������� ������������ �����

        // �������������� ������� ����������� ��� ������ �����
        checkerMoveCount = gcnew List<int>();

        InitializeUI();
        InitializeCheckers();

        // ������ ����� ���������� �� ������������
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
        rollDiceButton->Text = "������� ������";
        rollDiceButton->Size = System::Drawing::Size(120, 40);
        rollDiceButton->Location = System::Drawing::Point(
            this->ClientSize.Width / 2 - rollDiceButton->Width / 2,
            this->ClientSize.Height - 60);
        rollDiceButton->BackColor = Color::LightSkyBlue;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        endTurnButtonP1 = gcnew Button();
        endTurnButtonP1->Text = "��������� ���";
        endTurnButtonP1->Size = System::Drawing::Size(100, 40);
        endTurnButtonP1->Location = System::Drawing::Point(50, this->ClientSize.Height - 60);
        endTurnButtonP1->BackColor = Color::FromArgb(220, 220, 220);
        endTurnButtonP1->Font = gcnew System::Drawing::Font("Arial", 10, FontStyle::Bold);
        endTurnButtonP1->Click += gcnew EventHandler(this, &BackgammonForm::EndTurnButtonP1_Click);
        endTurnButtonP1->Enabled = false;
        this->Controls->Add(endTurnButtonP1);

        endTurnButtonP2 = gcnew Button();
        endTurnButtonP2->Text = "��������� ���";
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

    void AddCheckersForPoint(int pointIndex, int count, Color color)
    {
        Point position = GetPointPosition(pointIndex);
        int direction = (pointIndex < 12) ? -1 : 1; // ����� ��� ������, ���� ��� �������
        int spacing = 20; // ���������� ����� �������

        // ������� ��� ������������ ����� �� ������ �������
        int existingCount = 0;
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerPoints[i] == pointIndex)
            {
                existingCount++;
            }
        }

        // ���������� ����� ����� � ������ ��� ������������
        for (int i = 0; i < count; i++)
        {
            int y = position.Y + ((existingCount + i) * spacing * direction); // ����������� Y � ����������� �� ���������� ��� ������������ �����
            checkerPositions->Add(Point(position.X, y));
            checkerColors->Add(color);
            checkerPoints->Add(pointIndex);
            checkerMoveCount->Add(0); // ���������� ����� �� ������������
        }
    }

    Point GetPointPosition(int pointIndex)
    {
        int x, y;

        if (pointIndex < 12)
        {
            // ������ ������������ (1-12)
            x = boardOffsetX + (11 - pointIndex) * triangleWidth * 2 + triangleWidth;
            y = boardHeight - 30;
        }
        else
        {
            // ������� ������������ (13-24)
            x = boardOffsetX + (pointIndex - 12) * triangleWidth * 2 + triangleWidth;
            y = 70;
        }

        return Point(x, y);
    }

    void InitializeCheckers()
    {
        checkerPositions->Clear();
        checkerColors->Clear();
        checkerPoints->Clear();
        checkerMoveCount->Clear(); // ������� �������� �����������

        // Player 1 (dark) checkers
        AddCheckersForPoint(0, 2, Color::FromArgb(50, 50, 50));   // ����� 1
        AddCheckersForPoint(11, 5, Color::FromArgb(50, 50, 50));  // ����� 12
        AddCheckersForPoint(16, 3, Color::FromArgb(50, 50, 50));  // ����� 17
        AddCheckersForPoint(18, 5, Color::FromArgb(50, 50, 50));  // ����� 19

        // Player 2 (light) checkers
        AddCheckersForPoint(5, 5, Color::FromArgb(240, 240, 240));  // ����� 6
        AddCheckersForPoint(7, 3, Color::FromArgb(240, 240, 240));  // ����� 8
        AddCheckersForPoint(12, 5, Color::FromArgb(240, 240, 240)); // ����� 13
        AddCheckersForPoint(23, 2, Color::FromArgb(240, 240, 240)); // ����� 24
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        if (moveCounter >= 2) // ��������, ����� �� ������� ������
        {
            MessageBox::Show("�� ��� ����������� ��� �����. ��������� ���� ���.", "������", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        availableMoves->Clear(); // ������� ��������� ����� ����� ������� �������

        if (currentPlayer == 1)
        {
            player1Dice1 = random->Next(1, 7);
            player1Dice2 = random->Next(1, 7);
            if (player1Dice1 == player1Dice2) // ���������, ����� �� �����
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
            if (player2Dice1 == player2Dice2) // ���������, ����� �� �����
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

        this->Text = String::Format("Backgammon - Player {0}'s Turn | Moves: {1} and {2}",
            currentPlayer, availableMoves[0], availableMoves[1]);

        // ����������, ��� ��� �����
        if (availableMoves->Count > 2)
        {
            this->Text += " (Double!)";
        }

        rollDiceButton->Enabled = false; // ��������� ������ �������
        this->Invalidate();
    }

    void EndTurnButtonP1_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 2;
        ResetTurn();
        this->Text = "Backgammon - Player 2's Turn";
        this->Invalidate();
    }

    void EndTurnButtonP2_Click(Object^ sender, EventArgs^ e)
    {
        currentPlayer = 1;
        ResetTurn();
        this->Text = "Backgammon - Player 1's Turn";
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

        // ���������� ������� ������������ ����� � �������� ����������� ��� �����
        moveCounter = 0;
        for (int i = 0; i < checkerMoveCount->Count; i++)
        {
            checkerMoveCount[i] = 0; // ���������� ���� ����������� ������ �����
        }
    }

    void CalculatePossibleMoves(int checkerIndex)
    {
        highlightedPoints->Clear();

        int currentPoint = checkerPoints[checkerIndex];
        bool isPlayer1 = (checkerColors[checkerIndex].R < 100);

        for each(int move in availableMoves)
        {
            int targetPoint;
            if (isPlayer1)
            {
                // ����� 1 �������� �� ����������� (1->24)
                targetPoint = currentPoint + move;
            }
            else
            {
                // ����� 2 �������� �� �������� (24->1)
                targetPoint = currentPoint - move;
            }

            // ���������, ��� ����� ���������� (0-23)
            if (targetPoint >= 0 && targetPoint < 24)
            {
                // ���������, ����� �� ���������� ����� �� ������� �������
                int opponentCount = 0;
                Color opponentColor = isPlayer1 ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                    {
                        opponentCount++;
                    }
                }

                // ��������� ���������, ���� �� ������� ������� �� ����� ����� ����������
                if (opponentCount < 2)
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

        DrawBoard(g);
        DrawHomeArea(g); // ������ ����

        // ������ ������������ ������
        for each(int point in highlightedPoints)
        {
            Point pos = GetPointPosition(point);
            g->FillEllipse(Brushes::Yellow, pos.X - checkerRadius - 5, pos.Y - checkerRadius - 5,
                checkerRadius * 2 + 10, checkerRadius * 2 + 10);
        }

        // ������ �����
        for (int i = 0; i < checkerPositions->Count; i++)
        {
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

        // ������ ������
        if (currentPlayer == 1)
        {
            DrawDice(g, 100, 100, player1Dice1, Color::Black);
            DrawDice(g, 180, 100, player1Dice2, Color::Black);
        }
        else
        {
            DrawDice(g, this->ClientSize.Width - 220, 100, player2Dice1, Color::Red);
            DrawDice(g, this->ClientSize.Width - 140, 100, player2Dice2, Color::Red);
        }

        // ���������� �������� ������
        String^ turnText = String::Format("������� ���: ����� {0}", currentPlayer);
        g->DrawString(turnText, gcnew Drawing::Font("Arial", 16, FontStyle::Bold),
            Brushes::DarkBlue, PointF(this->ClientSize.Width / 2 - 100, 10));
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

    void DrawHomeArea(Graphics^ g)
    {
        // ������ ��� ������ 1 (������ �����)
        Color homeColor1 = Color::FromArgb(200, 220, 255);
        g->FillRectangle(gcnew SolidBrush(homeColor1), boardOffsetX, boardHeight - 30, boardWidth / 2, 30);
        g->DrawString("��� ������ 1", gcnew Font("Arial", 12), Brushes::Black, boardOffsetX + 10, boardHeight - 25);

        // ������ ��� ������ 2 (������� �����)
        Color homeColor2 = Color::FromArgb(255, 220, 220);
        g->FillRectangle(gcnew SolidBrush(homeColor2), boardOffsetX + boardWidth / 2, 50, boardWidth / 2, 30);
        g->DrawString("��� ������ 2", gcnew Font("Arial", 12), Brushes::Black,
            boardOffsetX + boardWidth / 2 + 10, 55);
    }

    void DrawPoint(Graphics^ g, int pointIndex, bool isTop)
    {
        int x = boardOffsetX + (pointIndex % 12) * triangleWidth * 2 + triangleWidth;
        int y = isTop ? 50 : 50 + triangleHeight;

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
            // ������� ������������ (13-24)
            int upperNumber = 13 + i;
            int upperX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(upperNumber.ToString(), font, brush, PointF(upperX, 50 - 25));

            // ������ ������������ (1-12)
            int lowerNumber = 12 - i;
            int lowerX = boardOffsetX + (i * triangleWidth * 2) + triangleWidth - 10;
            g->DrawString(lowerNumber.ToString(), font, brush, PointF(lowerX, 50 + boardHeight + 10));
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

        for (int i = 0; i < checkerPositions->Count; i++)
        {
            bool isPlayer1 = (checkerColors[i].R < 100);
            if ((currentPlayer == 1 && isPlayer1) || (currentPlayer == 2 && !isPlayer1))
            {
                int dx = e->X - checkerPositions[i].X;
                int dy = e->Y - checkerPositions[i].Y;
                if (dx * dx + dy * dy <= checkerRadius * checkerRadius)
                {
                    selectedCheckerIndex = i;
                    initialCheckerPosition = checkerPositions[i]; // ��������� ��������� �������
                    CalculatePossibleMoves(i);
                    this->Invalidate();
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        // ���������, ������� �� �����
        if (selectedCheckerIndex != -1)
        {
            // �������� ������� ������� �����
            Point currentPos = checkerPositions[selectedCheckerIndex];

            // ���������� ����� � ��������� ������� ���� � ���������� �������������
            currentPos.X = e->X - (checkerRadius); // ������� ��� �������������
            currentPos.Y = e->Y - (checkerRadius); // ������� ��� �������������

            // ��������� ������� �����
            checkerPositions[selectedCheckerIndex] = currentPos;

            // �������������� �����, ����� ���������� ���������
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (e->Button == System::Windows::Forms::MouseButtons::Right) // ���������, ������ ������ ����
        {
            // ���������� ����� �� ��������� �������
            if (selectedCheckerIndex != -1)
            {
                checkerPositions[selectedCheckerIndex] = initialCheckerPosition; // ���������� �� �������� �����
                selectedCheckerIndex = -1; // ���������� ��������� �����
                this->Invalidate(); // �������������� �����
            }
            return; // �������, ����� �� ��������� ���������� ��������
        }

        if (selectedCheckerIndex != -1 && highlightedPoints->Count > 0)
        {
            // ���������, ���� ����������� �����
            for each(int targetPoint in highlightedPoints)
            {
                Point targetPos = GetPointPosition(targetPoint);
                int dx = e->X - targetPos.X;
                int dy = e->Y - targetPos.Y;

                if (dx * dx + dy * dy <= checkerRadius * checkerRadius * 4)
                {
                    // ��������� ����� ����
                    int currentPoint = checkerPoints[selectedCheckerIndex];
                    int moveLength = Math::Abs(targetPoint - currentPoint);

                    // ���������, ��� ����� ��� ��������
                    if (availableMoves->Contains(moveLength))
                    {
                        // ������������ ���������� ����� �� ������� �������
                        int opponentCount = 0;
                        Color opponentColor = (checkerColors[selectedCheckerIndex].R < 100) ? Color::FromArgb(240, 240, 240) : Color::FromArgb(50, 50, 50);

                        // ������� ���������� ����� ���������� �� ������� �������
                        for (int i = 0; i < checkerPoints->Count; i++)
                        {
                            if (checkerPoints[i] == targetPoint && checkerColors[i] == opponentColor)
                            {
                                opponentCount++;
                            }
                        }

                        // ���������, ��� �� ������� ������� ����� ���������� �����
                        if (opponentCount < 2) // ���� 0 ��� 1 ���������, �� ����� �������
                        {
                            // ��������� ��������� �����
                            checkerPositions[selectedCheckerIndex] = targetPos;
                            checkerPoints[selectedCheckerIndex] = targetPoint;

                            // ����������� ������� ����������� ��� ������ �����
                            checkerMoveCount[selectedCheckerIndex]++;
                            moveCounter++; // ����������� ������� ������������ �����

                            // ������� �������������� ���
                            availableMoves->Remove(moveLength);

                            // ���������, ������ �� ����� � ������������
                            if (CanExitAllCheckers())
                            {
                                // ����� ����� ������ ����������� �����
                                this->Text = String::Format("����� {0} ����� ����������� �����. ������� '������� ������' ��� ������.", currentPlayer);
                            }

                            // ���������, ���� ���������� ������������ ���������� �����������
                            if (moveCounter >= 2)
                            {
                                // ��������� ����������� ����������� �����
                                rollDiceButton->Enabled = false;
                                this->Text = String::Format("Backgammon - Player {0}'s Turn | ��� ���� ���������.", currentPlayer);
                            }

                            break;
                        }
                    }
                }
            }

            selectedCheckerIndex = -1;
            highlightedPoints->Clear();
            this->Invalidate();
        }
    }

    bool CanExitAllCheckers()
    {
        for (int i = 0; i < checkerPoints->Count; i++)
        {
            if (checkerColors[i] == (currentPlayer == 1 ? Color::FromArgb(50, 50, 50) : Color::FromArgb(240, 240, 240)))
            {
                if (checkerPoints[i] >= 0 && checkerPoints[i] <= 5) // ���������, ��������� �� ����� � ����� ����
                    return false;
            }
        }
        return true; // ��� ����� � ����
    }

    void RemoveCheckersFromBoard()
    {
        // ������� ����� � �����, ������� ��������� � ���� � ������������� �������� ���������
        List<int>^ outgoingCheckers = gcnew List<int>();

        if (currentPlayer == 1)
        {
            // �������� ����� ��� ������ 1
            if (checkerPoints->Contains(player1Dice1 - 1)) // ���� ������ �������� ������� ������
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == player1Dice1 - 1 && checkerColors[i] == Color::FromArgb(50, 50, 50))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
            if (checkerPoints->Contains(player1Dice2 - 1)) // ���� ������ �������� ������� ������
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == player1Dice2 - 1 && checkerColors[i] == Color::FromArgb(50, 50, 50))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
        }
        else
        {
            // �������� ����� ��� ������ 2
            if (checkerPoints->Contains(24 - player2Dice1)) // ���� ������ �������� ������� ������
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == 24 - player2Dice1 && checkerColors[i] == Color::FromArgb(240, 240, 240))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
            if (checkerPoints->Contains(24 - player2Dice2)) // ���� ������ �������� ������� ������
            {
                for (int i = 0; i < checkerPoints->Count; i++)
                {
                    if (checkerPoints[i] == 24 - player2Dice2 && checkerColors[i] == Color::FromArgb(240, 240, 240))
                    {
                        outgoingCheckers->Add(i);
                    }
                }
            }
        }

        // ������� ��������� ����� �� ������
        for each(int index in outgoingCheckers)
        {
            checkerPositions->RemoveAt(index);
            checkerColors->RemoveAt(index);
            checkerPoints->RemoveAt(index);
        }
    }

    // ����� ���������� ���� ������, ������� ����� ���������� ������� ��� ������������ �����
    void CompleteTurn()
    {
        if (CanExitAllCheckers())
        {
            MessageBox::Show("����� ����� ������ ����������� �����!", "����������", MessageBoxButtons::OK, MessageBoxIcon::Information);
            // ����� ����������� �����
            RemoveCheckersFromBoard();
        }

        currentPlayer = (currentPlayer == 1) ? 2 : 1; // �������� ����
        ResetTurn(); // ����� ��������� ����
    }
};