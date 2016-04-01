#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "header.h"
#include <time.h>
#include <iostream>

 using namespace std;

int main()
{
    srand(time(NULL));
    int nbBalls, ballRadius;
    cout << "Enter the balls number: ";
    cin >> nbBalls;
    cout << "Enter the balls radius: ";
    cin >> ballRadius;
    vector<Ball*> balls(nbBalls);
    vector<sf::CircleShape> ballShapes(nbBalls);

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(LENGTH, HEIGHT), "Bouncing ball", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(30);
    for(int i=0; i<nbBalls; i++)
    {
        ballShapes[i].setRadius(ballRadius);
        sf::Color color(rand()%155+100, rand()%155+100, rand()%155+100, 255);
        ballShapes[i].setFillColor(color);
        balls[i] = new Ball();
        balls[i]->setRadius(ballRadius);
        balls[i]->setXY(LENGTH/2-ballRadius, HEIGHT/2-ballRadius);
        balls[i]->setSpeedXY(int(rand()%2000)/100-10,int(rand()%2000)/100-10);
    }

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        for(int i=0; i<nbBalls; i++)
        {
            balls[i]->updatePos(0.5);
            balls[i]->handleCollision();
            ballShapes[i].setPosition(balls[i]->getX(), balls[i]->getY());
            window.draw(ballShapes[i]);
        }
        window.display();
    }

    return 0;
}
