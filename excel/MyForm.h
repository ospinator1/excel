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
    Button^ rollDiceButton;
    Random^ random;
    int currentDiceValue1;
    int currentDiceValue2;
    int totalMoves;

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

        // Инициализация общего количества ходов
        currentDiceValue1 = 0;
        currentDiceValue2 = 0;
        totalMoves = 0;

        // Инициализация кнопки для броска обоих кубиков
        rollDiceButton = gcnew Button();
        rollDiceButton->Text = "Roll Dice";
        rollDiceButton->Size = System::Drawing::Size(100, 50);
        rollDiceButton->Location = Point(this->ClientSize.Width - 160, this->ClientSize.Height - 70);
        rollDiceButton->BackColor = Color::LightGray;
        rollDiceButton->Font = gcnew System::Drawing::Font("Arial", 12);
        rollDiceButton->Click += gcnew EventHandler(this, &CircleForm::RollDiceButton_Click);
        this->Controls->Add(rollDiceButton);

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

    void RollDiceButton_Click(Object^ sender, EventArgs^ e)
    {
        // Бросок обоих кубиков
        currentDiceValue1 = random->Next(1, 7);
        currentDiceValue2 = random->Next(1, 7);
        totalMoves = currentDiceValue1 + currentDiceValue2; // Сумма ходов

        // Установка количества ходов для кругов
        for (int i = 0; i < moveCounts->Count; i++)
        {
            moveCounts[i] = totalMoves; // Установить общее количество ходов
        }

        this->Text = String::Format("Dice 1: {0} | Dice 2: {1} | Total Moves: {2}",
            currentDiceValue1,
            currentDiceValue2,
            totalMoves);

        this->Invalidate();
    }

    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

        String^ diceText1 = String::Format("Dice 1: {0}", currentDiceValue1);
        String^ diceText2 = String::Format("Dice 2: {0}", currentDiceValue2);
        String^ totalMovesText = String::Format("Total Moves: {0}", totalMoves);

        g->DrawString(diceText1, gcnew System::Drawing::Font("Arial", 14), Brushes::Black, 20, 20);
        g->DrawString(diceText2, gcnew System::Drawing::Font("Arial", 14), Brushes::Black, 20, 50);
        g->DrawString(totalMovesText, gcnew System::Drawing::Font("Arial", 14), Brushes::Black, 20, 80);

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
        if (totalMoves <= 0) return; // Проверка на доступность ходов

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
            circleCenters[draggedCircleIndex] = e->Location;
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
                totalMoves--; // Уменьшаем общее количество оставшихся ходов
                this->Text = String::Format("Dice 1: {0} | Dice 2: {1} | Total Moves: {2}",
                    currentDiceValue1,
                    currentDiceValue2,
                    totalMoves);
            }
            draggedCircleIndex = -1;
            this->Cursor = Cursors::Default;
            this->Invalidate();
        }
    }
};