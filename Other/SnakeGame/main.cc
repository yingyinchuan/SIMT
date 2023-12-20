#include <iostream>
#include <ncurses.h>
#include <unistd.h>

bool gameOver;
const int width = 20;
const int height = 10;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};
eDirection dir;

void Setup()
{
    initscr();   // 初始化屏幕
    clear();     // 清空屏幕缓冲区
    noecho();    // 禁止在终端中回显字符
    cbreak();    // 禁用行缓冲
    curs_set(0); // 隐藏光标

    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw()
{
    clear(); // 清空屏幕

    // 绘制墙
    for (int i = 0; i < width + 2; i++)
        mvprintw(0, i, "#");
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (j == 0)
                mvprintw(i + 1, j, "#");
            if (i == y && j == x)
                mvprintw(i + 1, j, "O");
            else if (i == fruitY && j == fruitX)
                mvprintw(i + 1, j, "F");
            else
            {
                bool print = false;
                for (int k = 0; k < nTail; k++)
                {
                    if (tailX[k] == j && tailY[k] == i)
                    {
                        mvprintw(i + 1, j, "o");
                        print = true;
                    }
                }
                if (!print)
                    mvprintw(i + 1, j, " ");
            }

            if (j == width - 1)
                mvprintw(i + 1, j + 1, "#");
        }
    }

    // 绘制得分
    mvprintw(height + 2, 0, "Score: %d", score);

    // 刷新屏幕
    refresh();
}

void Input()
{
    keypad(stdscr, TRUE); // 启用键盘输入
    halfdelay(1);         // 设置非阻塞输入，等待1秒

    int c = getch();
    switch (c)
    {
    case 'a':
        dir = LEFT;
        break;
    case 'd':
        dir = RIGHT;
        break;
    case 'w':
        dir = UP;
        break;
    case 's':
        dir = DOWN;
        break;
    case 'x':
        gameOver = true;
        break;
    }
}

void Algorithm()
{
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++)
    {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    default:
        break;
    }

    if (x >= width)
        x = 0;
    else if (x < 0)
        x = width - 1;

    if (y >= height)
        y = 0;
    else if (y < 0)
        y = height - 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameOver = true;

    if (x == fruitX && y == fruitY)
    {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        nTail++;
    }
}

int main()
{
    Setup();
    while (!gameOver)
    {
        Draw();
        Input();
        Algorithm();
        usleep(100000); // 暂停100毫秒
    }

    endwin(); // 结束ncurses模式
    return 0;
}
