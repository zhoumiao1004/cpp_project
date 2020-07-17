#include "wall.h"
#include <windows.h>

Wall::Wall()
{

}

void Wall::initWall()
{
    for(int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            if (i == 0 || j == 0 || i == ROW-1 || j == COL-1)
                gameArray[i][j] = '*';
            else
                gameArray[i][j] = ' ';
        }
    }
}

void Wall::drawWall()
{
    for(int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++)
            cout << gameArray[i][j] << " ";
        switch (i) {
        case 5: cout << "created by zm"; break;
        case 6: cout << "a : left"; break;
        case 7: cout << "d : right"; break;
        case 8: cout << "w : up"; break;
        case 9: cout << "s : down"; break;
        case 10: cout << "zhou loves luo"; break;
        }
        cout << endl;
    }
}

void Wall::setWall(int x, int y, char c)
{
    gameArray[x][y] = c;
}

char Wall::getWall(int x, int y)
{
    return gameArray[x][y];
}
