#ifndef WALL_H
#define WALL_H

#include <iostream>
using namespace std;

class Wall
{
public:
    enum {
        ROW = 26,
        COL = 26
    };
    Wall();
    void initWall();
    void drawWall();
    void setWall(int x, int y, char c);
    char getWall(int x, int y);
private:
    char gameArray[ROW][COL];
};

#endif // WALL_H
