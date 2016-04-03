#include "Ball.h"
#include "header.h"
#include "Quadtree.h"
#include <time.h>
#include <iostream>
#include <sstream>

 using namespace std;

int circlesColliding(Ball* b1, Ball* b2);

int main()
{
    srand(time(NULL));
    int nbBalls, ballRadius, framerate = 60, frameCount = 0;

    sf::Font MyFont;

    // Loading the font
    if (!MyFont.loadFromFile("arial.ttf"))
    {
        cout << "Error while loading font file" << endl;
    }

    sf::Text fps("Hello", MyFont, 50);

    fps.setColor(sf::Color::Blue);
    fps.setPosition(0,0);

    cout << "Enter the balls number: ";
    cin >> nbBalls;
    cout << "Enter the balls radius: ";
    cin >> ballRadius;
    vector<Ball*> balls(nbBalls);
    vector<Ball*> returnBalls(nbBalls);
    vector<sf::CircleShape> ballShapes(nbBalls);
    vector<sf::CircleShape> returnBallShapes(nbBalls);

    Quadtree* quad = new Quadtree(0, 0, 0, LENGTH, HEIGHT);

    sf::Clock clock;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(LENGTH, HEIGHT), "Bouncing ball", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);
    //window.setFramerateLimit(30);

    for(int i=0; i<nbBalls; i++)
    {
        ballShapes[i].setRadius(ballRadius);
        sf::Color color(255, 0, 0, 255);
        ballShapes[i].setFillColor(color);
        balls[i] = new Ball();
        balls[i]->setRadius(ballRadius);
        balls[i]->setXY(rand()%(LENGTH-2*ballRadius), rand()%(HEIGHT-2*ballRadius));
        balls[i]->setSpeedXY(rand()%(100)/10-5, rand()%(100)/10-5);
    }

    Ball* ballTest = balls[nbBalls-1];
    sf::CircleShape ballTestShape = ballShapes[nbBalls-1];

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        quad->clr();
        for(int i=0; i<nbBalls; i++)
        {
            quad->insertion(balls[i]);
        }

        vector<sf::RectangleShape*> rectangle_array;

        quad->display(&window);

        for(int i=0; i<nbBalls; i++)
        {
            balls[i]->updatePos(0.5);
            balls[i]->handleCollision();
            ballShapes[i].setPosition(balls[i]->getX(), balls[i]->getY());
            window.draw(ballShapes[i]);
        }

        returnBalls.clear();
        returnBalls = quad->retrieve(ballTest);
        ballTestShape = ballShapes[nbBalls-1];
        ballTestShape.setOutlineColor(sf::Color::Black);
        ballTestShape.setOutlineThickness(ballRadius/2);
        window.draw(ballTestShape);

        for (int x = 0; x < returnBalls.size(); x++) {
            // Run collision detection algorithm between objects
            returnBallShapes[x].setPosition(returnBalls[x]->getX(), returnBalls[x]->getY());
            bool collision = circlesColliding(ballTest, returnBalls[x]);
            if(collision) returnBallShapes[x].setFillColor(sf::Color::Blue);
            else returnBallShapes[x].setFillColor(sf::Color::Cyan);
            returnBallShapes[x].setRadius(ballRadius);

            window.draw(returnBallShapes[x]);
        }

        frameCount++;
        if(frameCount > 60)
        {
            framerate = 60 / clock.getElapsedTime().asSeconds();
            clock.restart();
            frameCount = 0;
        }

        stringstream ss;
        ss << framerate;
        fps.setString(ss.str().c_str());
        window.draw(fps);
        window.display();

        //cin.ignore();

    }

    return 0;
}

int circlesColliding(Ball* b1, Ball* b2)
{
    //compare the distance to combined radii
    int x1 = b1->getX()+b1->getRadius(), y1 = b1->getY()+b1->getRadius();
    int x2 = b2->getX()+b2->getRadius(), y2 = b2->getY()+b2->getRadius();
    int dx = x2 - x1;
    int dy = y2 - y1;
    int radii = b1->getRadius() + b2->getRadius();
    if ( (dx * dx)  + (dy * dy) < radii*radii )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
