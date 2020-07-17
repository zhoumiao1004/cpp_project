#include "food.h"
#include <windows.h>

static void HideCursor()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态
}

//光标定位
static void gotoxy(HANDLE hOut, int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
    HideCursor();
}
static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  //定义显示器句柄变量

Food::Food(Wall &w): wall(w)
{

}

void Food::setFood()
{
    while (true) {
        x = rand() % (Wall::COL-2) + 1;  // 1-25
        y = rand() % (Wall::ROW-2) + 1;  // 1-25
        //如果随机的位置是墙壁或蛇身，再重新随机一次
        if (wall.getWall(x, y) == ' ')
        {
           wall.setWall(x, y, '#');
           gotoxy(hOut, 2 * y, x);
           cout << "#";
           break;
        }
    }
}
