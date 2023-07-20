#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!");
    window.setFramerateLimit(60);

    sf::CircleShape ball(50.f);
    ball.setFillColor(sf::Color::Green);
    ball.setPosition(0.f, 0.f);
    sf::Vector2f velocity(5.f, 5.f);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (ball.getPosition().x + ball.getRadius() * 2 > window.getSize().x || ball.getPosition().x < 0)
            velocity.x = -velocity.x;
        if (ball.getPosition().y + ball.getRadius() * 2 > window.getSize().y || ball.getPosition().y < 0)
            velocity.y = -velocity.y;

        ball.move(velocity);
        window.clear();
        window.draw(ball);
        window.display();
    }
}