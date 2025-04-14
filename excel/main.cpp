#include "MyForm.h"

[STAThread]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // Создание окна с кругами
    CircleForm^ form = gcnew CircleForm();
    Application::Run(form);

    return 0;
}