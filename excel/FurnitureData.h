#pragma once

using namespace System;
using namespace System::Drawing;

namespace RoomPlannerApp {
    [Serializable]
    public ref class FurnitureData {
    public:
        String^ ImagePath;
        Point Location;
        int RotationAngle;
        bool IsLocked;
        System::Drawing::Size Size;

        FurnitureData(String^ path, Point loc, int angle, bool locked, System::Drawing::Size size) {
            ImagePath = path;
            Location = loc;
            RotationAngle = angle;
            IsLocked = locked;
            Size = size;
        }
    };
}