#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <cmath>
#include <functional>

#include "FontManager.h"
#include "SoundManager.h"

//TODO: REFACTOR ALL THIS STUFF, FOR NOW IT'S JUST A TEMPORARY SOLUTION

const float GLOBAL_OUTLINE = 3.f;
const sf::Color TEST_ORANGE_COLOR = sf::Color(123, 176, 230);
const unsigned int CHARACTER_SIZE = 20;


enum class GUIElementType
{
    Button
};

template<typename ShapeType>
concept IsSFMLShape = requires(ShapeType shape) {
    { shape.setFillColor(sf::Color::White) } -> std::same_as<void>;
    { shape.setOutlineColor(sf::Color::White) } -> std::same_as<void>;
    { shape.setOutlineThickness(GLOBAL_OUTLINE) } -> std::same_as<void>;
    { shape.setOrigin({0.0f, 0.0f}) } -> std::same_as<void>;
    { shape.setPosition({0.0f, 0.0f}) } -> std::same_as<void>;
    { shape.setRotation(0.0f) } -> std::same_as<void>;
};

template<typename T>
struct IsRectangleShape {
    static constexpr bool value = std::is_same_v<T, sf::RectangleShape>;
};

template<typename T>
concept IsRectangle = IsRectangleShape<T>::value;

template<typename T>
struct IsCircleShape {
    static constexpr bool value = std::is_same_v<T, sf::CircleShape>;
};

template<typename T>
concept IsCircle = IsCircleShape<T>::value;

template<typename T>
struct IsConvexShape {
    static constexpr bool value = std::is_same_v<T, sf::ConvexShape>;
};

template<typename T>
concept IsConvex = IsConvexShape<T>::value;

template<IsSFMLShape ShapeType>
class ShapeBuilder
{
public:
    ShapeBuilder()
    {
        shape = std::make_unique<ShapeType>();
    }

    ShapeBuilder& setPosition(const sf::Vector2f& position)
    {
        shape->setPosition(position);
        return *this;
    }
    ShapeBuilder& setRotation(float rotation)
    {
        shape->setRotation(rotation);
        return *this;
    }
    ShapeBuilder& setFillColor(const sf::Color& color)
    {
        shape->setFillColor(color);
        return *this;
    }
    ShapeBuilder& setOutlineColor(const sf::Color& color)
    {
        shape->setOutlineColor(color);
        return *this;
    }
    ShapeBuilder& setOutlineThickness(float thickness)
    {
        shape->setOutlineThickness(thickness);
        return *this;
    }
    ShapeBuilder& setOrigin(const sf::Vector2f& origin)
    {
        shape->setOrigin(origin);
        return *this;
    }
    ShapeBuilder& setSize(const sf::Vector2f& size) requires IsRectangle<ShapeType>
    {
        static_assert(IsRectangle<ShapeType>, "setSize is only available for sf::RectangleShape");
        static_cast<sf::RectangleShape*>(shape.get())->setSize(size);
        return *this;
    }
    ShapeBuilder& setRadius(float radius) requires IsCircle<ShapeType>
    {
        static_assert(IsCircle<ShapeType>, "setRadius is only available for sf::CircleShape");
        static_cast<sf::CircleShape*>(shape.get())->setRadius(radius);
        return *this;
    }
    ShapeBuilder& setPointCount(unsigned int count) requires IsConvex<ShapeType>
    {
        static_assert(IsConvex<ShapeType>, "setPointCount is only available for sf::ConvexShape");
        static_cast<sf::ConvexShape*>(shape.get())->setPointCount(count);
        return *this;
    }
    ShapeBuilder& setPoint(unsigned int index, const sf::Vector2f& point) requires IsConvex<ShapeType>
    {
        static_assert(IsConvex<ShapeType>, "setPoint is only available for sf::ConvexShape");
        static_cast<sf::ConvexShape*>(shape.get())->setPoint(index, point);
        return *this;
    }

    std::unique_ptr<sf::Shape> build()
    {
        return std::move(shape);
    }

private:
    std::unique_ptr<sf::Shape> shape;
};

class ShapeFactory
{
public:
    static std::unique_ptr<sf::Shape> createCorner(const sf::Vector2f& position, float rotation, float thickness = 5, float size = 40)
    {
        ShapeBuilder<sf::ConvexShape> builder;
        builder.setPointCount(6)
            .setPoint(0, sf::Vector2f(0, 0))
            .setPoint(1, sf::Vector2f(size, 0))
            .setPoint(2, sf::Vector2f(size, thickness))
            .setPoint(3, sf::Vector2f(thickness, thickness))
            .setPoint(4, sf::Vector2f(thickness, size))
            .setPoint(5, sf::Vector2f(0, size))
            .setRotation(rotation)
            .setFillColor(sf::Color::Transparent)
            .setOutlineColor(sf::Color(50, 50, 50, 255))
            .setOutlineThickness(1)
            .setOrigin({0, 0})
            .setPosition(position);

        return std::move(builder.build());
    }

    static std::unique_ptr<sf::Shape> createButtonArrow(const sf::Vector2f& position, float rotation, float thickness = 5, float size = 40)
    {
        ShapeBuilder<sf::ConvexShape> builder;
        builder.setPointCount(3)
            .setPoint(0, sf::Vector2f(0, 0))
            .setPoint(1, sf::Vector2f(size, size / 2))
            .setPoint(2, sf::Vector2f(0, size))
            .setRotation(rotation)
            .setFillColor(sf::Color::Black)
            .setOutlineColor(sf::Color::White)
            .setOutlineThickness(1)
            .setOrigin(
                {size / 2, size / 2}
            )
            .setPosition(position);

        return std::move(builder.build());
    }

    static std::unique_ptr<sf::Shape> createBasicRectangle(const sf::Vector2f& position, const sf::Vector2f& size)
    {
        ShapeBuilder<sf::RectangleShape> builder;
        builder.setSize(size)
            .setFillColor(sf::Color::Transparent)
            .setOrigin({size.x / 2, size.y / 2})
            .setPosition(position);

        return std::move(builder.build());
    }

    static std::unique_ptr<sf::Shape> createLine(const sf::Vector2f& A, const sf::Vector2f& B, float thickness)
    {
        sf::Vector2f direction = B - A;
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        float angle = std::atan2(direction.y, direction.x) * 180 / M_PI;

        ShapeBuilder<sf::RectangleShape> builder;
        builder.setSize({length, thickness})
            .setFillColor(sf::Color(50, 50, 50, 255))
            .setOrigin({0, thickness / 2})
            .setPosition(A)
            .setRotation(angle);

        return std::move(builder.build());
    }
};

enum class EffectType
{
    Idle,
    Hover,
    Active
};

class GUIEffect
{
public:
    GUIEffect(sf::Shape* shape, EffectType type) : shape(shape), type(type) {}
    virtual void update(sf::Time deltaTime, bool active) = 0;
    EffectType getType() const { return type; }

protected:
    sf::Shape* shape;
    EffectType type;
};

class OutlineFadeEffect : public GUIEffect
{
public:
    OutlineFadeEffect(sf::Shape* shape, EffectType type) : GUIEffect(shape, type)
    {
        this->fadeIn = false;
        this->outlineOpacity = shape->getOutlineColor().a;
    }

    void update(sf::Time deltaTime, bool active) override
    {

        auto outlineColor = this->shape->getOutlineColor();

        if (fadeIn)
        {
            outlineOpacity += this->speed * deltaTime.asSeconds();
            if (outlineOpacity >= maxRange)
            {
                outlineOpacity = maxRange;
                fadeIn = false;
            }
        }
        else
        {
            outlineOpacity -= this->speed * deltaTime.asSeconds();
            if (outlineOpacity <= minRange)
            {
                outlineOpacity = minRange;
                fadeIn = true;
            }
        }

        if (active)
        {
            this->shape->setOutlineColor(sf::Color(outlineColor.r, outlineColor.g, outlineColor.b, outlineOpacity));
        }
    }

private:
    bool fadeIn = false;
    float outlineOpacity;
    //TODO: make this configurable
    float speed = 200.f;
    float minRange = 50.f;
    float maxRange = 255.f;
};

class PlaySoundEffect : public GUIEffect
{
public:

    PlaySoundEffect(sf::Shape* shape, EffectType type, const std::string& soundName) : GUIEffect(shape, type), soundName(soundName) {}

    void update(sf::Time deltaTime, bool active) override
    {
        if (active)
        {
            if (!this->soundPlayed)
            {
                this->soundPlayed = true;
                SoundManager::getInstance().playSound(soundName);
            }
        }
        else
        {
            this->soundPlayed = false;
        }
    }
private:
    std::string soundName;
    bool soundPlayed = false;
};

class MoveOutOnHover : public GUIEffect
{
public:
    MoveOutOnHover(sf::Shape* shape, EffectType type, float effectStrength) : GUIEffect(shape, type)
    {
        this->basePosition = shape->getPosition();
        this->effectStrength = effectStrength;
    }

    void update(sf::Time deltaTime, bool active) override
    {
        if (active)
        {
            float angle = std::fmod(this->shape->getRotation(), 360.f);
            float offsetX = this->effectStrength * std::cos(2 * angle * (M_PI / 180.f)) * (angle >= 180.f ? 1 : -1);
            float offsetY = -this->effectStrength * std::sin(2* angle * (M_PI / 180.f) - (M_PI / 2)) * (angle >= 90.f && angle < 270.f ? 1 : -1);
            shape->setPosition(basePosition + sf::Vector2f(offsetX, offsetY));

        }
        else
        {
            shape->setPosition(basePosition);
        }
    }

private:
    sf::Vector2f basePosition;
    float effectStrength;
};

class ChangeOutlineColorEffectOnHover : public GUIEffect
{
public:
    ChangeOutlineColorEffectOnHover(sf::Shape* shape, EffectType type, const sf::Color& color) : GUIEffect(shape, type), color(color)
    {
        this->originalColor = shape->getOutlineColor();
    }

    void update(sf::Time deltaTime, bool active) override
    {
        if (active)
        {
            if (!this->colorSet)
            {
                this->alpha = shape->getOutlineColor().a;
                shape->setOutlineColor(sf::Color(
                    color.r,
                    color.g,
                    color.b,
                    255
                ));

                this->colorSet = true;
            }
        }
        else
        {
            if (this->colorSet)
            {
                shape->setOutlineColor(sf::Color(
                    originalColor.r,
                    originalColor.g,
                    originalColor.b,
                    this->alpha
                ));

                this->colorSet = false;
            }
        }
    }

private:
    sf::Color color;
    sf::Color originalColor;
    sf::Uint8 alpha;
    bool colorSet = false;
};

class ChangeFillColor : public GUIEffect
{
public:
    ChangeFillColor(sf::Shape* shape, EffectType type, const sf::Color& color) : GUIEffect(shape, type), color(color)
    {
        this->originalColor = shape->getFillColor();
    }

    void update(sf::Time deltaTime, bool active) override
    {
        if (active)
        {
            this->shape->setFillColor(this->color);
        }
    }

private:
    sf::Color color;
    sf::Color originalColor;
};

class ChangeTextColor : public GUIEffect
{
public:
    ChangeTextColor(sf::Shape* shape, EffectType type, sf::Text& text, const sf::Color& color) : GUIEffect(shape, type), text(text), color(color)
    {
        this->originalColor = text.getFillColor();
    }

    void update(sf::Time deltaTime, bool active) override
    {
        if (active)
        {
            this->text.setFillColor(this->color);
        }
        else
        {
            this->text.setFillColor(this->originalColor);
        }
    }

private:
    sf::Text& text;
    sf::Color color;
    sf::Color originalColor;
};

class GUIShape : public sf::Drawable
{
public:
    GUIShape(std::unique_ptr<sf::Shape> shape);
    void update(sf::Time deltaTime, sf::Vector2f mousePosition, EffectType type = EffectType::Idle);
    void addEffect(std::unique_ptr<GUIEffect> effect);
    sf::FloatRect getGlobalBounds() const { return shape->getGlobalBounds(); }
    void setPosition(const sf::Vector2f& position) { shape->setPosition(position); }
    sf::Shape* getShapePointer() { return shape.get(); }
    void setAsInteractiveElement(std::function<void()> action) { this->interactiveElement = true; this->onActivateAction = action; }

protected:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    std::unique_ptr<sf::Shape> shape;
    std::vector<std::unique_ptr<GUIEffect>> effects;

    sf::Vector2f basePosition;


    float outlineOpacity;
    bool fadeIn;

    bool interactiveElement = false;
    std::function<void()> onActivateAction;

    bool isClicked = false;
};

class GUIShapeText : public GUIShape
{
public:
    using GUIShape::GUIShape;

    void setText(const std::string& text, unsigned int characterSize = CHARACTER_SIZE)
    {
        font = FontManager::getInstance().getFont("font");

        sf::Text newText;
        newText.setFont(font);
        newText.setString(text);

        newText.setCharacterSize(characterSize);

        sf::FloatRect textBounds = newText.getLocalBounds();
        sf::FloatRect shapeBounds = getGlobalBounds();
        newText.setOrigin(textBounds.left + textBounds.width / 2.0f,
                          textBounds.top + textBounds.height / 2.0f);
        newText.setPosition(shapeBounds.left + shapeBounds.width / 2.0f,
                            shapeBounds.top + shapeBounds.height / 2.0f);

        newText.setFillColor(sf::Color::White);

        textObject = std::move(newText);
    }

    void setText(unsigned int index)
    {
        if (index < texts.size())
        {
            currentIndex = index;
            setText(texts[index]);
        }
    }

    unsigned int getTextIndex() const { return currentIndex; }

    void setTexts(const std::vector<std::string>& texts, unsigned int characterSize = CHARACTER_SIZE)
    {
        this->texts = texts;
    }

    sf::Text& getTextObject() { return textObject; }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
    {
        GUIShape::draw(target, states);

        target.draw(textObject, states);
    }

    unsigned int currentIndex = 0;

private:
    std::vector<std::string> texts;
    sf::Text textObject;
    sf::Font font;
};

class GUIElement : public sf::Drawable
{
public:
    using ActionFunction = std::function<void()>;

    void setOnActivateAction(ActionFunction function)
    {
        this->onActivateAction = function;
    }

    void activate()
    {
        if (onActivateAction)
            onActivateAction();
    }

    std::string getText() const
    {
        return static_cast<GUIShapeText*>(shapes[0].get())->getTextObject().getString();
    }

    void setPosition(const sf::Vector2f& position);
    
    void setType(GUIElementType type);
    GUIElementType getType() const;

    void addShape(std::unique_ptr<GUIShape> shape);

    void update(sf::Time deltaTime, sf::Vector2f mousePosition);
private:
    ActionFunction onActivateAction;
    bool isActive = false;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    GUIElementType type;

    std::vector<std::unique_ptr<GUIShape>> shapes;
};

class GUIElementBuilder
{
public:
    GUIElementBuilder& setPosition(const sf::Vector2f& position);
    GUIElementBuilder& setType(GUIElementType type);
    GUIElementBuilder& addShape(std::unique_ptr<GUIShape> shape);

    GUIElement build();
private:
    GUIElement element;
};

class GUIElementFactory
{
public:
    static std::unique_ptr<GUIElement> createButton(const sf::Vector2f& position, const sf::Vector2f& size = sf::Vector2f(300, 50), std::string text = "", float outline = GLOBAL_OUTLINE, unsigned int characterSize = CHARACTER_SIZE)
    {
        float buttonMargin = std::sqrt(std::min(size.x, size.y) / 10);
        float cornerSize = std::min(size.x, size.y) / 4;

        std::unique_ptr<GUIShapeText> base = std::make_unique<GUIShapeText>(ShapeFactory::createBasicRectangle(position, sf::Vector2f(size.x - buttonMargin * 2, size.y - buttonMargin * 2)));
        base->addEffect(std::make_unique<PlaySoundEffect>(base->getShapePointer(), EffectType::Hover, "ButtonHover"));
        //TODO: sound effect on click is temporary
        base->addEffect(std::make_unique<PlaySoundEffect>(base->getShapePointer(), EffectType::Active, "SkillUnlock"));


        if (!text.empty())
        {
            base->setText(text, characterSize);
            base->addEffect(std::make_unique<ChangeTextColor>(base->getShapePointer(), EffectType::Active, base->getTextObject(), TEST_ORANGE_COLOR));
        }

        std::unique_ptr<GUIShape> corner1 = std::make_unique<GUIShape>(ShapeFactory::createCorner(position - sf::Vector2f(size.x / 2, size.y / 2), 0.0f, outline, cornerSize));
        std::unique_ptr<GUIShape> corner2 = std::make_unique<GUIShape>(ShapeFactory::createCorner(position + sf::Vector2f(size.x, 0) - sf::Vector2f(size.x / 2, size.y / 2), 90.0f, outline, cornerSize));
        std::unique_ptr<GUIShape> corner3 = std::make_unique<GUIShape>(ShapeFactory::createCorner(position + sf::Vector2f(size.x, size.y) - sf::Vector2f(size.x / 2, size.y / 2), 180.0f, outline, cornerSize));
        std::unique_ptr<GUIShape> corner4 = std::make_unique<GUIShape>(ShapeFactory::createCorner(position + sf::Vector2f(0, size.y) - sf::Vector2f(size.x / 2, size.y / 2), 270.0f, outline, cornerSize));

        std::unique_ptr<GUIShape> line1 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(cornerSize + buttonMargin, outline / 2),
            position + sf::Vector2f(size.x, 0) - sf::Vector2f(size.x / 2, size.y / 2) - sf::Vector2f(cornerSize + buttonMargin, -outline / 2),
            2.f
        ));

        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> line2 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position + sf::Vector2f(size.x, 0) - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(-outline / 2, cornerSize + buttonMargin),
            position + sf::Vector2f(size.x, size.y) - sf::Vector2f(size.x / 2, size.y / 2) - sf::Vector2f(outline / 2, cornerSize + buttonMargin),
            2.f
        ));

        line2->addEffect(std::make_unique<ChangeFillColor>(line2->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line2->addEffect(std::make_unique<ChangeFillColor>(line2->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line2->addEffect(std::make_unique<ChangeFillColor>(line2->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> line3 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position + sf::Vector2f(size.x, size.y) - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(-cornerSize - buttonMargin, -outline / 2),
            position + sf::Vector2f(0, size.y) - sf::Vector2f(size.x / 2, size.y / 2) - sf::Vector2f(-cornerSize - buttonMargin, outline / 2),
            2.f
        ));

        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> line4 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position + sf::Vector2f(0, size.y) - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(outline / 2, -cornerSize - buttonMargin),
            position - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(outline / 2, cornerSize + buttonMargin),
            2.f
        ));

        line4->addEffect(std::make_unique<ChangeFillColor>(line4->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line4->addEffect(std::make_unique<ChangeFillColor>(line4->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line4->addEffect(std::make_unique<ChangeFillColor>(line4->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));


        corner1->addEffect(std::make_unique<MoveOutOnHover>(corner1->getShapePointer(), EffectType::Hover, buttonMargin));
        corner1->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(corner1->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        corner2->addEffect(std::make_unique<MoveOutOnHover>(corner2->getShapePointer(), EffectType::Hover, buttonMargin));
        corner2->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(corner2->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        corner3->addEffect(std::make_unique<MoveOutOnHover>(corner3->getShapePointer(), EffectType::Hover, buttonMargin));
        corner3->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(corner3->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        corner4->addEffect(std::make_unique<MoveOutOnHover>(corner4->getShapePointer(), EffectType::Hover, buttonMargin));
        corner4->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(corner4->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        GUIElementBuilder builder;
        builder
            .setPosition(position)
            .setType(GUIElementType::Button)
            .addShape(std::move(base))
            .addShape(std::move(corner1))
            .addShape(std::move(corner2))
            .addShape(std::move(corner3))
            .addShape(std::move(corner4))
            .addShape(std::move(line1))
            .addShape(std::move(line2))
            .addShape(std::move(line3))
            .addShape(std::move(line4));

        // return builder.build();
        return std::make_unique<GUIElement>(builder.build());
    }

    static std::unique_ptr<GUIElement> createArrowButton(const sf::Vector2f& position, const sf::Vector2f& size = sf::Vector2f(300, 50), std::vector<std::string> texts = {}, unsigned int index = 0)
    {
        float buttonMargin = std::sqrt(std::min(size.x, size.y) / 10);
        float cornerSize = std::min(size.x, size.y) / 4;

        std::unique_ptr<GUIShapeText> base = std::make_unique<GUIShapeText>(ShapeFactory::createBasicRectangle(position, sf::Vector2f(size.x - buttonMargin * 2, size.y - buttonMargin * 2)));
        
        base->setTexts(texts);
        base->setText(index);
        base->addEffect(std::make_unique<PlaySoundEffect>(base->getShapePointer(), EffectType::Hover, "ButtonHover"));

        std::unique_ptr<GUIShape> line1 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(cornerSize + buttonMargin, GLOBAL_OUTLINE / 2),
            position + sf::Vector2f(size.x, 0) - sf::Vector2f(size.x / 2, size.y / 2) - sf::Vector2f(cornerSize + buttonMargin, -GLOBAL_OUTLINE / 2),
            2.f
        ));

        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line1->addEffect(std::make_unique<ChangeFillColor>(line1->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> line3 = std::make_unique<GUIShape>(ShapeFactory::createLine(
            position + sf::Vector2f(size.x, size.y) - sf::Vector2f(size.x / 2, size.y / 2) + sf::Vector2f(-cornerSize - buttonMargin, -GLOBAL_OUTLINE / 2),
            position + sf::Vector2f(0, size.y) - sf::Vector2f(size.x / 2, size.y / 2) - sf::Vector2f(-cornerSize - buttonMargin, GLOBAL_OUTLINE / 2),
            2.f
        ));

        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Idle, sf::Color(50, 50, 50, 255)));
        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        line3->addEffect(std::make_unique<ChangeFillColor>(line3->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> arrowRight = std::make_unique<GUIShape>(ShapeFactory::createButtonArrow(
            sf::Vector2f(position.x + size.x / 2 - buttonMargin - cornerSize, position.y),
            0.0f,
            GLOBAL_OUTLINE,
            cornerSize * 1.5
        ));

        arrowRight->setAsInteractiveElement(
            [base = base.get()]() mutable {
                base->setText(base->getTextIndex() + 1);
            }
        );
        arrowRight->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(arrowRight->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
       
        arrowRight->addEffect(std::make_unique<ChangeFillColor>(arrowRight->getShapePointer(), EffectType::Idle, sf::Color::Black));
        arrowRight->addEffect(std::make_unique<ChangeFillColor>(arrowRight->getShapePointer(), EffectType::Hover, sf::Color::Black));
        arrowRight->addEffect(std::make_unique<ChangeFillColor>(arrowRight->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        std::unique_ptr<GUIShape> arrowLeft = std::make_unique<GUIShape>(ShapeFactory::createButtonArrow(
            sf::Vector2f(position.x - size.x / 2 + buttonMargin + cornerSize, position.y),
            180.0f,
            GLOBAL_OUTLINE,
            cornerSize * 1.5
        ));

        arrowLeft->setAsInteractiveElement(
            [base = base.get()]() mutable {
                base->setText(base->getTextIndex() - 1);
            }
        );
        arrowLeft->addEffect(std::make_unique<ChangeOutlineColorEffectOnHover>(arrowLeft->getShapePointer(), EffectType::Hover, TEST_ORANGE_COLOR));
        
        arrowLeft->addEffect(std::make_unique<ChangeFillColor>(arrowLeft->getShapePointer(), EffectType::Idle, sf::Color::Black));
        arrowLeft->addEffect(std::make_unique<ChangeFillColor>(arrowLeft->getShapePointer(), EffectType::Hover, sf::Color::Black));
        arrowLeft->addEffect(std::make_unique<ChangeFillColor>(arrowLeft->getShapePointer(), EffectType::Active, TEST_ORANGE_COLOR));

        GUIElementBuilder builder;
        builder
            .setPosition(position)
            .setType(GUIElementType::Button)
            .addShape(std::move(base))
            .addShape(std::move(line1))
            .addShape(std::move(line3))
            .addShape(std::move(arrowRight))
            .addShape(std::move(arrowLeft));

        return std::make_unique<GUIElement>(builder.build());
    }

    static std::unique_ptr<GUIElement> createText(const sf::Vector2f& position, std::string text = "", unsigned int characterSize = CHARACTER_SIZE)
    {
        std::unique_ptr<GUIShapeText> base = std::make_unique<GUIShapeText>(ShapeFactory::createBasicRectangle(position, sf::Vector2f(1, 1)));

        if (!text.empty())
        {
            base->setText(text, characterSize);
        }
        else
        {
            throw std::runtime_error("Text in createText cannot be empty");
        }

        GUIElementBuilder builder;
        builder
            .setPosition(position)
            .setType(GUIElementType::Button)
            .addShape(std::move(base));

        return std::make_unique<GUIElement>(builder.build());
    }
};