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
void debug(sf::RenderWindow* window, vector<Ball*> balls); // displaying center & old center of the balls

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

    balls[0]->setXY(LENGTH/2, HEIGHT/2);
    balls[0]->setOldXY(LENGTH/2 - 1, HEIGHT/2 - 1);
    balls[1]->setXY(LENGTH/2 + 100, HEIGHT/2 + 100);
    balls[1]->setOldXY(LENGTH/2 + 100 - 0.5, HEIGHT/2 + 100 - 0.5);


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
                    cout << "collision" << endl;
                    handleBallCollision(&ballTest, &returnBalls[x]);
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
        debug(&window, balls);
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

    /*
    // Calcul de la base orthonormée (n,g)
    double dx = (*b1)->getX() - (*b2)->getX();
    double dy = (*b1)->getY() - (*b2)->getY();
    double r = sqrt(dx*dx + dy*dy);

    if(abs((long long)dx)<0.1)
    {
        cout << "dx=" << dx << endl;
        cin.ignore();
        cin.ignore();
    }
    if(abs((long long)dy)<0.1)
    {
        cout << "dy=" << dy << endl;
        cin.ignore();
        cin.ignore();
    }
    // n est perpendiculaire au plan de collision
    double nx = -dx;
    double ny = -dy;
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

    // setting up the modifications
    (*b1)->setOldXY((*b1)->getX(), (*b1)->getY());
    (*b1)->setXY(newX, newY);

    // second object
    pg = (*b2)->getY() - mg * (*b2)->getX();
    pp = (*b2)->getOldY() - mp * (*b2)->getOldX();
    projectionX = (pn - pp)/(mp - mn);
    projectionY = mp * projectionX + pp;
    newX = 2*projectionX - (*b2)->getOldX();
    newY = 2*projectionY - (*b2)->getOldY();
    (*b2)->setOldXY((*b2)->getX(), (*b2)->getY());
    (*b2)->setXY(newX, newY);
    */

    float m1 = (*b1)->getM(), m2 = (*b2)->getM();
    float v1x_i = (*b1)->getX() - (*b1)->getOldX(), v1y_i = (*b1)->getY() - (*b1)->getOldY();
    float v2x_i = (*b2)->getX() - (*b2)->getOldX(), v2y_i = (*b2)->getY() - (*b2)->getOldY();
    float v1x_f = (m1-m2)/(m1+m2)*v1x_i + 2*m2/(m1+m2)*v2x_i, v1y_f = (m1-m2)/(m1+m2)*v1y_i + 2*m2/(m1+m2)*v2y_i;
    float v2x_f = 2*m1/(m1+m2)*v1x_i + (m2-m1)/(m1+m2)*v2x_i, v2y_f = 2*m1/(m1+m2)*v1y_i + (m2-m1)/(m1+m2)*v2y_i;

    cout << "v1x_i=" << v1x_i << " v2x_i=" << v2x_i << endl;
    cout << "v1x_f=" << v1x_f << " v2x_f=" << v2x_f << endl;

    float b1x = (*b1)->getX(), b1y = (*b1)->getY();
    float b2x = (*b2)->getX(), b2y = (*b2)->getY();

    (*b1)->setOldXY(b1x, b1y);
    (*b2)->setOldXY(b2x, b2y);

    (*b1)->setXY(b1x + v1x_f, b1y + v1y_f);
    (*b2)->setXY(b2x + v2x_f, b2y + v2y_f);

}

void debug(sf::RenderWindow* window, vector<Ball*> balls)
{
    sf::CircleShape center(balls[0]->getRadius()/10), oldCenter(balls[0]->getRadius()/10


                                                                );
    center.setFillColor(sf::Color::Green);
    oldCenter.setFillColor(sf::Color::Red);
    for(int i=0; i<balls.size(); i++)
    {
        center.setPosition(balls[i]->getX(), balls[i]->getY());
        oldCenter.setPosition(balls[i]->getOldX(), balls[i]->getOldY());
        window->draw(center);
        window->draw(oldCenter);
    }
}

