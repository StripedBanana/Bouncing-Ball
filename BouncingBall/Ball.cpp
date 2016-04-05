#include "Ball.h"
#include "header.h"
#include <iostream>

using namespace std;

Ball::Ball()
{
    setXY(0,0);
    setAccXY(0,0);
    setOldXY(0,0);
    this->radius = 10;
    this->mass = 1;
}

Ball::Ball(float x, float y, float ox, float oy, float accx, float accy, float radius, float mass)
{
    setXY(x,y);
    setAccXY(accx,accy);
    setOldXY(ox,oy);
    this->radius = radius;
    this->mass = mass;
}

void Ball::updatePos(float step)
{

    // Verlet integration
    sf::Vector2f temp = pos;
    pos.x += pos.x - oldPos.x + acc.x*step;
    pos.y += pos.y - oldPos.y + acc.y*step;
    oldPos = temp;

/* Euler
    this->speedX = this->previousSpeedX + this->accX*step;
    this->speedY = this->previousSpeedY + this->accY*step;

    this->currentX = this->previousX + this->speedX*step;
    this->currentY = this->previousY + this->speedY*step;

    this->previousX = this->currentX;
    this->previousY = this->currentY;

    this->previousSpeedX = this->speedX;
    this->previousSpeedY = this->speedY;
*/
}

void Ball::handleWallCollision()
{
    if (pos.x <= 0) {
        oldPos.x *= -1;
        pos.x *= -1;
    }
    if (pos.x >= (LENGTH-2*radius)) {
        oldPos.x = LENGTH-2*radius + (LENGTH-2*radius - oldPos.x);
        pos.x = LENGTH-2*radius + (LENGTH-2*radius - pos.x);
    }
    if (pos.y <= 0) {
        oldPos.y *= -1;
        pos.y *= -1;
    }
    if (pos.y >= (HEIGHT-2*radius)) {
        oldPos.y = HEIGHT-2*radius + (HEIGHT-2*radius - oldPos.y);
        pos.y = HEIGHT-2*radius + (HEIGHT-2*radius - pos.y);
    }
}

void Ball::setRadius(float newr)
{
    this->radius=newr;
}

void Ball::setXY(float x, float y)
{
    pos.x = x;
    pos.y = y;
}

void Ball::setOldXY(float x, float y)
{
    oldPos.x = x;
    oldPos.y = y;
}

void Ball::setAccXY(float newaccx, float newaccy)
{
    acc.x = newaccx;
    acc.y = newaccy;
}

Ball::~Ball()
{
    //dtor
}
