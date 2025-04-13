#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <vector>
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections; // Required for ArrayList (though List is preferred)
using namespace System::Collections::Generic; // Required for List
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::IO;

namespace RoomPlannerApp {

    public ref class MyForm : public System::Windows::Forms::Form
    {
    private:
        PictureBox^ roomPictureBox;
        Button^ addRoomButton;
        Button^ addFurnitureButton;
        OpenFileDialog^ openFileDialog;
        Panel^ drawingPanel;

        Image^ roomImage;
        List<PictureBox^>^ furnitureItems;
        bool isDragging;
        Point dragStartPoint;
        PictureBox^ draggedFurniture;

    public:
        MyForm(void)
        {
            InitializeComponent();
            furnitureItems = gcnew List<PictureBox^>();
            isDragging = false;
        }

    protected:
        ~MyForm()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->roomPictureBox = (gcnew System::Windows::Forms::PictureBox());
            this->addRoomButton = (gcnew System::Windows::Forms::Button());
            this->addFurnitureButton = (gcnew System::Windows::Forms::Button());
            this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
            this->drawingPanel = (gcnew System::Windows::Forms::Panel());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->roomPictureBox))->BeginInit();
            this->drawingPanel->SuspendLayout();
            this->SuspendLayout();

            // roomPictureBox
            this->roomPictureBox->BackColor = System::Drawing::SystemColors::ControlDark;
            this->roomPictureBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->roomPictureBox->Location = System::Drawing::Point(50, 50);
            this->roomPictureBox->Name = L"roomPictureBox";
            this->roomPictureBox->Size = System::Drawing::Size(600, 400);
            this->roomPictureBox->TabStop = false;

            // addRoomButton
            this->addRoomButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->addRoomButton->Location = System::Drawing::Point(12, 12);
            this->addRoomButton->Name = L"addRoomButton";
            this->addRoomButton->Size = System::Drawing::Size(40, 40);
            this->addRoomButton->TabIndex = 0;
            this->addRoomButton->Text = L"+";
            this->addRoomButton->UseVisualStyleBackColor = true;
            this->addRoomButton->Click += gcnew System::EventHandler(this, &MyForm::addRoomButton_Click);

            // addFurnitureButton
            this->addFurnitureButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->addFurnitureButton->Location = System::Drawing::Point(58, 12);
            this->addFurnitureButton->Name = L"addFurnitureButton";
            this->addFurnitureButton->Size = System::Drawing::Size(40, 40);
            this->addFurnitureButton->TabIndex = 1;
            this->addFurnitureButton->Text = L"+";
            this->addFurnitureButton->UseVisualStyleBackColor = true;
            this->addFurnitureButton->Click += gcnew System::EventHandler(this, &MyForm::addFurnitureButton_Click);

            // openFileDialog
            this->openFileDialog->FileName = L"openFileDialog1";
            this->openFileDialog->Filter = L"Image Files|*.jpg;*.jpeg;*.png;*.bmp";

            // drawingPanel
            this->drawingPanel->AutoScroll = true;
            this->drawingPanel->Controls->Add(this->roomPictureBox);
            this->drawingPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->drawingPanel->Location = System::Drawing::Point(0, 0);
            this->drawingPanel->Name = L"drawingPanel";
            this->drawingPanel->Size = System::Drawing::Size(784, 561);
            this->drawingPanel->TabIndex = 2;

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(784, 561);
            this->Controls->Add(this->addFurnitureButton);
            this->Controls->Add(this->addRoomButton);
            this->Controls->Add(this->drawingPanel);
            this->Name = L"MyForm";
            this->Text = L"Room Planner 2D";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->roomPictureBox))->EndInit();
            this->drawingPanel->ResumeLayout(false);
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        void addRoomButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    roomImage = Image::FromFile(openFileDialog->FileName);
                    roomPictureBox->Image = roomImage;
                    roomPictureBox->SizeMode = PictureBoxSizeMode::Zoom;
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Error loading image: " + ex->Message, "Error",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void addFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    Image^ furnitureImage = Image::FromFile(openFileDialog->FileName);

                    PictureBox^ newFurniture = gcnew PictureBox();
                    newFurniture->Image = furnitureImage;
                    newFurniture->Size = System::Drawing::Size(100, 100);
                    newFurniture->SizeMode = PictureBoxSizeMode::Zoom;
                    newFurniture->BackColor = Color::Transparent;
                    newFurniture->Location = Point(100, 100);
                    newFurniture->BorderStyle = BorderStyle::FixedSingle;

                    // Make furniture draggable
                    newFurniture->MouseDown += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseDown);
                    newFurniture->MouseMove += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseMove);
                    newFurniture->MouseUp += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseUp);

                    drawingPanel->Controls->Add(newFurniture);
                    newFurniture->BringToFront();
                    furnitureItems->Add(newFurniture);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Error loading furniture image: " + ex->Message, "Error",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void Furniture_MouseDown(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                isDragging = true;
                draggedFurniture = (PictureBox^)sender;
                dragStartPoint = Point(e->X, e->Y);
                draggedFurniture->BringToFront();
            }
        }

        void Furniture_MouseMove(Object^ sender, MouseEventArgs^ e) {
            if (isDragging && draggedFurniture != nullptr) {
                Point newLocation = draggedFurniture->Location;
                newLocation.X += e->X - dragStartPoint.X;
                newLocation.Y += e->Y - dragStartPoint.Y;
                draggedFurniture->Location = newLocation;
            }
        }

        void Furniture_MouseUp(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                isDragging = false;
                draggedFurniture = nullptr;
            }
        }
    };
}