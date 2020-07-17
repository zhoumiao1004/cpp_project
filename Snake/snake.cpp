#include "snake.h"
#include <windows.h>

//光标定位
static void gotoxy(HANDLE hOut, int x, int y)
{
    COORD pos;
    pos.X = x;
    pos.Y = y;
    SetConsoleCursorPosition(hOut, pos);
}
static HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);  //定义显示器句柄变量

Snake::Snake(Wall &w, Food &f): wall(w), food(f)
{
    pHead = NULL;
    isRool = false;
}

void Snake::destroyPoint()
{
    Point *pCur = NULL;
    while (pHead != NULL) {
        pCur = pHead->next;
        delete pHead;
        pHead = pCur;
    }
}

//添加新节点
void Snake::addPoint(int x, int y)
{
    //创建新节点
    Point *np = new Point;
    np->x = x;
    np->y = y;
    np->next = NULL;

    //如果原来头不为空 改为身子
    if (pHead != NULL) {
        wall.setWall(pHead->x, pHead->y, '=');
        gotoxy(hOut, 2 * pHead->y, pHead->x);
        cout << "=";
    }
    np->next = pHead;
    pHead = np; //更新头部
    wall.setWall(pHead->x, pHead->y, '@');
    gotoxy(hOut, 2 * pHead->y, pHead->x);
    cout << '@';
    length++;
}

//删除尾节点
void Snake::delPoint()
{
    if (!pHead || !pHead->next)
        return;
    Point *pPre = pHead;
    Point *pCur = pHead->next;
    while(pCur->next) {
        pCur = pCur->next;
        pPre = pPre->next;
    }
    wall.setWall(pCur->x, pCur->y, ' ');
    gotoxy(hOut, 2 * pCur->y, pCur->x);
    cout << " ";  // 覆盖原来的尾节点

    pPre->next = NULL;
    delete pCur;
    pCur = NULL;
    length--;
}

//移动
bool Snake::move(char key)
{
    int x = pHead->x;
    int y = pHead->y;

    switch(key)
    {
    case UP: x--; break;
    case DOWN: x++; break;
    case LEFT: y--; break;
    case RIGHT: y++; break;
    default: break;
    }

    char ch = wall.getWall(x, y);  // 获取新头的值
    Point *pTail = getTail();
    if (pTail->x == x && pTail->y == y)
    {
        isRool = true;
    }
    else
    {
        //判断用户到达位置是否成功
        if (ch == '*' || ch == '=')
        {
            addPoint(x, y);
            delPoint();
//            system("cls");
//            wall.drawWall(getScore());
            showScore();
            cout << "GAMEOVER!!!" << endl;
            return false;
        }
    }

    // 成功分两种情况
    if (ch == '#') {
        //吃到食物
        addPoint(x, y);
        food.setFood();
    } else {
        //未吃到食物
        addPoint(x, y);
        delPoint();
        if (isRool == true)
        {
            wall.setWall(x, y, '@');
            gotoxy(hOut, 2 * y, x);
            cout << "@";
        }
    }
    return true;
}

Snake::Point *Snake::getTail()
{
    Point *pPre = pHead;
    Point *pCur = pHead->next;
    while(pCur->next) {
        pCur = pCur->next;
        pPre = pPre->next;
    }
    return pCur;
}

int Snake::getSleepTime()
{
    int sleepTime = 0;
    int size = countList();
//    if (size < 5)
//    {
//        sleepTime = 300;
//    }
//    else if (size >=5 && size <=8)
//    {
//        sleepTime = 200;
//    }
//    else
//    {
//        sleepTime = 100;
//    }
    sleepTime = 1000 / size;
    return sleepTime;
}

int Snake::countList()
{
//    int size = 0;
//    Point *pCur = pHead;
//    while (pCur)
//    {
//        size++;
//        pCur = pCur->next;
//    }
//    return size;
    return length;
}

int Snake::getScore()
{
    int size = countList();
    return (size-3) * 100;
}

void Snake::showScore()
{
    gotoxy(hOut, 0, Wall::COL);
    cout << "Score:" << getScore() << endl;
}

void Snake::initSnake()
{
    destroyPoint();
    addPoint(5, 3);
    addPoint(5, 4);
    addPoint(5, 5);
}

