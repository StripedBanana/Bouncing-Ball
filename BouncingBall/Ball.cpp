#include "Ball.h"
#include "header.h"
#include <iostream>

using namespace std;

Ball::Ball()
{
    this->previousX = 0;
    this->previousY = 0;
    this->currentX = 0;
    this->currentY = 0;
    this->speedX = 0;
    this->speedY = 0;
    this->previousSpeedX = 0;
    this->previousSpeedY = 0;
    this->accX = 0;
    this->accY = 1;
    this->radius = 10;
}

void Ball::updatePos(float step)
{

/*  Verlet integration
    this->nextX = 2*this->currentX - previousX + accX*step*step;
    this->nextY = 2*this->currentY - previousY + accY*step*step;

    this->previousX = this->currentX;
    this->previousY = this->currentY;

    this->currentX = this->nextX;
    this->currentY = this->nextY;
*/

    this->speedX = this->previousSpeedX + this->accX*step;
    this->speedY = this->previousSpeedY + this->accY*step;

    this->currentX = this->previousX + this->speedX*step;
    this->currentY = this->previousY + this->speedY*step;

    this->previousX = this->currentX;
    this->previousY = this->currentY;

    this->previousSpeedX = this->speedX;
    this->previousSpeedY = this->speedY;

    cout << "x=" << currentX << " y=" << currentY << endl;
    cout << "sx=" << previousSpeedX << " sy=" << previousSpeedY << endl;
}

void Ball::handleCollision()
{
    float X = this->currentX, Y = this->currentY, rad = this->radius;
    if(X > (LENGTH - 2*rad) || X < 0) this->previousSpeedX = -this->previousSpeedX;
    if(Y > (HEIGHT - 2*rad) || Y < 0) this->previousSpeedY = -this->previousSpeedY;
}

void Ball::setRadius(float newr)
{
    this->radius=newr;
}

void Ball::setXY(float x, float y)
{
    this->previousX = x;
    this->previousY = y;
    this->currentX = x;
    this->currentY = y;
}

void Ball::setSpeedXY(float sx, float sy)
{
    this->previousSpeedX = sx;
    this->previousSpeedY = sy;
}

void Ball::setAccXY(float newaccx, float newaccy)
{
    this->accX = newaccx;
    this->accY = newaccy;
}

Ball::~Ball()
{
    //dtor
}
