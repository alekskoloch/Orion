#pragma once

#include <SFML/Graphics.hpp>
#include "ConfigManager.hpp"
#include "SoundManager.h"

enum class GUIDialogBoxState
{
    Yes,
    No,
    Ok,
    Idle,
    Hidden
};

enum class GUIDialogBoxType
{
    YesNo,
    Ok
};

class GUIDialogBox
{
public:
    GUIDialogBox(sf::RenderWindow& window, std::vector<std::string> messages, sf::Font& font);

    void update();
    void draw();

    GUIDialogBoxState getState() const { return this->state; }
    void setState(GUIDialogBoxState state) { this->state = state; }
    
    std::string getTarget() const { return this->target; }
    void setTarget(std::string target) { this->target = target; }

    void setMessage(std::vector<std::string> message);
    void setType(GUIDialogBoxType type) { this->type = type; }
    void setButtonOkText(const std::string& text) { this->okText.setString(text); }
    void setCallback(std::function<void()> callback) { this->callback = callback; }
private:
    sf::RenderWindow& window;
    std::vector<std::string> messages;
    std::string target;
    std::function<void()> callback;

    GUIDialogBoxType type = GUIDialogBoxType::YesNo;
    GUIDialogBoxState state = GUIDialogBoxState::Hidden;

    void initialize();
    void initializeBox();
    void initializeText();
    void updateText();

    sf::Font& font;
    std::vector<sf::Text> messageTexts;
    sf::Text noText;
    sf::Text yesText;
    sf::Text okText;
    sf::RectangleShape box;

    bool readyToExecute = false;
    bool mouseIsReleased = false;
};