#ifndef QUADTREE_H
#define QUADTREE_H
#include "header.h"
#include "Ball.h"
#include <vector>

class Quadtree
{
    public:
        Quadtree();
        Quadtree(int level, float tlCornerX, float tlCornerY, float L, float H);
        void clr();
        void split();
        int getIndex(Ball* ball);
        void insertion(Ball* ball);
        std::vector<Ball*> retrieve(std::vector<Ball*> returnBalls, Ball* ball);
        virtual ~Quadtree();

    protected:

    private:
        int MAX_OBJECTS = 10;
        int MAX_LEVELS = 5;
        int level;
        std::vector<Ball*> balls;
        std::vector<Quadtree*> nodes;
        float tlCornerX, tlCornerY, L, H;
};

#endif // QUADTREE_H
