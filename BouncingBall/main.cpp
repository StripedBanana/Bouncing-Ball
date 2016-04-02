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
    int nbBalls, ballRadius;

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
    vector<sf::CircleShape> ballShapes(nbBalls);

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
        sf::Color color(rand()%155+100, rand()%155+100, rand()%155+100, 255);
        ballShapes[i].setFillColor(color);
        balls[i] = new Ball();
        balls[i]->setRadius(ballRadius);
        balls[i]->setXY(rand()%(LENGTH/3)+LENGTH/3, rand()%(HEIGHT/3)+HEIGHT/3);
        balls[i]->setSpeedXY(5, 5);
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

        int framerate = 1 / clock.getElapsedTime().asSeconds() ;
        clock.restart();

        stringstream ss;
        ss << framerate;
        fps.setString(ss.str().c_str());
        window.draw(fps);
        window.display();



    }

    return 0;
}
