#pragma once

struct WaypointMovement
{
    std::vector<sf::Vector2f> waypoints;
    unsigned int currentWaypointIndex;

    WaypointMovement(std::vector<sf::Vector2f> waypoints, unsigned int currentWaypointIndex = 0)
        : waypoints(waypoints), currentWaypointIndex(currentWaypointIndex) {}
};