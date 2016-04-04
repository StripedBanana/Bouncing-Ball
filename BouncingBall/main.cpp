#include "Ball.h"
#include "header.h"
#include "Quadtree.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <math.h>

 using namespace std;

int circlesColliding(Ball* b1, Ball* b2);
void handleBallCollision(Ball* b1, Ball* b2);

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
    Ball* ballTest;
    sf::CircleShape ballTestShape;
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

        // Updating balls, ballShapes positions and detecting collisions
        for(int i=0; i<nbBalls; i++)
        {
            balls[i]->updatePos(0.5);
            balls[i]->handleWallCollision();
            ballShapes[i].setPosition(balls[i]->getX(), balls[i]->getY());
            window.draw(ballShapes[i]);
        }

        for(int i=0; i<nbBalls; i++)
        {
            // Running quadtree for list of possible collisions
            ballTest = balls[i];
            returnBalls.erase(returnBalls.begin(), returnBalls.end());
            quad->retrieve(&returnBalls, ballTest);
            ballTestShape = ballShapes[i];
            //ballTestShape.setOutlineColor(sf::Color::Black);
            //ballTestShape.setOutlineThickness(ballRadius/2);
            window.draw(ballTestShape);

            // Checking collision and updating speeds of both balls if so
            bool collision;
            for (int x = 0; x < returnBalls.size(); x++) {
                // Run collision detection algorithm between objects
                returnBallShapes[x].setPosition(returnBalls[x]->getX(), returnBalls[x]->getY());
                if(ballTest != returnBalls[x]) collision = circlesColliding(ballTest, returnBalls[x]);
                else collision = false;
                if(collision)
                {
                    returnBallShapes[x].setFillColor(sf::Color::Blue);
                    handleBallCollision(ballTest, returnBalls[x]);
                    window.draw(returnBallShapes[x]);
                }
                //else returnBallShapes[x].setFillColor(sf::Color::Cyan);
                returnBallShapes[x].setRadius(ballRadius);

            }
        }


        // Displaying fps
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

void handleBallCollision(Ball* b1, Ball* b2)
{
    double dx = b1->getX() - b2->getX();
    double dy = b1->getY() - b2->getY();
    double r = sqrt(dx*dx + dy*dy);

    // n est perpendiculaire au plan de collision
    double nx = - dx / r;
    double ny = - dy / r;

    // g est tangent au plan de collision
    double gx = -ny;
    double gy =  nx;

// Transition des vitesses de la base (x,y) vers (n,g)

    double v1n = nx*b1->getSpdY() + ny*b1->getSpdY();
    double v1g = gx*b1->getSpdX() + gy*b1->getSpdY();
    double v2n = nx*b2->getSpdX() + ny*b2->getSpdY();
    double v2g = gx*b2->getSpdX() + gy*b2->getSpdY();

// Détermination des nouvelles vitesses dans (n,g)

    double m = b1->getM() + b2->getM();

    double m12 = (b1->getM() - b2->getM())/m;
    double m22 = (b2->getM() + b2->getM())/m;
    double m11 = (b1->getM() + b1->getM())/m;
    double m21 = (b2->getM() - b1->getM())/m;

    double v1n2 = m12*v1n + m22*v2n;
    double v1g2 = m12*v1g + m22*v2g;
    double v2n2 = m11*v1n + m21*v2n;
    double v2g2 = m11*v1g + m21*v2g;

// Modification des vitesses dans la base (x,y)

    b1->setSpeedXY(nx*v1n2 + gx*v1g2, ny*v1n2 + gy*v1g2);
    b2->setSpeedXY(nx*v2n2 + gx*v2g2, ny*v2n2 + gy*v2g2);
}
