#pragma once

#include "../../pch.h"

enum class GUIDialogBoxState
{
    Yes,
    No,
    Idle
};

class GUIDialogBox
{
public:
    GUIDialogBox(sf::RenderWindow& window, std::string message, sf::Font& font);

    void update();
    void draw();

    GUIDialogBoxState getState() const { return this->state; }
    void setState(GUIDialogBoxState state) { this->state = state; }
private:
    sf::RenderWindow& window;
    std::string message;

    GUIDialogBoxState state = GUIDialogBoxState::Idle;

    void initialize();
    void initializeBox();
    void initializeText();
    void updateText();

    sf::Font& font;
    sf::Text messageText;
    sf::Text noText;
    sf::Text yesText;
    sf::RectangleShape box;
};