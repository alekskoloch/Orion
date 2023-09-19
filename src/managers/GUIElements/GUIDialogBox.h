#pragma once

#include "../../pch.h"

enum class GUIDialogBoxState
{
    Yes,
    No,
    Idle,
    Hidden
};

class GUIDialogBox
{
public:
    GUIDialogBox(sf::RenderWindow& window, std::string message, sf::Font& font);

    void update();
    void draw();

    GUIDialogBoxState getState() const { return this->state; }
    void setState(GUIDialogBoxState state) { this->state = state; }
    
    std::string getTarget() const { return this->target; }
    void setTarget(std::string target) { this->target = target; }

    void setMessage(std::string message);
private:
    sf::RenderWindow& window;
    std::string message;
    std::string target;

    GUIDialogBoxState state = GUIDialogBoxState::Hidden;

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