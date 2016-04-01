#include <SFML/Graphics.hpp>
#include "Ball.h"
#include "header.h"
#include <time.h>

 using namespace std;

int main()
{
    srand(time(NULL));
    int nbBalls=1000;
    vector<Ball*> balls(nbBalls);
    vector<sf::CircleShape> ballShapes(nbBalls);
    sf::RenderWindow window(sf::VideoMode(LENGTH, HEIGHT), "Bouncing ball");
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(30);
    int ballRadius=10;
    for(int i=0; i<nbBalls; i++)
    {
        ballShapes[i].setRadius(ballRadius);
        sf::Color color(rand()%255+1, rand()%255+1, rand()%255+1);
        ballShapes[i].setFillColor(color);
        balls[i] = new Ball();
        balls[i]->setRadius(ballRadius);
        balls[i]->setXY(rand()%(LENGTH/2)+1, rand()%(HEIGHT/2)+1);
        balls[i]->setSpeedXY(rand()%10+1,rand()%10+1);
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
            balls[i]->updatePos(0.2);
            balls[i]->handleCollision();
            ballShapes[i].setPosition(balls[i]->getX(), balls[i]->getY());
            window.draw(ballShapes[i]);
        }
        window.display();
    }

    return 0;
}
