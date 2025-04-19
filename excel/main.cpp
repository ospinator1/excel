#include <Windows.h>
#include "MyForm.h"
using namespace System;
using namespace System::Windows::Forms;
[STAThread]
int main(array<String^>^ args)
{
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    BackgammonForm^ form = gcnew BackgammonForm();
    Application::Run(form);

    return 0;
}