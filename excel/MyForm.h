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
    List<bool>^ wasMovedFlags;  // Флаг, указывающий, был ли кружок перемещен
    int circleRadius;
    int draggedCircleIndex;

public:
    CircleForm()
    {
        this->Text = "Movable Circles (One-time move)";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(800, 600);

        circleRadius = 30;
        circleCenters = gcnew List<Point>();
        isDraggingFlags = gcnew List<bool>();
        wasMovedFlags = gcnew List<bool>();
        draggedCircleIndex = -1;

        // Calculate vertical spacing between circles
        int verticalSpacing = this->ClientSize.Height / 6;
        int horizontalMargin = 100;

        // Initialize 5 circles on the left side
        for (int i = 1; i <= 5; i++)
        {
            int x = horizontalMargin;
            int y = i * verticalSpacing;
            circleCenters->Add(Point(x, y));
            isDraggingFlags->Add(false);
            wasMovedFlags->Add(false);
        }

        // Initialize 5 circles on the right side
        for (int i = 1; i <= 5; i++)
        {
            int x = this->ClientSize.Width - horizontalMargin;
            int y = i * verticalSpacing;
            circleCenters->Add(Point(x, y));
            isDraggingFlags->Add(false);
            wasMovedFlags->Add(false);
        }

        // Set up event handlers
        this->MouseDown += gcnew MouseEventHandler(this, &CircleForm::Form_MouseDown);
        this->MouseMove += gcnew MouseEventHandler(this, &CircleForm::Form_MouseMove);
        this->MouseUp += gcnew MouseEventHandler(this, &CircleForm::Form_MouseUp);
        this->Paint += gcnew PaintEventHandler(this, &CircleForm::Form_Paint);
    }

private:
    void Form_Paint(Object^ sender, PaintEventArgs^ e)
    {
        Graphics^ g = e->Graphics;
        g->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::AntiAlias;

        // Draw all circles
        for (int i = 0; i < circleCenters->Count; i++)
        {
            // Кружки, которые уже перемещали, будут зелеными
            Color circleColor = wasMovedFlags[i] ? Color::Green :
                (i == draggedCircleIndex ? Color::Red : Color::Blue);
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
        }
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        // Check if mouse is inside any circle
        for (int i = 0; i < circleCenters->Count; i++)
        {
            // Проверяем, что кружок еще не был перемещен
            if (!wasMovedFlags[i])
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
        if (draggedCircleIndex != -1 && isDraggingFlags[draggedCircleIndex] && !wasMovedFlags[draggedCircleIndex])
        {
            circleCenters[draggedCircleIndex] = e->Location;
            this->Invalidate(); // Force redraw
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        if (draggedCircleIndex != -1)
        {
            isDraggingFlags[draggedCircleIndex] = false;
            // Помечаем кружок как перемещенный
            wasMovedFlags[draggedCircleIndex] = true;
            draggedCircleIndex = -1;
            this->Cursor = Cursors::Default;
            this->Invalidate(); // Обновляем отображение
        }
    }
};