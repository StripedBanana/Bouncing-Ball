#include "Ball.h"

Ball::Ball()
{
    //ctor
}

void Ball::updatePos(float step)
{
    float accX = 0;
    float accY = -1;

    this->nextX = 2*this->currentX - previousX + accX*step*step;
    this->nextY = 2*this->currentY - previousY + accY*step*step;
}

void Ball::setXY(float x, float y)
{
    this->previousX = x;
    this->previousY = y;
    this->currentX = x;
    this->currentY = y;
    this->nextX = x;
    this->nextY = y;
}

Ball::~Ball()
{
    //dtor
}
