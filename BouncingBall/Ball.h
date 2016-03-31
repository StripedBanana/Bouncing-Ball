#ifndef BALL_H
#define BALL_H


class Ball
{
    public:
        Ball();
        virtual ~Ball();

        void updatePos(float step); // Using Verlet integration
        float getX(){return nextX;};
        float getY(){return nextY;};
        void setXY(float x, float y);

    protected:

    private:
        // Ball positions
        float nextX, nextY;
        float currentX, currentY;
        float previousX, previousY;
};

#endif // BALL_H
