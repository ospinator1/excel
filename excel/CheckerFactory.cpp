#include "CheckerFactory.h"

using namespace System::Drawing;

Color Player1CheckerFactory::CreateColor()
{
    return Color::FromArgb(50, 50, 50); // Dark color for Player 1
}

Color Player2CheckerFactory::CreateColor()
{
    return Color::FromArgb(240, 240, 240); // Light color for Player 2
}