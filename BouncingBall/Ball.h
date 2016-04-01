#ifndef BALL_H
#define BALL_H


class Ball
{
    public:
        Ball();
        void handleCollision();
        virtual ~Ball();

        void updatePos(float step); // Using Euleur method

        // Getters
        float getX(){return currentX;};
        float getY(){return currentY;};
        float getAccX(){return accX;};
        float getAccY(){return accY;};
        float getRadius(){return radius;};

        // Setters
        void setRadius(float newr);
        void setXY(float x, float y);
        void setSpeedXY(float sx, float sy);
        void setAccXY(float newaccx, float newaccy);

    protected:

    private:
        // Ball positions
        float speedX, speedY;
        float previousSpeedX, previousSpeedY;
        float currentX, currentY;
        float previousX, previousY;
        float accX, accY;
        float radius;
};

#endif // BALL_H
