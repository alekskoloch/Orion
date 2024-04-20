#include "GUIDialogBox.h"

GUIDialogBox::GUIDialogBox(sf::RenderWindow& window, std::vector<std::string> messages, sf::Font& font)
    : window(window), messages(messages), font(font)
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
    this->box.setSize(sf::Vector2f(
        800 * ConfigManager::getInstance().getScale(),
        300 * ConfigManager::getInstance().getScale()
    ));
    this->box.setFillColor(sf::Color(0, 0, 0, 200));
    this->box.setOutlineColor(sf::Color(255, 255, 255, 255));
    this->box.setOutlineThickness(2);
    this->box.setOrigin(this->box.getSize().x / 2, this->box.getSize().y / 2);
    this->box.setPosition(this->window.getSize().x / 2, this->window.getSize().y / 2);
}

void GUIDialogBox::initializeText()
{
    for (auto line : this->messages)
    {
        sf::Text text;
        text.setString(line);
        text.setFont(this->font);
        text.setCharacterSize(30 * ConfigManager::getInstance().getScale());
        text.setFillColor(sf::Color::White);
        text.setOutlineThickness(1);
        text.setOutlineColor(sf::Color::Black);

        this->messageTexts.push_back(text);
    }

    this->noText.setString("No");
    this->noText.setFont(this->font);
    this->noText.setCharacterSize(30 * ConfigManager::getInstance().getScale());
    this->noText.setFillColor(sf::Color::White);
    this->noText.setOutlineThickness(1);
    this->noText.setOutlineColor(sf::Color::Black);

    this->yesText.setString("Yes");
    this->yesText.setFont(this->font);
    this->yesText.setCharacterSize(30 * ConfigManager::getInstance().getScale());
    this->yesText.setFillColor(sf::Color::White);
    this->yesText.setOutlineThickness(1);
    this->yesText.setOutlineColor(sf::Color::Black);

    this->okText.setString("Ok");
    this->okText.setFont(this->font);
    this->okText.setCharacterSize(30 * ConfigManager::getInstance().getScale());
    this->okText.setFillColor(sf::Color::White);
    this->okText.setOutlineThickness(1);
    this->okText.setOutlineColor(sf::Color::Black);
}

void GUIDialogBox::updateText()
{
    for (int i = 0; i < this->messageTexts.size(); i++)
    {
        this->messageTexts[i].setOrigin(
            this->messageTexts[i].getGlobalBounds().width / 2.f,
            this->messageTexts[i].getGlobalBounds().height / 2.f
        );
        this->messageTexts[i].setPosition(
            this->box.getPosition().x,
            this->box.getPosition().y - this->box.getSize().y / 2 + 40 * ConfigManager::getInstance().getScale() + i * 40 * ConfigManager::getInstance().getScale()
        );
    }

    this->noText.setOrigin(
        this->noText.getGlobalBounds().width / 2.f,
        this->noText.getGlobalBounds().height / 2.f
    );
    this->noText.setPosition(
        this->box.getPosition().x - this->box.getSize().x / 4,
        this->box.getPosition().y + this->box.getSize().y / 2 - 60 * ConfigManager::getInstance().getScale()
    );

    this->yesText.setOrigin(
        this->yesText.getGlobalBounds().width / 2.f,
        this->yesText.getGlobalBounds().height / 2.f
    );
    this->yesText.setPosition(
        this->box.getPosition().x + this->box.getSize().x / 4,
        this->box.getPosition().y + this->box.getSize().y / 2 - 60 * ConfigManager::getInstance().getScale()
    );

    this->okText.setOrigin(
        this->okText.getGlobalBounds().width / 2.f,
        this->okText.getGlobalBounds().height / 2.f
    );
    this->okText.setPosition(
        this->box.getPosition().x,
        this->box.getPosition().y + this->box.getSize().y / 2 - 60 * ConfigManager::getInstance().getScale()
    );
}

void GUIDialogBox::update()
{
    this->updateText();

    if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
        this->mouseIsReleased = true;

    if (this->type == GUIDialogBoxType::YesNo)
    {
        if (this->noText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->window))))
        {
            if (this->noText.getFillColor() != sf::Color::Red)
            {
                this->noText.setFillColor(sf::Color::Red);
                SoundManager::getInstance().playSound("MouseHover");
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                this->state = GUIDialogBoxState::No;
        }
        else
        {
            this->noText.setFillColor(sf::Color::White);
        }

        if (this->yesText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->window))))
        {
            if (this->yesText.getFillColor() != sf::Color::Green)
            {
                this->yesText.setFillColor(sf::Color::Green);
                SoundManager::getInstance().playSound("MouseHover");
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                this->state = GUIDialogBoxState::Yes;
        }
        else
        {
            this->yesText.setFillColor(sf::Color::White);
        }
    }
    else if (this->type == GUIDialogBoxType::Ok)
    {
        if (this->okText.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(this->window))))
        {
            if (this->okText.getFillColor() != sf::Color::Green)
            {
                this->okText.setFillColor(sf::Color::Green);
                SoundManager::getInstance().playSound("MouseHover");
            }
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->mouseIsReleased)
            {
                this->readyToExecute = true;
            }
            else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->readyToExecute)
            {
                this->readyToExecute = false;
                this->state = GUIDialogBoxState::Ok;

                if (this->callback)
                {
                    this->callback();
                    this->callback = nullptr;
                }
            }
        }
        else
        {
            this->okText.setFillColor(sf::Color::White);
        }
    }
}

void GUIDialogBox::draw()
{
    this->window.draw(this->box);
    for (auto text : this->messageTexts)
        this->window.draw(text);

    if (this->type == GUIDialogBoxType::YesNo)
    {
        this->window.draw(this->noText);
        this->window.draw(this->yesText);
    }
    else if (this->type == GUIDialogBoxType::Ok)
    {
        this->window.draw(this->okText);
    }
}

void GUIDialogBox::setMessage(std::vector<std::string> message)
{
    this->messages = message;

    this->messageTexts.clear();

    for (auto line : this->messages)
    {
        sf::Text text;
        text.setString(line);
        text.setFont(this->font);
        text.setCharacterSize(30 * ConfigManager::getInstance().getScale());
        text.setFillColor(sf::Color::White);
        text.setOutlineThickness(1);
        text.setOutlineColor(sf::Color::Black);

        this->messageTexts.push_back(text);
    }

    auto findLongestLine = [](std::vector<sf::Text> texts) -> sf::Text
    {
        sf::Text longestText = texts[0];

        for (auto text : texts)
        {
            if (text.getGlobalBounds().width > longestText.getGlobalBounds().width)
                longestText = text;
        }

        return longestText;
    };

    this->box.setSize(sf::Vector2f(
        findLongestLine(this->messageTexts).getGlobalBounds().width + 100 * ConfigManager::getInstance().getScale(),
        this->box.getSize().y
    ));

    this->box.setOrigin(sf::Vector2f(
        this->box.getSize().x / 2,
        this->box.getSize().y / 2
    ));

    this->box.setPosition(sf::Vector2f(
        this->window.getSize().x / 2,
        this->window.getSize().y / 2
    ));

    this->updateText();
}