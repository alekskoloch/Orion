#pragma once

#include <entt/entt.hpp>

#include <vector>
#include <algorithm>

using GroupID = unsigned int;

struct GroupFormation
{
    std::vector<std::vector<int>> formation;
    std::vector<std::vector<bool>> seats;

    float offset = 200.f;

    bool isFormationFull() const
    {
        for (auto& row : seats)
            for (auto seat : row)
                if (!seat)
                    return false;
        return true;
    }

    int getCurrentPositionPriority()
    {
        if (isFormationFull())
            return -1;

        int maxPriority = 0;
        for (const auto& row : formation) {
            auto rowMax = *std::max_element(row.begin(), row.end());
            if (rowMax > maxPriority) {
                maxPriority = rowMax;
            }
        }

        for (int priority = 1; priority <= maxPriority; ++priority) {
            for (size_t i = 0; i < formation.size(); ++i) {
                for (size_t j = 0; j < formation[i].size(); ++j) {
                    if (formation[i][j] == priority && !seats[i][j]) {
                        return priority;
                    }
                }
            }
        }

        return -1; 
    }

    //TODO: position should be assigned randomly based on priority
    sf::Vector2f getSeatPosition()
    {
        auto priority = getCurrentPositionPriority();

        for (size_t i = 0; i < formation.size(); ++i) {
            for (size_t j = 0; j < formation[i].size(); ++j) {
                if (formation[i][j] == priority && !seats[i][j])
                {
                    //TODO: this should be calculated based on the leader position
                    int leaderRow = 1;
                    int leaderCol = 1;

                    int seatRow = i;
                    int seatCol = j;

                    int rowOffset = seatRow - leaderRow;
                    int colOffset = seatCol - leaderCol;

                    seats[i][j] = true;

                    return sf::Vector2f(colOffset * offset, rowOffset * offset);
                }
            }
        }
        return sf::Vector2f(0.f, 0.f);
    }
};

static const GroupFormation RECTANGLE_FORMATION = {
    {
        {2, 1, 2},
        {1, 0, 1},
        {2, 1, 2}
    },
    {
        {false, false, false},
        {false, true, false},
        {false, false, false}
    }
};

struct EnemyGroupLeader
{
    GroupID groupID;

    std::vector<entt::entity> members;
    GroupFormation formation = RECTANGLE_FORMATION;
    
    unsigned int numOfMembers = 0;
};

struct EnemyGroupMember
{
    entt::entity leader;
    sf::Vector2f offset = sf::Vector2f(0.f, 0.f);
};
