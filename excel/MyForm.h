// BackgammonGame.cpp

#include <Windows.h>
#include <stdlib.h>
#include <time.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;
using namespace System::Drawing::Drawing2D;

public ref class BackgammonForm : public Form
{
private:
    List<Point>^ checkers;
    List<bool>^ isDraggingFlags;
    int checkerRadius;
    int draggedCheckerIndex;
    Button^ rollDiceButton;
    Random^ random;
    int currentDiceValue1;
    int currentDiceValue2;
    int totalMoves;
    array<Rectangle>^ points;

public:
    BackgammonForm()
    {
        this->Text = "�����";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(1000, 600);
        this->BackColor = Color::SandyBrown;

        checkerRadius = 20;
        draggedCheckerIndex = -1;
        random = gcnew Random();
        checkers = gcnew List<Point>();
        isDraggingFlags = gcnew List<bool>();

        points = gcnew array<Rectangle>(24);
        InitializeBoardPoints();

        rollDiceButton = gcnew Button();
        rollDiceButton->Text = "������� �����";
        rollDiceButton->Size = System::Drawing::Size(120, 50);
        rollDiceButton->Location = Point(this->ClientSize.Width - 140, this->ClientSize.Height - 70);
        rollDiceButton->BackColor = Color::LightGray;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 12, FontStyle::Bold);
        rollDiceButton->Click += gcnew EventHandler(this, &BackgammonForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

        InitializeCheckers();

        this->MouseDown += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &BackgammonForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &BackgammonForm::Form_Paint);
    }

private:
    void InitializeBoardPoints()
    {
        int boardWidth = this->ClientSize.Width;
        int boardHeight = this->ClientSize.Height;
        int pointWidth = boardWidth / 12;
        int pointHeight = boardHeight / 2 - 60;

        // ������ ����� (1-12)
        for (int i = 0; i < 12; i++)
        {
            points[i] = Rectangle(
                i * pointWidth + 5,
                boardHeight / 2 + 30,
                pointWidth - 10,
                pointHeight
            );
        }

        // ������� ����� (13-24)
        for (int i = 0; i < 12; i++)
        {
            points[23 - i] = Rectangle(
                i * pointWidth + 5,
                30,
                pointWidth - 10,
                pointHeight
            );
        }
    }

    void InitializeCheckers()
    {
        // ����������� ��������� ����������� ��� ����
        // ������ ����� (����� 1)
        AddCheckersToPoint(0, 2, false);   // ����� 24
        AddCheckersToPoint(11, 5, false);  // ����� 13
        AddCheckersToPoint(16, 3, false);  // ����� 8
        AddCheckersToPoint(18, 5, false);  // ����� 6

        // ����� ����� (����� 2)
        AddCheckersToPoint(23, 2, true);  // ����� 1
        AddCheckersToPoint(12, 5, true);   // ����� 12
        AddCheckersToPoint(7, 3, true);    // ����� 17
        AddCheckersToPoint(5, 5, true);    // ����� 19
    }

    void AddCheckersToPoint(int pointIndex, int count, bool isWhite)
    {
        Rectangle point = points[pointIndex];
        int startY = point.Y;
        int step = checkerRadius * 2;

        if (pointIndex < 12) // ������ �������� �����
        {
            startY = point.Y - step;
            for (int i = 0; i < count; i++)
            {
                int x = point.X + point.Width / 2;
                int y = startY - i * step;
                checkers->Add(Point(x, y));
                isDraggingFlags->Add(false);
            }
        }
        else // ������� �������� �����
        {
            startY = point.Y + point.Height;
            for (int i = 0; i < count; i++)
            {
                int x = point.X + point.Width / 2;
                int y = startY + i * step;
                checkers->Add(Point(x, y));
                isDraggingFlags->Add(false);
            }
        }
    }

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        currentDiceValue1 = random->Next(1, 7);
        currentDiceValue2 = random->Next(1, 7);
        totalMoves = currentDiceValue1 + currentDiceValue2;

        this->Text = String::Format("����� - �����: {0} � {1} | �����: {2}",
            currentDiceValue1, currentDiceValue2, totalMoves);
        this->Invalidate();
    }

    void DrawBoard(Graphics^ g)
    {
        int boardWidth = this->ClientSize.Width;
        int boardHeight = this->ClientSize.Height;

        // ��� �����
        LinearGradientBrush^ boardBrush = gcnew LinearGradientBrush(
            Point(0, 0),
            Point(boardWidth, boardHeight),
            Color::SaddleBrown,
            Color::Peru);
        g->FillRectangle(boardBrush, 0, 0, boardWidth, boardHeight);

        // ������ ������������ (������)
        for (int i = 0; i < 24; i++)
        {
            Brush^ pointBrush;
            if (i < 12)
                pointBrush = gcnew LinearGradientBrush(
                    points[i], Color::Maroon, Color::DarkRed, LinearGradientMode::Vertical);
            else
                pointBrush = gcnew LinearGradientBrush(
                    points[i], Color::LightYellow, Color::Goldenrod, LinearGradientMode::Vertical);

            array<Point>^ triangle = gcnew array<Point>(3);
            if (i < 12) // ������ ������������
            {
                triangle[0] = Point(points[i].X, points[i].Y + points[i].Height);
                triangle[1] = Point(points[i].X + points[i].Width, points[i].Y + points[i].Height);
                triangle[2] = Point(points[i].X + points[i].Width / 2, points[i].Y);
            }
            else // ������� ������������
            {
                triangle[0] = Point(points[i].X, points[i].Y);
                triangle[1] = Point(points[i].X + points[i].Width, points[i].Y);
                triangle[2] = Point(points[i].X + points[i].Width / 2, points[i].Y + points[i].Height);
            }

            g->FillPolygon(pointBrush, triangle);
            g->DrawPolygon(Pens::Black, triangle);

            // ������ �������
            StringFormat^ format = gcnew StringFormat();
            format->Alignment = StringAlignment::Center;
            format->LineAlignment = StringAlignment::Near;

            String^ pointNumber = (i + 1).ToString();
            Font^ font = gcnew Font("Arial", 10, FontStyle::Bold);

            if (i < 12)
                g->DrawString(pointNumber, font, Brushes::White,
                    points[i].X + points[i].Width / 2, points[i].Y + points[i].Height - 25, format);
            else
                g->DrawString(pointNumber, font, Brushes::Black,
                    points[i].X + points[i].Width / 2, points[i].Y + 10, format);
        }

        // ��� (�������� �����)
        g->FillRectangle(Brushes::Tan, boardWidth / 2 - 20, 0, 40, boardHeight);
        g->DrawRectangle(Pens::Black, boardWidth / 2 - 20, 0, 40, boardHeight);
    }

    void DrawCheckers(Graphics^ g)
    {
        for (int i = 0; i < checkers->Count; i++)
        {
            Brush^ checkerBrush;
            if (i < 15) // ������ �����
            {
                LinearGradientBrush^ blackBrush = gcnew LinearGradientBrush(
                    Point(checkers[i].X - checkerRadius, checkers[i].Y - checkerRadius),
                    Point(checkers[i].X + checkerRadius, checkers[i].Y + checkerRadius),
                    Color::DimGray,
                    Color::Black);
                checkerBrush = blackBrush;
            }
            else // ����� �����
            {
                LinearGradientBrush^ whiteBrush = gcnew LinearGradientBrush(
                    Point(checkers[i].X - checkerRadius, checkers[i].Y - checkerRadius),
                    Point(checkers[i].X + checkerRadius, checkers[i].Y + checkerRadius),
                    Color::WhiteSmoke,
                    Color::Silver);
                checkerBrush = whiteBrush;
            }

            g->FillEllipse(checkerBrush,
                checkers[i].X - checkerRadius,
                checkers[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);
            g->DrawEllipse(Pens::Black,
                checkers[i].X - checkerRadius,
                checkers[i].Y - checkerRadius,
                checkerRadius * 2,
                checkerRadius * 2);
        }
    }

    void DrawDice(Graphics^ g)
    {
        if (currentDiceValue1 > 0)
            DrawDie(g, currentDiceValue1, this->ClientSize.Width - 200, 30);

        if (currentDiceValue2 > 0)
            DrawDie(g, currentDiceValue2, this->ClientSize.Width - 120, 30);
    }

    void DrawDie(Graphics^ g, int value, int x, int y)
    {
        int size = 60;
        g->FillRectangle(Brushes::White, x, y, size, size);
        g->DrawRectangle(Pens::Black, x, y, size, size);

        Brush^ dotBrush = Brushes::Black;
        int dotSize = 10;
        int offset = size / 4;

        switch (value)
        {
        case 1:
            g->FillEllipse(dotBrush, x + size / 2 - dotSize / 2, y + size / 2 - dotSize / 2, dotSize, dotSize);
            break;
        case 2:
            g->FillEllipse(dotBrush, x + offset, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size - offset - dotSize, dotSize, dotSize);
            break;
        case 3:
            g->FillEllipse(dotBrush, x + offset, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size / 2 - dotSize / 2, y + size / 2 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size - offset - dotSize, dotSize, dotSize);
            break;
        case 4:
            g->FillEllipse(dotBrush, x + offset, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + offset, y + size - offset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size - offset - dotSize, dotSize, dotSize);
            break;
        case 5:
            g->FillEllipse(dotBrush, x + offset, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size / 2 - dotSize / 2, y + size / 2 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + offset, y + size - offset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size - offset - dotSize, dotSize, dotSize);
            break;
        case 6:
            g->FillEllipse(dotBrush, x + offset, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + offset, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + offset, y + size / 2 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size / 2 - dotSize / 2, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + offset, y + size - offset - dotSize, dotSize, dotSize);
            g->FillEllipse(dotBrush, x + size - offset - dotSize, y + size - offset - dotSize, dotSize, dotSize);
            break;
        }
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = SmoothingMode::AntiAlias;

        DrawBoard(g);
        DrawCheckers(g);
        DrawDice(g);
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (totalMoves <= 0) return;

        for (int i = 0; i < checkers->Count; i++)
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

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCheckerIndex != -1 && isDraggingFlags[draggedCheckerIndex])
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
            draggedCheckerIndex = -1;
            this->Cursor = Cursors::Default;

            if (totalMoves > 0)
            {
                totalMoves--;
                this->Text = String::Format("����� - �����: {0} � {1} | �������� �����: {2}",
                    currentDiceValue1, currentDiceValue2, totalMoves);
            }

            this->Invalidate();
        }
    }
};

