#include <Windows.h>
#include "MyForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace RoomPlannerApp;

int main(array<String^>^ args) { // Изменен тип на int
    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);
    Application::Run(gcnew MyForm());
    return 0; // Возврат 0
}