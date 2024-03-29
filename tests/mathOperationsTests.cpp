#include <gtest/gtest.h>

#include <SFML/Graphics.hpp>

#include "../src/lib/Components/Position.h"
#include "../src/lib/Utils/MathOperations.h"
#include "../src/lib/Utils/Mouse.h"

TEST(DistanceToMouseTest, SamePoints)
{
    sf::Vector2f mousePosition(0, 0);
    Position position{sf::Vector2f(0, 0)};
    EXPECT_FLOAT_EQ(CalculateDistance(mousePosition, position.position), 0.0f);
}

TEST(DistanceToMouseTest, DifferentPoints)
{
    sf::Vector2f mousePosition(3, 4);
    Position position{sf::Vector2f(0, 0)};
    EXPECT_FLOAT_EQ(CalculateDistance(mousePosition, position.position), 5.0f);
}

TEST(AngleDifferenceTest, ZeroDifference)
{
    EXPECT_FLOAT_EQ(AngleDifference(45.0f, 45.0f), 0.0f);
}

TEST(AngleDifferenceTest, PositiveDifference)
{
    EXPECT_FLOAT_EQ(AngleDifference(90.0f, 45.0f), 45.0f);
}

TEST(AngleDifferenceTest, NegativeDifference)
{
    EXPECT_FLOAT_EQ(AngleDifference(45.0f, 90.0f), -45.0f);
}

TEST(AngleDifferenceTest, PositiveDifferenceOverflow)
{
    EXPECT_FLOAT_EQ(AngleDifference(180.0f, -180.0f), 0.0f);
}

TEST(AngleDifferenceTest, NegativeDifferenceOverflow)
{
    EXPECT_FLOAT_EQ(AngleDifference(-180.0f, 180.0f), 0.0f);
}