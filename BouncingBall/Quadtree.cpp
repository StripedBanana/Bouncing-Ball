#include "Quadtree.h"
#include <stdbool.h>
#include "header.h"
#include <iostream>

using namespace std;

Quadtree::Quadtree()
{
    this->level = 0;
    this->tlCornerX = 0;
    this->tlCornerY = 0;
    this->L = LENGTH;
    this->H = HEIGHT;
    std::vector<Quadtree*> q(4);
    this->nodes = q;


}

Quadtree::Quadtree(int level, float tlCornerX, float tlCornerY, float L, float H)
{
    this->level = level;
    this->tlCornerX = tlCornerX;
    this->tlCornerY = tlCornerY;
    this->L = L;
    this->H = H;
    std::vector<Quadtree*> q(4);
    this->nodes = q;
}

/*
 * Clears the quadtree
 */
void Quadtree::clr() {
   balls.clear();

   for (int i = 0; i < nodes.size(); i++) {
     if (nodes[i] != 0) {
       nodes[i]->clr();
       nodes[i] = 0;
     }
   }
 }

/*
 * Splits the node into 4 subnodes
 */
void Quadtree::split()
 {
   int subWidth = (int)(this->L / 2);
   int subHeight = (int)(this->H / 2);
   int x = (int)this->tlCornerX;
   int y = (int)this->tlCornerY;

   nodes[0] = new Quadtree(level+1, x + subWidth, y, subWidth, subHeight);
   nodes[1] = new Quadtree(level+1, x, y, subWidth, subHeight);
   nodes[2] = new Quadtree(level+1, x, y + subHeight, subWidth, subHeight);
   nodes[3] = new Quadtree(level+1, x + subWidth, y + subHeight, subWidth, subHeight);
 }

 /*
 * Determine which node the object belongs to. -1 means
 * object cannot completely fit within a child node and is part
 * of the parent node
 */
int Quadtree::getIndex(Ball* ball) {
    int index = -1;
    float x = ball->getX();
    float y = ball->getY();
    float radius = ball->getRadius();
    double verticalMidpoint = this->tlCornerX + (this->L)/2;
    double horizontalMidpoint = this->tlCornerY + (this->H)/2;

    // Object can completely fit within the top quadrants
    bool topQuadrant = (y < horizontalMidpoint && y + radius*2 < horizontalMidpoint);
    // Object can completely fit within the bottom quadrants
    bool bottomQuadrant = (y > horizontalMidpoint);

    // Object can completely fit within the left quadrants
    if (x < verticalMidpoint && x + radius*2 < verticalMidpoint) {
        if (topQuadrant) {
            index = 1;
        }
        else if (bottomQuadrant) {
            index = 2;
        }
    }
    // Object can completely fit within the right quadrants
    else if (x > verticalMidpoint) {
        if (topQuadrant) {
            index = 0;
        }
        else if (bottomQuadrant) {
            index = 3;
        }
    }

    //cout << "index=" << index << endl;
    return index;
 }

 /*
 * Insert the object into the quadtree. If the node
 * exceeds the capacity, it will split and add all
 * objects to their corresponding nodes.
 */
 void Quadtree::insertion(Ball* ball) {
   if (nodes.at(0)) {
     int index = getIndex(ball);

     if (index != -1) {
       nodes[index]->insertion(ball);

       return;
     }
   }


   balls.push_back(ball);

   if (balls.size() > MAX_OBJECTS && level < MAX_LEVELS) {
      if (nodes[0] == 0) {
         split();
      }

     int i = 0;
     while (i < balls.size()) {
       int index = getIndex(balls.at(i));
       if (index != -1) {
         nodes[index]->insertion(balls.at(i));
         balls.erase(balls.begin()+i);
       }
       else {
         i++;
       }
     }
   }
 }

 /*
 * Return all objects that could collide with the given object
 */
void Quadtree::retrieve(std::vector<Ball*>* returnBalls, Ball* ball) {
    int index = getIndex(ball);
    if (index != -1 && nodes[0] != 0) {
        nodes[index]->retrieve(returnBalls, ball);
    }

    returnBalls->insert(returnBalls->end(), balls.begin(), balls.end());

}

void Quadtree::display(sf::RenderWindow* window)
{
    sf::RectangleShape rectangle;
    rectangle.setPosition(this->tlCornerX, this->tlCornerY);
    rectangle.setSize(sf::Vector2f(this->L, this->H));
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1);
    window->draw(rectangle);
    /*
    cout << "level=" << this->level << endl;
    cout << "x=" << this->tlCornerX << " y=" << this->tlCornerY << endl;
    cout << "L=" << this->L << " H=" << this->H << endl;
    */
    for(int i=0; i<4; i++)
    {
        if(nodes[i])
        {
            nodes[i]->display(window);
        }
    }
}

Quadtree::~Quadtree()
{
    //dtor
}
