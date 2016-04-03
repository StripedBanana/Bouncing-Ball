#include "Ball.h"
#include "header.h"
#include "Quadtree.h"
#include <time.h>
#include <iostream>
#include <sstream>

 using namespace std;

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
        returnBalls = quad->retrieve(balls.at(nbBalls-1));
        ballShapes.at(nbBalls-1).setOutlineColor(sf::Color::Black);
        ballShapes.at(nbBalls-1).setOutlineThickness(ballRadius/2);
        //cout << "x=" << balls.at(nbBalls-1)->getX() << " y=" << balls.at(nbBalls-1)->getY() << endl;

        for (int x = 0; x < returnBalls.size(); x++) {
            // Run collision detection algorithm between objects
            returnBallShapes[x].setPosition(returnBalls[x]->getX(), returnBalls[x]->getY());
            returnBallShapes[x].setFillColor(sf::Color::Cyan);
            returnBallShapes[x].setRadius(ballRadius);
            //cout << "x=" << returnBalls[x]->getX() << " y=" << returnBalls[x]->getY() << endl;
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

        cin.ignore();

    }

    return 0;
}
