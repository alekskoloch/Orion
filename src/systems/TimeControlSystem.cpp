#include "TimeControlSystem.h"

void TimeControlSystem::updateSlowMotion(float& slowMotionFactor, bool slowMotion, float slowMotionSpeed, float deltaTime)
{
    float targetFactor = slowMotion ? 0.1f : 1.0f;
    float direction = (slowMotionFactor < targetFactor) ? 1.0f : -1.0f;
    float change = direction * slowMotionSpeed * deltaTime;

    slowMotionFactor += change;

    if (direction > 0 && slowMotionFactor > targetFactor || direction < 0 && slowMotionFactor < targetFactor)
    {
        slowMotionFactor = targetFactor;
    }
}