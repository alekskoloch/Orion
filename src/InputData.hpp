#pragma once

namespace Mouse
{
struct ScreenCoords
{
    int x;
    int y;
};

struct WorldCoords
{
    float x;
    float y;
};

struct MouseState
{
    ScreenCoords screenPosition;
    WorldCoords worldPosition;
};
} // namespace Mouse

// TODO: in the future:
// struct InputData
// {
//     MouseState mouse;
//     KeyboardState keyboard;
//     GamepadState gamepad;
// };