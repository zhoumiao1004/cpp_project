#ifndef SNAKE_H
#define SNAKE_H

#include "wall.h"
#include "food.h"

class Snake
{
public:
    enum {UP = 'w', DOWN = 's', LEFT = 'a', RIGHT = 'd'};
    Snake(Wall &w, Food &food);
    //节点
    struct Point {
        int x;
        int y;
        Point *next;
    };

    // 初始化蛇
    void initSnake();
    // 销毁节点
    void destroyPoint();
    // 添加节点
    void addPoint(int x, int y);
    //删除尾节点
    void delPoint();
    //移动
    bool move(char key);
    //获取蛇尾
    Point *getTail();
    //获取刷屏时间
    int getSleepTime();
    //获取蛇长度
    int countList();
    //获取分数
    int getScore();
    //显示分数
    void showScore();

    Point *pHead;  //蛇头
    Wall &wall;
    Food &food;
    bool isRool;  //头尾循环标志
    int length = 0;  // 蛇的长度
};

#endif // SNAKE_H
