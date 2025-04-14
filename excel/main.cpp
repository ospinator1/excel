#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int main(array<String^>^ args) {
    // Включение визуальных стилей для приложения
    Application::EnableVisualStyles();

    // Настройка совместимости текстового отображения
    Application::SetCompatibleTextRenderingDefault(false);

    // Создание экземпляра формы
    CircleForm^ form = gcnew CircleForm();

    // Запуск приложения
    Application::Run(form);

    return 0; // Возвращаем 0 для успешного завершения
}