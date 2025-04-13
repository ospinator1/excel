#pragma once

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <vector>
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
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
        Button^ rotateFurnitureButton;
        Button^ deleteButton; // Кнопка для удаления
        OpenFileDialog^ openFileDialog;
        Panel^ drawingPanel;

        Image^ roomImage;
        List<PictureBox^>^ furnitureItems;
        bool isDragging;
        Point dragStartPoint;
        PictureBox^ draggedFurniture;
        bool isRoomSelected; // Флаг, чтобы знать, выбрана ли комната

    public:
        MyForm(void)
        {
            InitializeComponent();
            furnitureItems = gcnew List<PictureBox^>();
            isDragging = false;
            isRoomSelected = false; // Комната изначально не выбрана
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
            this->rotateFurnitureButton = (gcnew System::Windows::Forms::Button());
            this->deleteButton = (gcnew System::Windows::Forms::Button());
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
            this->roomPictureBox->Click += gcnew System::EventHandler(this, &MyForm::roomPictureBox_Click);

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

            // rotateFurnitureButton
            this->rotateFurnitureButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->rotateFurnitureButton->Location = System::Drawing::Point(150, 12);
            this->rotateFurnitureButton->Name = L"rotateFurnitureButton";
            this->rotateFurnitureButton->Size = System::Drawing::Size(90, 40);
            this->rotateFurnitureButton->TabIndex = 2;
            this->rotateFurnitureButton->Text = L"Повернуть";
            this->rotateFurnitureButton->UseVisualStyleBackColor = true;
            this->rotateFurnitureButton->Click += gcnew System::EventHandler(this, &MyForm::rotateFurnitureButton_Click);

            // deleteButton
            this->deleteButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->deleteButton->Location = System::Drawing::Point(236, 12);
            this->deleteButton->Name = L"deleteButton";
            this->deleteButton->Size = System::Drawing::Size(80, 40);
            this->deleteButton->TabIndex = 3;
            this->deleteButton->Text = L"Удалить";
            this->deleteButton->UseVisualStyleBackColor = true;
            this->deleteButton->Click += gcnew System::EventHandler(this, &MyForm::deleteButton_Click);

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
            this->drawingPanel->TabIndex = 4;

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(784, 561);
            this->Controls->Add(this->deleteButton);
            this->Controls->Add(this->rotateFurnitureButton);
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
                    isRoomSelected = true; // Отмечаем, что комната выбрана
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка загрузки изображения комнаты: " + ex->Message, "Ошибка",
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
                    newFurniture->Tag = 0; // 0 означает угол не повернут

                    // Make furniture draggable
                    newFurniture->MouseDown += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseDown);
                    newFurniture->MouseMove += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseMove);
                    newFurniture->MouseUp += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseUp);

                    drawingPanel->Controls->Add(newFurniture);
                    newFurniture->BringToFront();
                    furnitureItems->Add(newFurniture);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка загрузки изображения мебели: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void rotateFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (draggedFurniture != nullptr && draggedFurniture != roomPictureBox) {
                int currentAngle = safe_cast<int>(draggedFurniture->Tag);
                currentAngle += 90;
                if (currentAngle >= 360) {
                    currentAngle = 0;
                }
                draggedFurniture->Tag = currentAngle;
                RotateFurniture(draggedFurniture, currentAngle);
            }
            else {
                MessageBox::Show("Пожалуйста, выберите элемент мебели, который хотите повернуть.", "Ошибка выбора",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (draggedFurniture != nullptr) {
                if (draggedFurniture == roomPictureBox) {
                    // Удаление комнаты
                    roomPictureBox->Image = nullptr;
                    roomImage = nullptr;
                    isRoomSelected = false; // Снимаем флаг, что комната выбрана
                }
                else {
                    // Удаление мебели
                    drawingPanel->Controls->Remove(draggedFurniture);
                    furnitureItems->Remove(draggedFurniture);
                }
                draggedFurniture = nullptr;
            }
            else {
                MessageBox::Show("Пожалуйста, выберите объект для удаления.", "Ошибка выбора",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void roomPictureBox_Click(System::Object^ sender, System::EventArgs^ e) {
            // При клике на roomPictureBox устанавливаем draggedFurniture на roomPictureBox
            // Это позволяет нам удалять комнату
            draggedFurniture = roomPictureBox;
        }

        void RotateFurniture(PictureBox^ furniture, int angle) {
            if (furniture->Image == nullptr) return;

            Bitmap^ original = gcnew Bitmap(furniture->Image);
            Bitmap^ rotated = gcnew Bitmap(original->Width, original->Height);
            rotated->SetResolution(original->HorizontalResolution, original->VerticalResolution);

            Graphics^ g = Graphics::FromImage(rotated);
            g->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;

            PointF center = PointF(original->Width / 2.0f, original->Height / 2.0f);
            g->TranslateTransform(center.X, center.Y);
            g->RotateTransform(angle);
            g->TranslateTransform(-center.X, -center.Y);
            g->DrawImage(original, 0, 0);

            delete g;
            furniture->Image = rotated;
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
            }
        }
    };
}