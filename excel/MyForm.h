#pragma once

#include <Windows.h>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class CircleForm : public Form
{
private:
    Point circleCenter;
    int circleRadius;
    bool isDragging;

public:
    CircleForm()
    {
        this->Text = "Movable Circle";
        this->DoubleBuffered = true;
        this->ClientSize = System::Drawing::Size(800, 600);

        circleCenter = Point(this->ClientSize.Width / 2, this->ClientSize.Height / 2);
        circleRadius = 50;
        isDragging = false;

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

        // Draw circle
        g->FillEllipse(Brushes::Blue,
            circleCenter.X - circleRadius,
            circleCenter.Y - circleRadius,
            circleRadius * 2,
            circleRadius * 2);
        g->DrawEllipse(Pens::Black,
            circleCenter.X - circleRadius,
            circleCenter.Y - circleRadius,
            circleRadius * 2,
            circleRadius * 2);
    }

    void Form_MouseDown(Object^ sender, MouseEventArgs^ e)
    {
        // Check if mouse is inside the circle
        int dx = e->X - circleCenter.X;
        int dy = e->Y - circleCenter.Y;
        if (dx * dx + dy * dy <= circleRadius * circleRadius)
        {
            isDragging = true;
            this->Cursor = Cursors::Hand;
        }
    }

    void Form_MouseMove(Object^ sender, MouseEventArgs^ e)
    {
        if (isDragging)
        {
            circleCenter = e->Location;
            this->Invalidate(); // Force redraw
        }
    }

    void Form_MouseUp(Object^ sender, MouseEventArgs^ e)
    {
        isDragging = false;
        this->Cursor = Cursors::Default;
    }
};