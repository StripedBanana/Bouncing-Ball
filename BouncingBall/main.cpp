#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "header.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(LENGTH, HEIGHT), "Bouncing ball");
    float ballRadius=10;
    sf::CircleShape ballShape(ballRadius);
    ballShape.setFillColor(sf::Color::Green);
    Ball* ball = new Ball();
    ball->setRadius(ballRadius);
    ball->setXY(300.0, 200.0);
    ball->setSpeedXY(0,1);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        ball->updatePos(0.2);
        ball->handleCollision();
        ballShape.setPosition(ball->getX(), ball->getY());
        window.clear();
        window.draw(ballShape);
        window.display();
    }

    return 0;
}
