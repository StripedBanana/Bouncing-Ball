#include "Ball.h"
#include "header.h"
#include "Quadtree.h"
#include <time.h>
#include <iostream>
#include <sstream>
#include <math.h>

 using namespace std;

 // Prototypes
int circlesColliding(Ball* b1, Ball* b2); // returns true if both balls are in contact
void handleBallCollision(Ball** b1, Ball** b2);   // handles the collision between both balls

int main()
{
    // Loading the font and setting up fps
    sf::Clock clock;
    sf::Font MyFont;
    if (!MyFont.loadFromFile("arial.ttf")) cout << "Error while loading font file" << endl;
    sf::Text fps("Hello", MyFont, 50);
    fps.setColor(sf::Color::Blue);
    fps.setPosition(0,0);

    // User inputs
    int nbBalls, ballRadius, speed, accY;
    cout << "Enter the balls number: ";
    cin >> nbBalls;
    cout << "Enter the balls radius: ";
    cin >> ballRadius;
    cout << "Enter the speed (between 1 and 10): ";
    cin >> speed;
    cout << "Enter the value of g: ";
    cin >> accY;

    // Variables declarations
    srand(time(NULL));
    int framerate = 60, frameCount = 0;
    double x, y, dx, dy;
    vector<Ball*> balls(nbBalls);
    vector<Ball*> returnBalls(nbBalls);
    vector<sf::CircleShape*> ballShapes(nbBalls);
    Ball* ballTest;
    sf::CircleShape* ballTestShape;
    Quadtree* quad = new Quadtree(0, 0, 0, LENGTH, HEIGHT);
    bool collision;

    // Initializing SFML window
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(LENGTH, HEIGHT), "Bouncing ball", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // Initializing declared arrays
    for(int i=0; i<nbBalls; i++)
    {
        ballShapes[i] = new sf::CircleShape(ballRadius);
        sf::Color color(rand()%255+1, rand()%255+1, rand()%255+1, 255);
        (*ballShapes[i]).setFillColor(color);
        dx = speed*cos(PI*i/nbBalls);
        dy = speed*sin(PI*i/nbBalls);
        x = rand()%(LENGTH-2*ballRadius);
        y = rand()%(HEIGHT-2*ballRadius);
        cout << "dx=" << dx << " dy=" << dy << endl;
        balls[i] = new Ball(x + dx, y + dy, x, y, 0, accY, ballRadius, 1);
    }


    // Process loop
    while (window.isOpen())
    {
        // Event handler
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Resetting the window and the quadtree
        window.clear();
        quad->clr();

        // Loop setting up the quadtree
        for(int i=0; i<nbBalls; i++)
        {
            quad->insertion(balls[i]);
        }

        // Displaying the quadtree
        quad->display(&window);

        // Loop updating objects positions and detecting collisions
        for(int i=0; i<nbBalls; i++)
        {
            balls[i]->updatePos(0.01);
            balls[i]->handleWallCollision();
            (*ballShapes[i]).setPosition(balls[i]->getX(), balls[i]->getY());
            window.draw(*ballShapes[i]);
        }

        // Loop detecting in-between objects collisions
        for(int i=0; i<nbBalls; i++)
        {
            // Running quadtree for list of possible collisions, one object at a time
            ballTest = balls[i];
            returnBalls.erase(returnBalls.begin(), returnBalls.end());
            quad->retrieve(&returnBalls, ballTest);

            // Checking collision and updating speeds of both balls if so
            for (int x = 0; x < returnBalls.size(); x++) {
                // Running collision detection algorithm between objects
                if(ballTest != returnBalls[x]) collision = circlesColliding(ballTest, returnBalls[x]);
                else collision = false;
                if(collision)
                {
                    cout << "avant\t" << "x=" << ballTest->getX() << " y=" << ballTest->getY() << " oldx=" << ballTest->getOldX() << " oldy=" << ballTest->getOldY() << endl;
                    handleBallCollision(&ballTest, &returnBalls[x]);
                    handleBallCollision(&returnBalls[x], &ballTest);
                    cout << "apres\t" << "x=" << ballTest->getX() << " y=" << ballTest->getY() << " oldx=" << ballTest->getOldX() << " oldy=" << ballTest->getOldY() << endl;
                }

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

        // Updating window
        window.display();
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


void handleBallCollision(Ball** b1, Ball** b2)
{
    // Calcul de la base orthonormée (n,g)

    double dx = (*b1)->getX() - (*b2)->getX();
    double dy = (*b1)->getY() - (*b2)->getY();
    double r = sqrt(dx*dx + dy*dy);

    // n est perpendiculaire au plan de collision
    double nx = - dx / r;
    double ny = - dy / r;
    double mn = ny / nx;
    double pn = (*b1)->getY() - mn * (*b1)->getX();

    // g est tangent au plan de collision
    double gx = -ny;
    double gy =  nx;
    double mg = gy / gx;
    double pg = (*b1)->getY() - mg * (*b1)->getX();

    // projection of the old position on the normal
    double mp = mg;
    double pp = (*b1)->getOldY() - mp * (*b1)->getOldX();

    // resolution
    double projectionX = (pn - pp)/(mp - mn);
    double projectionY = mp * projectionX + pp;

    // axial symmetry
    double newX = 2*projectionX - (*b1)->getOldX();
    double newY = 2*projectionY - (*b1)->getOldY();

    //cout << "dx=" << dx << " dy=" << dy << " mg=" << mg << " pg=" << pg << " mp=" << mp << " pp=" << pp << endl;
    //cout << "projectionX=" << projectionX << " projectionY=" << projectionY << endl;
    (*b1)->setOldXY((*b1)->getX(), (*b1)->getY());
    (*b1)->setXY(newX, newY);
    cout << "apres\t" << "x=" << (*b1)->getX() << " y=" << (*b1)->getY() << " oldx=" << (*b1)->getOldX() << " oldy=" << (*b1)->getOldY() << endl;


}

