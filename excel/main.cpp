#include "MyForm.h"

[STAThread]
int main(array<System::String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    // �������� ���� � �������
    CircleForm^ form = gcnew CircleForm();
    Application::Run(form);

    return 0;
}