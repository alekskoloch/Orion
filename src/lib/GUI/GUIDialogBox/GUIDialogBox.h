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

class Window;

namespace Mouse
{
class MouseState;
} // namespace Mouse

class GUIDialogBox
{
public:
    GUIDialogBox( std::vector< std::string > messages, sf::Font& font );

    void update( const Mouse::MouseState& mouseState );
    void draw( Window& window );

    GUIDialogBoxState getState() const { return this->state; }
    void setState(GUIDialogBoxState state) { this->state = state; }
    
    std::string getTarget() const { return this->target; }
    void setTarget(std::string target) { this->target = target; }

    void setMessage(std::vector<std::string> message);
    void setType(GUIDialogBoxType type) { this->type = type; }
private:
    std::vector<std::string> messages;
    std::string target;

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
};