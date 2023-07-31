#pragma once

class TimeControlSystem
{
public:
    static void updateSlowMotion(float& slowMotionFactor, bool slowMotion, float slowMotionSpeed, float deltaTime);
};