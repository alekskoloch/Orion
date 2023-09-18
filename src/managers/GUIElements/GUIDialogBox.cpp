#include "GUIDialogBox.h"

GUIDialogBox::GUIDialogBox(sf::RenderWindow& window, std::string message, sf::Font& font)
    : window(window), message(message), font(font)
{
    this->initialize();
}

void GUIDialogBox::initialize()
{
    this->initializeBox();
    this->initializeText();
}

void GUIDialogBox::initializeBox()
{
    this->box.setSize(sf::Vector2f(800, 300));
    this->box.setFillColor(sf::Color(0, 0, 0, 200));
    this->box.setOutlineColor(sf::Color(255, 255, 255, 255));
    this->box.setOutlineThickness(2);
    this->box.setOrigin(this->box.getSize().x / 2, this->box.getSize().y / 2);
    this->box.setPosition(this->window.getSize().x / 2, this->window.getSize().y / 2);
}

void GUIDialogBox::initializeText()
{
    this->messageText.setString(this->message);
    this->messageText.setFont(this->font);
    this->messageText.setCharacterSize(30);
    this->messageText.setFillColor(sf::Color::White);
    this->messageText.setOutlineThickness(1);
    this->messageText.setOutlineColor(sf::Color::Black);

    this->noText.setString("No");
    this->noText.setFont(this->font);
    this->noText.setCharacterSize(30);
    this->noText.setFillColor(sf::Color::White);
    this->noText.setOutlineThickness(1);
    this->noText.setOutlineColor(sf::Color::Black);

    this->yesText.setString("Yes");
    this->yesText.setFont(this->font);
    this->yesText.setCharacterSize(30);
    this->yesText.setFillColor(sf::Color::White);
    this->yesText.setOutlineThickness(1);
    this->yesText.setOutlineColor(sf::Color::Black);
}

void GUIDialogBox::updateText()
{
    this->messageText.setOrigin(
        this->messageText.getGlobalBounds().width / 2.f,
        this->messageText.getGlobalBounds().height / 2.f
    );
    this->messageText.setPosition(
        this->box.getPosition().x,
        this->box.getPosition().y - this->box.getSize().y / 2 + 40
    );

    this->noText.setOrigin(
        this->noText.getGlobalBounds().width / 2.f,
        this->noText.getGlobalBounds().height / 2.f
    );
    this->noText.setPosition(
        this->box.getPosition().x - this->box.getSize().x / 4,
        this->box.getPosition().y + this->box.getSize().y / 2 - 60
    );

    this->yesText.setOrigin(
        this->yesText.getGlobalBounds().width / 2.f,
        this->yesText.getGlobalBounds().height / 2.f
    );
    this->yesText.setPosition(
        this->box.getPosition().x + this->box.getSize().x / 4,
        this->box.getPosition().y + this->box.getSize().y / 2 - 60
    );
}

void GUIDialogBox::update()
{
    this->updateText();

    if (this->noText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->window))))
    {
        this->noText.setFillColor(sf::Color::Red);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            this->state = GUIDialogBoxState::No;
    }
    else
    {
        this->noText.setFillColor(sf::Color::White);
    }

    if (this->yesText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->window))))
    {
        this->yesText.setFillColor(sf::Color::Green);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            this->state = GUIDialogBoxState::Yes;
    }
    else
    {
        this->yesText.setFillColor(sf::Color::White);
    }
}

void GUIDialogBox::draw()
{
    this->window.draw(this->box);
    this->window.draw(this->messageText);
    this->window.draw(this->noText);
    this->window.draw(this->yesText);
}

void GUIDialogBox::setMessage(std::string message)
{
    this->message = message;
    this->messageText.setString(this->message);
}