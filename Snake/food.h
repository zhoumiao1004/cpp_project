#ifndef FOOD_H
#define FOOD_H

#include "wall.h"

class Food
{
public:
    Food(Wall &w);
    void setFood();
private:
    int x;
    int y;
    Wall &wall;
};

#endif // FOOD_H
