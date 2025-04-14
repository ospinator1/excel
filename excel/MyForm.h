#pragma once

#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Collections::Generic;

public ref class CircleForm : public Form
{
private:
    List<Point>^ circleCenters;
    List<bool>^ isDraggingFlags;
    List<int>^ moveCounts;
    int circleRadius;
    int draggedCircleIndex;
    Button^ diceButton;
    Random^ random;
    int currentDiceValue;
    int remainingMoves;

public:
    CircleForm()
    {
        this->Text = "Dice-Controlled Circles";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(800, 600);

        circleRadius = 30;

        // Инициализация коллекций
        circleCenters = gcnew List<Point>();
        isDraggingFlags = gcnew List<bool>();
        moveCounts = gcnew List<int>();

        draggedCircleIndex = -1;
        random = gcnew Random();
        currentDiceValue = 0;
        remainingMoves = 0;

        // Инициализация кнопки для броска кубика
        diceButton = gcnew Button();
        diceButton->Text = "Roll Dice";
        diceButton->Size = System::Drawing::Size(100, 50);
        diceButton->Location = Point(this->ClientSize.Width - 120, this->ClientSize.Height - 70);
        diceButton->BackColor = Color::LightGray;
        diceButton->Font = gcnew System::Drawing::Font("Arial", 12);
        diceButton->Click += gcnew EventHandler(this, &CircleForm::DiceButton_Click);
        this->Controls->Add(diceButton);

        // Инициализация кругов
        InitializeCircles();

        // Настройка обработчиков событий
        this->MouseDown += gcnew MouseEventHandler(this, &CircleForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &CircleForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &CircleForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &CircleForm::Form_Paint);
    }

private:
    void InitializeCircles()
    {
        int verticalSpacing = this->ClientSize.Height / 6;
        int horizontalMargin = 100;

        // Левые круги
        for (int i = 1; i <= 5; i++)
        {
            circleCenters->Add(Point(horizontalMargin, i * verticalSpacing));
            isDraggingFlags->Add(false);
            moveCounts->Add(0);
        }

        // Правые круги
        for (int i = 1; i <= 5; i++)
        {
            circleCenters->Add(Point(this->ClientSize.Width - horizontalMargin, i * verticalSpacing));
            isDraggingFlags->Add(false);
            moveCounts->Add(0);
        }
    }

    void DiceButton_Click(Object^ sender, EventArgs^ e)
    {
        currentDiceValue = random->Next(1, 7);
        remainingMoves = currentDiceValue;
        this->Text = String::Format("Dice: {0} | Moves left: {1}", currentDiceValue, remainingMoves);

        for (int i = 0; i < moveCounts->Count; i++)
        {
            moveCounts[i] = currentDiceValue;
        }

        this->Invalidate();
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

        String^ diceText = String::Format("Dice: {0}", currentDiceValue);
        g->DrawString(diceText, gcnew System::Drawing::Font("Arial", 14), Brushes::Black, 20, 20);

        String^ movesText = String::Format("Moves left: {0}", remainingMoves);
        g->DrawString(movesText, gcnew System::Drawing::Font("Arial", 14), Brushes::Black, 20, 50);

        for (int i = 0; i < circleCenters->Count; i++)
        {
            Color circleColor = moveCounts[i] > 0 ? Color::Blue : Color::Gray;
            if (i == draggedCircleIndex) circleColor = Color::Red;

            Brush^ brush = gcnew SolidBrush(circleColor);

            g->FillEllipse(brush,
                circleCenters[i].X - circleRadius,
                circleCenters[i].Y - circleRadius,
                circleRadius * 2,
                circleRadius * 2);

            g->DrawEllipse(Pens::Black,
                circleCenters[i].X - circleRadius,
                circleCenters[i].Y - circleRadius,
                circleRadius * 2,
                circleRadius * 2);

            String^ moveCountText = moveCounts[i].ToString();
            System::Drawing::Font^ font = gcnew System::Drawing::Font("Arial", 10);
            SizeF textSize = g->MeasureString(moveCountText, font);
            g->DrawString(moveCountText, font, Brushes::White,
                circleCenters[i].X - textSize.Width / 2,
                circleCenters[i].Y - textSize.Height / 2);
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        if (remainingMoves <= 0) return;

        for (int i = 0; i < circleCenters->Count; i++)
        {
            if (moveCounts[i] > 0)
            {
                int dx = e->X - circleCenters[i].X;
                int dy = e->Y - circleCenters[i].Y;
                if (dx * dx + dy * dy <= circleRadius * circleRadius)
                {
                    isDraggingFlags[i] = true;
                    draggedCircleIndex = i;
                    this->Cursor = Cursors::Hand;
                    break;
                }
            }
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCircleIndex != -1 &&
            isDraggingFlags[draggedCircleIndex] &&
            moveCounts[draggedCircleIndex] > 0)
        {
            circleCenters[draggedCircleIndex] = e->Location; // Это правильный способ
            this->Invalidate();
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCircleIndex != -1 && isDraggingFlags[draggedCircleIndex])
        {
            isDraggingFlags[draggedCircleIndex] = false;
            if (moveCounts[draggedCircleIndex] > 0)
            {
                moveCounts[draggedCircleIndex]--;
                remainingMoves--;
                this->Text = String::Format("Dice: {0} | Moves left: {1}", currentDiceValue, remainingMoves);
            }
            draggedCircleIndex = -1;
            this->Cursor = Cursors::Default;
            this->Invalidate();
        }
    }
};