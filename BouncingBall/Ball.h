#ifndef BALL_H
#define BALL_H
#include "header.h"

class Ball
{
    public:
        Ball();
        Ball(float x, float y, float ox, float oy, float accx, float accy, float radius, float mass);
        void handleWallCollision();
        virtual ~Ball();

        void updatePos(float step); // Using Euleur method

        // Getters
        float getM(){return mass;};
        float getX(){return pos.x;};
        float getOldX(){return oldPos.x;};
        float getY(){return pos.y;};
        float getOldY(){return oldPos.y;};
        float getAccX(){return acc.x;};
        float getAccY(){return acc.y;};
        float getRadius(){return radius;};

        // Setters
        void setRadius(float newr);
        void setXY(float x, float y);
        void setOldXY(float x, float y);
        void setAccXY(float newaccx, float newaccy);

    protected:

    private:
        // Ball positions
        sf::Vector2f pos, oldPos, acc;
        float radius;
        float mass;
};

#endif // BALL_H
