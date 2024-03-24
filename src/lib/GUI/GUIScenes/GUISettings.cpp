#include "pch.h"
#include "GUISettings.hpp"

#include "nlohmann/json.hpp"
#include <fstream>

GUISettings::GUISettings(entt::registry& registry, sf::RenderWindow& window) : registry(registry), window(window)
{
    this->initialize();
    this->view = this->window.getDefaultView();

    unsigned int currentWidth;
    unsigned int currentHeight;
    unsigned int currentFrameRate;

    //load settings from gameConfig.json trough nlohmann json
    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    if (file.is_open())
    {
        nlohmann::json j;
        file >> j;

        currentWidth = j["resolution"]["width"];
        currentHeight = j["resolution"]["height"];
        currentFrameRate = j["frameRateLimit"];
    }
    else
    {
        throw std::runtime_error("Could not open gameConfig.json");
    }
    

    this->elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 4),
        "Settings",
        TITLE_FONT_SIZE / 2
    ));

    this->elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2),
        "Resolution",
        BUTTONS_FONT_SIZE
    ));

    unsigned int currentResolutionIndex = 0;
    std::vector<std::string> resolutions;
    for (auto& resolution : sf::VideoMode::getFullscreenModes())
    {
        resolutions.push_back(std::to_string(resolution.width) + "x" + std::to_string(resolution.height));
    }
    

    std::sort(resolutions.begin(), resolutions.end(), [](const std::string& a, const std::string& b) {
        return std::stoi(a.substr(0, a.find('x'))) < std::stoi(b.substr(0, b.find('x')));
    });

    for (size_t i = 0; i < resolutions.size(); i++)
    {
        if (resolutions[i] == std::to_string(currentWidth) + "x" + std::to_string(currentHeight))
        {
            currentResolutionIndex = i;
            break;
        }
    }

    this->elements.push_back(GUIElementFactory::createArrowButton(
        sf::Vector2f(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2),
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        resolutions,
        currentResolutionIndex
    ));

    unsigned int currentFrameRateIndex = 0;
    std::vector<std::string> frameRates = { "30", "60", "120", "144", "240", "unlimited" };

    for (size_t i = 0; i < frameRates.size(); i++)
    {
        if (frameRates[i] == std::to_string(currentFrameRate))
        {
            currentFrameRateIndex = i;
            break;
        }
    }

    this->elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + BUTTON_HEIGHT + MARGIN),
        "Frame Rate",
        BUTTONS_FONT_SIZE
    ));

    this->elements.push_back(GUIElementFactory::createArrowButton(
        sf::Vector2f(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + BUTTON_HEIGHT + MARGIN),
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        frameRates,
        currentFrameRateIndex
    ));

}

void GUISettings::update(sf::Time& deltaTime)
{
    sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(this->window).x, sf::Mouse::getPosition(this->window).y);

    for (auto& element : elements)
    {
        element->update(deltaTime, mousePosition);
    }

    window.setView(view);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        //save settings to gameConfig.json
        std::ofstream file(CONFIG_PATH + std::string("gameConfig.json"));
        if (file.is_open())
        {
            nlohmann::json j;
            j["resolution"]["width"] = std::stoi(elements[2]->getText());
            j["resolution"]["height"] = std::stoi(elements[2]->getText().substr(elements[2]->getText().find('x') + 1));
            j["frameRateLimit"] = std::stoi(elements[4]->getText());
            file << j.dump(4);
        }
        else
        {
            throw std::runtime_error("Could not open gameConfig.json");
        }


        SceneManager::getInstance().setCurrentScene(Scene::MainMenu);
    }



}

void GUISettings::draw()
{
    for (auto& element : elements)
    {
        this->window.draw(*element);
    }
}

void GUISettings::initialize()
{
    this->SCREEN_WIDTH = this->window.getSize().x;
    this->SCREEN_HEIGHT = this->window.getSize().y;

    this->BUTTON_WIDTH = this->SCREEN_WIDTH / 6;
    this->BUTTON_HEIGHT = this->SCREEN_HEIGHT / 20;

    this->TITLE_FONT_SIZE = SCREEN_WIDTH / 10;
    this->BUTTONS_FONT_SIZE = BUTTON_HEIGHT / 4;

    this->MARGIN = this->BUTTON_HEIGHT / 3;
    this->OUTLINE_THICKNESS = this->SCREEN_HEIGHT / 200;
}