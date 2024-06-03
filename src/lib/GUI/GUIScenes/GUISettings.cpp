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
    std::string currentWindowMode;
    bool tutorialEnabled;

    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    if (file.is_open())
    {
        nlohmann::json config;
        file >> config;

        currentWidth = config["resolution"]["width"];
        currentHeight = config["resolution"]["height"];
        currentFrameRate = config["frameRateLimit"];
        currentWindowMode = config["windowMode"].get<std::string>();
        tutorialEnabled = config["tutorialEnabled"].get<bool>();
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
            this->loadedResolution = resolutions[i];
            break;
        }
    }

    this->elements.push_back(GUIElementFactory::createArrowButton(
        sf::Vector2f(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2),
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        resolutions,
        currentResolutionIndex,
        BUTTONS_FONT_SIZE
    ));

    unsigned int currentFrameRateIndex = 0;
    std::vector<std::string> frameRates = { "30", "60", "120", "144", "240", "360", "unlimited" };

    for (size_t i = 0; i < frameRates.size(); i++)
    {
        if (frameRates[i] == std::to_string(currentFrameRate))
        {
            currentFrameRateIndex = i;
            this->loadedFrameRate = frameRates[i];
            break;
        }
        else if (currentFrameRate == 0)
        {
            currentFrameRateIndex = frameRates.size() - 1;
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
        currentFrameRateIndex,
        BUTTONS_FONT_SIZE
    ));

    unsigned int currentWindowModeIndex = 0;
    std::vector<std::string> windowModes = { "Fullscreen", "Windowed" };

    for (size_t i = 0; i < windowModes.size(); i++)
    {
        if (windowModes[i] == currentWindowMode)
        {
            currentWindowModeIndex = i;
            this->loadedWindowMode = windowModes[i];
            break;
        }
    }

    this->elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * BUTTON_HEIGHT + 2 * MARGIN),
        "Window Mode",
        BUTTONS_FONT_SIZE
    ));

    this->elements.push_back(GUIElementFactory::createArrowButton(
        sf::Vector2f(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 2 * BUTTON_HEIGHT + 2 * MARGIN),
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        windowModes,
        currentWindowModeIndex,
        BUTTONS_FONT_SIZE
    ));

    this->elements.push_back(GUIElementFactory::createText(
        sf::Vector2f(SCREEN_WIDTH / 2 - BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 3 * BUTTON_HEIGHT + 3 * MARGIN),
        "Tutorial",
        BUTTONS_FONT_SIZE
    ));

    this->elements.push_back(GUIElementFactory::createArrowButton(
        sf::Vector2f(SCREEN_WIDTH / 2 + BUTTON_WIDTH / 2, SCREEN_HEIGHT / 2 + 3 * BUTTON_HEIGHT + 3 * MARGIN),
        sf::Vector2f(BUTTON_WIDTH, BUTTON_HEIGHT),
        { "Enabled", "Disabled" },
        tutorialEnabled ? 0 : 1,
        BUTTONS_FONT_SIZE
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
        std::ofstream file(CONFIG_PATH + std::string("gameConfig.json"));
        if (file.is_open())
        {
            nlohmann::json config;
            config["resolution"]["width"] = std::stoi(elements[2]->getText());
            config["resolution"]["height"] = std::stoi(elements[2]->getText().substr(elements[2]->getText().find('x') + 1));
            if (elements[4]->getText() == "unlimited")
            {
                config["frameRateLimit"] = 0;
            }
            else
            {
                config["frameRateLimit"] = std::stoi(elements[4]->getText());
            }

            if (elements[6]->getText() == "Fullscreen")
            {
                config["windowMode"] = "Fullscreen";
            }
            else
            {
                config["windowMode"] = "Windowed";
            }

            if (elements[8]->getText() == "Enabled")
            {
                config["tutorialEnabled"] = true;
            }
            else
            {
                config["tutorialEnabled"] = false;
            }

            file << config.dump(4);
        }
        else
        {
            throw std::runtime_error("Could not open gameConfig.json");
        }


        SceneManager::getInstance().setCurrentScene(Scene::MainMenu);
    }

    //TODO: this is horrible, refactor this
    if (this->elements[2]->getText() != this->loadedResolution || this->elements[4]->getText() != this->loadedFrameRate || this->elements[6]->getText() != this->loadedWindowMode)
    {
        if (elements.size() == 9)
        {
            elements.push_back(GUIElementFactory::createText(
                sf::Vector2f(SCREEN_WIDTH / 2, SCREEN_HEIGHT - SCREEN_HEIGHT / 10),
                "Changes will be applied after restarting the game",
                BUTTONS_FONT_SIZE
            ));
        }
    }
    else
    {
        if (elements.size() == 10)
        {
            elements.pop_back();
        }
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