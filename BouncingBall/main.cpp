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
    balls[0]->setOldXY(LENGTH/2, HEIGHT/2 );
    balls[1]->setXY(LENGTH/2 + 2*ballRadius + 5, HEIGHT/2);
    balls[1]->setOldXY(LENGTH/2 + 2*ballRadius + 5, HEIGHT/2);
    balls[2]->setXY(LENGTH/2 - 200, HEIGHT/2);
    balls[2]->setOldXY(LENGTH/2 - 200 - 10, HEIGHT/2);



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
        cout << "(" << balls[0]->getX() << "," << balls[0]->getY() << ")" << endl;

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
    float x1 = b1->getX(), y1 = b1->getY();
    float x2 = b2->getX(), y2 = b2->getY();
    float r1 = b1->getRadius(), r2 = b2->getRadius();
    bool collision = false;
    float d = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
    if(d < (r1 + r2)*(r1 + r2))
    {
        collision = true;
    }
    return collision;
}


void handleBallCollision(Ball** b1, Ball** b2)
{

    /**
     * Modifies les vecteurs vitesses de deux balles qui entrent en collision.
     */



    float x1 = (*b1)->getX(), y1 = (*b1)->getY();
    float x2 = (*b2)->getX(), y2 = (*b2)->getY();

    float v1x_i = x1 - (*b1)->getOldX(), v1y_i = y1 - (*b1)->getOldY();
    float v2x_i = x2 - (*b2)->getOldX(), v2y_i = y2 - (*b2)->getOldY();

    float radius1 = (*b1)->getRadius(), radius2 = (*b2)->getRadius();
    float m1 = (*b1)->getM(), m2 = (*b2)->getM();


    // Calcul de la base orthonormée (n,g)
    double dx = x1 - x2;
    double dy = y1 - y2;
    double r = sqrt(dx*dx + dy*dy);

    // Correction pour éviter un chevauchement
    /*
    sf::Vector2f mtd(dx * ((radius1+radius2) - r)/r, dy * ((radius1+radius2) - r)/r);
    sf::Vector2f correction(mtd.x * m2/(m2 + m1), mtd.x * m2/(m2 + m1));
    (*b1)->setXY(x1 + correction.x, y1 + correction.y);
    (*b1)->setOldXY((*b1)->getOldX() + correction.x, (*b1)->getOldY() + correction.y);
    (*b2)->setXY(x2 - correction.x, y2 - correction.y);
    (*b2)->setOldXY((*b2)->getOldX() - correction.x, (*b2)->getOldY() - correction.y);

    cout << "correctionX" << correction.x << endl;
    cout << "correctionY" << correction.y << endl << endl;*/

    // n est perpendiculaire au plan de collision
    double nx = (x2 - x1) / r;
    double ny = (y2 - y1) / r;

    // g est tangent au plan de collision
    double gx = -ny;
    double gy =  nx;

    // Transition des vitesses de la base (x,y) vers (n,g)

    double v1n = nx*v1x_i + ny*v1y_i;
    double v1g = gx*v1x_i + gy*v1y_i;
    double v2n = nx*v2x_i + ny*v2y_i;
    double v2g = gx*v2x_i + gy*v2y_i;

    // Détermination des nouvelles vitesses dans (n,g)
    double m = m1 + m2;

    double m12 = (m1 - m2)/m;
    double m22 = (m2 + m2)/m;
    double m11 = (m1 + m1)/m;
    double m21 = (m2 - m1)/m;

    double v1n2 = m12*v1n + m22*v2n; //=v2n if m1=m2
    double v1g2 = m12*v2g + m22*v1g; //=v1g
    double v2n2 = m11*v1n + m21*v2n; //=v1n
    double v2g2 = m11*v2g + m21*v1g; //=v2g

    // Modification des vitesses dans la base (x,y)

    double v1x_f = nx*v1n2 + gx*v1g2;
    double v1y_f = ny*v1n2 + gy*v1g2;
    double v2x_f = nx*v2n2 + gx*v2g2;
    double v2y_f = ny*v2n2 + gy*v2g2;

    float b1x = (*b1)->getX(), b1y = (*b1)->getY();
    float b2x = (*b2)->getX(), b2y = (*b2)->getY();

    (*b1)->setOldXY(b1x, b1y);
    (*b2)->setOldXY(b2x, b2y);

    (*b1)->setXY(b1x + v1x_f, b1y + v1y_f);
    (*b2)->setXY(b2x + v2x_f, b2y + v2y_f);

}

void debug(sf::RenderWindow* window, vector<Ball*> balls)
{
    sf::CircleShape center(balls[0]->getRadius()/10), oldCenter(balls[0]->getRadius()/10);
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

