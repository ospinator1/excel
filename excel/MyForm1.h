#pragma once

#include <memory>
#include <vector>
#include "FurnitureData.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace RoomPlannerApp {

    public ref class MyForm : public System::Windows::Forms::Form {
    private:
        PictureBox^ roomPictureBox;
        Button^ addRoomButton;
        Button^ addFurnitureButton;
        Button^ rotateFurnitureButton;
        Button^ deleteButton;
        Button^ saveButton;
        OpenFileDialog^ openFileDialog;
        SaveFileDialog^ saveFileDialog;
        Panel^ drawingPanel;
        Panel^ controlPanel;
        Label^ titleLabel;

        Image^ roomImage;
        List<PictureBox^>^ furnitureItems;  // Список мебели
        bool isDragging;
        Point dragStartPoint;
        PictureBox^ draggedFurniture;
        bool isRoomSelected;

    public:
        MyForm();
        virtual ~MyForm();

    private:
        void InitializeComponent(void);
        void ScaleElements();
        void MyForm_Resize(System::Object^ sender, System::EventArgs^ e);
        void addRoomButton_Click(System::Object^ sender, System::EventArgs^ e);
        void addFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e);
        void Furniture_DoubleClick(Object^ sender, EventArgs^ e);
        void rotateFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e);
        void deleteButton_Click(System::Object^ sender, System::EventArgs^ e);
        void saveButton_Click(System::Object^ sender, System::EventArgs^ e);
        Bitmap^ CombineRoomAndFurniture();
        void roomPictureBox_Click(System::Object^ sender, System::EventArgs^ e);
        void RotateFurniture(PictureBox^ furniture, int angle);
        void Furniture_MouseDown(Object^ sender, MouseEventArgs^ e);
        void Furniture_MouseMove(Object^ sender, MouseEventArgs^ e);
        void Furniture_MouseUp(Object^ sender, MouseEventArgs^ e);
    };
}