#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Collections::Generic;
using namespace System::Windows::Forms;
using namespace System::Drawing;

namespace RoomPlannerApp {

    // Класс для данных о мебели
    [Serializable]
    public ref class FurnitureData {
    public:
        String^ ImagePath;
        Point Location;
        int RotationAngle;
        bool IsLocked;
        System::Drawing::Size Size;  // Используем System::Drawing::Size

        // Конструктор класса
        FurnitureData(String^ path, Point loc, int angle, bool locked, System::Drawing::Size size) {
            ImagePath = path;
            Location = loc;
            RotationAngle = angle;
            IsLocked = locked;
            Size = size;
        }
    };

    public ref class MyForm : public System::Windows::Forms::Form
    {
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
        MyForm(void)
        {
            InitializeComponent();
            furnitureItems = gcnew List<PictureBox^>();
            isDragging = false;
            isRoomSelected = false;
            this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
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
            this->saveButton = (gcnew System::Windows::Forms::Button());
            this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
            this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
            this->drawingPanel = (gcnew System::Windows::Forms::Panel());
            this->controlPanel = (gcnew System::Windows::Forms::Panel());
            this->titleLabel = (gcnew System::Windows::Forms::Label());
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->roomPictureBox))->BeginInit();
            this->drawingPanel->SuspendLayout();
            this->controlPanel->SuspendLayout();
            this->SuspendLayout();

            // roomPictureBox
            this->roomPictureBox->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(240)),
                static_cast<System::Int32>(static_cast<System::Byte>(240)),
                static_cast<System::Int32>(static_cast<System::Byte>(240)));
            this->roomPictureBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
            this->roomPictureBox->Location = System::Drawing::Point(0, 0);
            this->roomPictureBox->Size = System::Drawing::Size(700, 500);
            this->roomPictureBox->SizeMode = System::Windows::Forms::PictureBoxSizeMode::Zoom;
            this->roomPictureBox->TabStop = false;
            this->roomPictureBox->Click += gcnew System::EventHandler(this, &MyForm::roomPictureBox_Click);

            // addRoomButton
            this->addRoomButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
                static_cast<System::Int32>(static_cast<System::Byte>(130)),
                static_cast<System::Int32>(static_cast<System::Byte>(180)));
            this->addRoomButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->addRoomButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->addRoomButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->addRoomButton->Location = System::Drawing::Point(10, 50);
            this->addRoomButton->Size = System::Drawing::Size(150, 55);
            this->addRoomButton->TabIndex = 0;
            this->addRoomButton->Text = L"Добавить комнату";
            this->addRoomButton->UseVisualStyleBackColor = false;
            this->addRoomButton->Click += gcnew System::EventHandler(this, &MyForm::addRoomButton_Click);

            // addFurnitureButton
            this->addFurnitureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
                static_cast<System::Int32>(static_cast<System::Byte>(130)),
                static_cast<System::Int32>(static_cast<System::Byte>(180)));
            this->addFurnitureButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->addFurnitureButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->addFurnitureButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->addFurnitureButton->Location = System::Drawing::Point(170, 50);
            this->addFurnitureButton->Size = System::Drawing::Size(150, 40);
            this->addFurnitureButton->TabIndex = 1;
            this->addFurnitureButton->Text = L"Добавить мебель";
            this->addFurnitureButton->UseVisualStyleBackColor = false;
            this->addFurnitureButton->Click += gcnew System::EventHandler(this, &MyForm::addFurnitureButton_Click);

            // rotateFurnitureButton
            this->rotateFurnitureButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)),
                static_cast<System::Int32>(static_cast<System::Byte>(130)),
                static_cast<System::Int32>(static_cast<System::Byte>(180)));
            this->rotateFurnitureButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->rotateFurnitureButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->rotateFurnitureButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->rotateFurnitureButton->Location = System::Drawing::Point(330, 50);
            this->rotateFurnitureButton->Size = System::Drawing::Size(120, 40);
            this->rotateFurnitureButton->TabIndex = 2;
            this->rotateFurnitureButton->Text = L"Повернуть";
            this->rotateFurnitureButton->UseVisualStyleBackColor = false;
            this->rotateFurnitureButton->Click += gcnew System::EventHandler(this, &MyForm::rotateFurnitureButton_Click);

            // deleteButton
            this->deleteButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(220)),
                static_cast<System::Int32>(static_cast<System::Byte>(53)),
                static_cast<System::Int32>(static_cast<System::Byte>(69)));
            this->deleteButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->deleteButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->deleteButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->deleteButton->Location = System::Drawing::Point(460, 50);
            this->deleteButton->Size = System::Drawing::Size(120, 40);
            this->deleteButton->TabIndex = 3;
            this->deleteButton->Text = L"Удалить";
            this->deleteButton->UseVisualStyleBackColor = false;
            this->deleteButton->Click += gcnew System::EventHandler(this, &MyForm::deleteButton_Click);

            // saveButton
            this->saveButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(92)),
                static_cast<System::Int32>(static_cast<System::Byte>(184)),
                static_cast<System::Int32>(static_cast<System::Byte>(92)));
            this->saveButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
            this->saveButton->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->saveButton->ForeColor = System::Drawing::SystemColors::ControlLightLight;
            this->saveButton->Location = System::Drawing::Point(590, 50);
            this->saveButton->Size = System::Drawing::Size(150, 40);
            this->saveButton->TabIndex = 4;
            this->saveButton->Text = L"Сохранить проект";
            this->saveButton->UseVisualStyleBackColor = false;
            this->saveButton->Click += gcnew System::EventHandler(this, &MyForm::saveButton_Click);

            // openFileDialog
            this->openFileDialog->FileName = L"openFileDialog1";
            this->openFileDialog->Filter = L"Изображения|*.png;*.jpg;*.jpeg|Все файлы|*.*";

            // saveFileDialog
            this->saveFileDialog->Filter = L"PNG Files|*.png";

            // drawingPanel
            this->drawingPanel->AutoScroll = false;
            this->drawingPanel->BackColor = System::Drawing::Color::White;
            this->drawingPanel->Controls->Add(this->roomPictureBox);
            this->drawingPanel->Dock = System::Windows::Forms::DockStyle::Fill;
            this->drawingPanel->Location = System::Drawing::Point(0, 100);
            this->drawingPanel->Name = L"drawingPanel";
            this->drawingPanel->TabIndex = 6;

            // controlPanel
            this->controlPanel->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(248)),
                static_cast<System::Int32>(static_cast<System::Byte>(249)),
                static_cast<System::Int32>(static_cast<System::Byte>(250)));
            this->controlPanel->Controls->Add(this->titleLabel);
            this->controlPanel->Controls->Add(this->addRoomButton);
            this->controlPanel->Controls->Add(this->addFurnitureButton);
            this->controlPanel->Controls->Add(this->rotateFurnitureButton);
            this->controlPanel->Controls->Add(this->deleteButton);
            this->controlPanel->Controls->Add(this->saveButton);
            this->controlPanel->Dock = System::Windows::Forms::DockStyle::Top;
            this->controlPanel->Location = System::Drawing::Point(0, 0);
            this->controlPanel->Name = L"controlPanel";
            this->controlPanel->Size = System::Drawing::Size(984, 100);
            this->controlPanel->TabIndex = 7;

            // titleLabel
            this->titleLabel->AutoSize = true;
            this->titleLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
                static_cast<System::Byte>(0)));
            this->titleLabel->Location = System::Drawing::Point(10, 10);
            this->titleLabel->Size = System::Drawing::Size(216, 24);
            this->titleLabel->TabIndex = 5;
            this->titleLabel->Text = L"Room Planner 2D Pro";

            // MyForm
            this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(984, 761);
            this->Controls->Add(this->drawingPanel);
            this->Controls->Add(this->controlPanel);
            this->Name = L"MyForm";
            this->Text = L"Room Planner 2D Pro";
            (cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->roomPictureBox))->EndInit();
            this->drawingPanel->ResumeLayout(false);
            this->controlPanel->ResumeLayout(false);
            this->controlPanel->PerformLayout();
            this->ResumeLayout(false);
        }
#pragma endregion

        void ScaleElements() {
            float scaleX = (this->ClientSize.Width - 20) / 700.0f; // Учитываем отступы
            float scaleY = (this->ClientSize.Height - controlPanel->Height - 20) / 500.0f; // Учитываем панель управления

            // Устанавливаем размеры и позиции
            this->roomPictureBox->Size = System::Drawing::Size(static_cast<int>(700 * scaleX), static_cast<int>(500 * scaleY));
            this->roomPictureBox->Location = System::Drawing::Point(0, 0);

            // Масштабируем мебель
            for each(PictureBox ^ furniture in furnitureItems) {
                furniture->Size = System::Drawing::Size(static_cast<int>(100 * scaleX), static_cast<int>(100 * scaleY));
                furniture->Location = Point(static_cast<int>(furniture->Location.X * scaleX), static_cast<int>(furniture->Location.Y * scaleY));
            }
        }

        void MyForm_Resize(System::Object^ sender, System::EventArgs^ e) {
            ScaleElements();
        }

        void addRoomButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    roomImage = Image::FromFile(openFileDialog->FileName);
                    roomPictureBox->Image = roomImage;
                    roomPictureBox->SizeMode = PictureBoxSizeMode::Zoom;
                    isRoomSelected = true;
                    ScaleElements(); // Устанавливаем масштаб
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Не удалось загрузить изображение комнаты: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void addFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!isRoomSelected) {
                MessageBox::Show("Пожалуйста, сначала добавьте комнату", "Предупреждение",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            if (openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    String^ furnitureImagePath = openFileDialog->FileName;
                    Image^ furnitureImage = Image::FromFile(furnitureImagePath);

                    PictureBox^ newFurniture = gcnew PictureBox();
                    newFurniture->Image = furnitureImage;
                    newFurniture->Size = System::Drawing::Size(100, 100); // Задаем размер мебели
                    newFurniture->SizeMode = PictureBoxSizeMode::Zoom;
                    newFurniture->BackColor = Color::Transparent;
                    newFurniture->Location = Point(100, 100);
                    newFurniture->BorderStyle = BorderStyle::FixedSingle;
                    newFurniture->Tag = gcnew FurnitureData(furnitureImagePath, newFurniture->Location, 0, false, newFurniture->Size);
                    newFurniture->DoubleClick += gcnew System::EventHandler(this, &MyForm::Furniture_DoubleClick);

                    // Сделать мебель перетаскиваемой
                    newFurniture->MouseDown += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseDown);
                    newFurniture->MouseMove += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseMove);
                    newFurniture->MouseUp += gcnew MouseEventHandler(this, &MyForm::Furniture_MouseUp);

                    drawingPanel->Controls->Add(newFurniture);
                    newFurniture->BringToFront();
                    furnitureItems->Add(newFurniture);
                    ScaleElements(); // Устанавливаем масштаб
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Не удалось загрузить изображение мебели: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void Furniture_DoubleClick(Object^ sender, EventArgs^ e) {
            PictureBox^ furniture = (PictureBox^)sender;
            FurnitureData^ data = (FurnitureData^)furniture->Tag;

            // Переключение состояния блокировки
            bool isLocked = furniture->BorderStyle == BorderStyle::Fixed3D;

            if (isLocked) {
                // Разблокировка мебели
                furniture->BorderStyle = BorderStyle::FixedSingle;
                furniture->Cursor = Cursors::SizeAll;
                data->IsLocked = false;
            }
            else {
                // Блокировка мебели
                furniture->BorderStyle = BorderStyle::Fixed3D;
                furniture->Cursor = Cursors::No;
                data->IsLocked = true;
            }
        }

        void rotateFurnitureButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (draggedFurniture != nullptr && draggedFurniture != roomPictureBox) {
                FurnitureData^ data = (FurnitureData^)draggedFurniture->Tag;

                // Проверка, заблокирована ли мебель
                if (data->IsLocked) {
                    MessageBox::Show("Эта мебель заблокирована. Разблокируйте ее, дважды щелкнув по ней.",
                        "Мебель заблокирована", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    return;
                }

                int currentAngle = data->RotationAngle;
                currentAngle += 90;
                if (currentAngle >= 360) {
                    currentAngle = 0;
                }
                data->RotationAngle = currentAngle;
                RotateFurniture(draggedFurniture, currentAngle);
            }
            else {
                MessageBox::Show("Пожалуйста, выберите мебель, которую хотите повернуть.", "Нет мебели",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void deleteButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (draggedFurniture != nullptr) {
                if (draggedFurniture == roomPictureBox) {
                    // Удаление комнаты
                    roomPictureBox->Image = nullptr;
                    roomImage = nullptr;
                    isRoomSelected = false;
                }
                else {
                    // Удаление мебели
                    drawingPanel->Controls->Remove(draggedFurniture);
                    furnitureItems->Remove(draggedFurniture);
                }
                draggedFurniture = nullptr;
            }
            else {
                MessageBox::Show("Пожалуйста, выберите мебель или комнату для удаления.", "Нет выбора",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
            }
        }

        void saveButton_Click(System::Object^ sender, System::EventArgs^ e) {
            if (!isRoomSelected) {
                MessageBox::Show("Нет комнаты для сохранения", "Предупреждение",
                    MessageBoxButtons::OK, MessageBoxIcon::Information);
                return;
            }

            saveFileDialog->FileName = "RoomPlan.png"; // Название файла по умолчанию
            if (saveFileDialog->ShowDialog() == Windows::Forms::DialogResult::OK) {
                try {
                    Bitmap^ combinedImage = CombineRoomAndFurniture(); // Объединение комнаты и мебели
                    combinedImage->Save(saveFileDialog->FileName, System::Drawing::Imaging::ImageFormat::Png);  // Сохранение как PNG
                    delete combinedImage;

                    MessageBox::Show("Проект успешно сохранен в формате PNG", "Сохранено",
                        MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка при сохранении: " + ex->Message, "Ошибка",
                        MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        Bitmap^ CombineRoomAndFurniture() {
            int width = roomPictureBox->Width;
            int height = roomPictureBox->Height;
            Bitmap^ combinedImage = gcnew Bitmap(width, height);
            Graphics^ g = Graphics::FromImage(combinedImage);

            // Рисуем комнату
            if (roomImage != nullptr) {
                g->DrawImage(roomImage, 0, 0, width, height);
            }

            // Рисуем мебель
            for each(PictureBox ^ furniture in furnitureItems) {
                if (furniture->Image != nullptr) {
                    g->DrawImage(furniture->Image, furniture->Location.X, furniture->Location.Y, furniture->Width, furniture->Height);
                }
            }

            delete g; // Освобождение ресурсов
            return combinedImage;
        }

        void roomPictureBox_Click(System::Object^ sender, System::EventArgs^ e) {
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
            ((FurnitureData^)furniture->Tag)->RotationAngle = angle;
        }

        void Furniture_MouseDown(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                PictureBox^ furniture = (PictureBox^)sender;
                FurnitureData^ data = (FurnitureData^)furniture->Tag;

                // Проверка, заблокирована ли мебель
                if (data->IsLocked) {
                    return;
                }

                isDragging = true;
                draggedFurniture = furniture;
                dragStartPoint = Point(e->X, e->Y);
                draggedFurniture->BringToFront();
            }
        }

        void Furniture_MouseMove(Object^ sender, MouseEventArgs^ e) {
            if (isDragging && draggedFurniture != nullptr) {
                FurnitureData^ data = (FurnitureData^)draggedFurniture->Tag;

                // Проверка, заблокирована ли мебель
                if (data->IsLocked) {
                    return;
                }

                Point newLocation = draggedFurniture->Location;
                newLocation.X += e->X - dragStartPoint.X;
                newLocation.Y += e->Y - dragStartPoint.Y;

                // Получаем границы комнаты
                int roomLeft = roomPictureBox->Left;
                int roomTop = roomPictureBox->Top;
                int roomWidth = roomPictureBox->Width;
                int roomHeight = roomPictureBox->Height;

                // Получаем границы мебели
                int furnitureWidth = draggedFurniture->Width;
                int furnitureHeight = draggedFurniture->Height;

                // Проверка на столкновение с границами комнаты
                if (newLocation.X < roomLeft) {
                    newLocation.X = roomLeft;
                }
                if (newLocation.Y < roomTop) {
                    newLocation.Y = roomTop;
                }
                if (newLocation.X + furnitureWidth > roomLeft + roomWidth) {
                    newLocation.X = roomLeft + roomWidth - furnitureWidth;
                }
                if (newLocation.Y + furnitureHeight > roomTop + roomHeight) {
                    newLocation.Y = roomTop + roomHeight - furnitureHeight;
                }

                draggedFurniture->Location = newLocation;
                data->Location = newLocation;
            }
        }

        void Furniture_MouseUp(Object^ sender, MouseEventArgs^ e) {
            if (e->Button == System::Windows::Forms::MouseButtons::Left) {
                isDragging = false;
            }
        }
    };
}