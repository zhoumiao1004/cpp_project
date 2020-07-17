#include <iostream>
#include "wall.h"
#include "snake.h"
#include "food.h"
#include <ctime>
#include <conio.h>
#include <unistd.h>
#include <windows.h>
#pragma execution_character_set("utf-8");

using namespace std;

int main()
{
    srand((unsigned int)time(NULL));
    bool isDead = false;
    char preKey = NULL;

    //初始化墙壁
    Wall wall;
    wall.initWall();
    wall.drawWall();  //先画好墙壁，后面就不动了

    //初始化食物
    Food food(wall);
    food.setFood();

    //初始化蛇
    Snake snake(wall, food);
    snake.initSnake();
    snake.showScore();

    //接受用户输入
    while (!isDead)
    {
        char key = _getch();
        // 如果是第一次按了左
        if (preKey == NULL && key == Snake::LEFT)
            continue;
        do{
            if (key == Snake::LEFT || key == Snake::RIGHT || key == Snake::UP || key == Snake::DOWN)
            {
                // 判断本次按键是否与上次按键冲突
                if ((key == Snake::LEFT && preKey == Snake::RIGHT) ||
                        (key == Snake::RIGHT && preKey == Snake::LEFT) ||
                        (key == Snake::UP && preKey == Snake::DOWN) ||
                        (key == Snake::DOWN && preKey == Snake::UP))
                {
                    key = preKey;
                }
                else
                {
                    preKey = key;
                }

                if (snake.move(key) == true)
                {
//                    system("cls");
//                    wall.drawWall(snake.getScore());
                    snake.showScore();
                    Sleep(snake.getSleepTime());
                }
                else
                {
                    isDead = true;
                    break;
                }
            }
            else
            {
                key = preKey;  // 按了方向键以外的键
            }
        }while(!_kbhit()); //当没有键盘输入的时候返回0
    }

    system("pause");
    return 0;
}
