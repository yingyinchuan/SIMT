#include <iostream>
#include <ncurses.h>
#include <cstdlib>
#include <ctime>

const int BOARD_SIZE = 4;
int board[BOARD_SIZE][BOARD_SIZE];

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

void initializeBoard()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            board[i][j] = 0;
        }
    }
}

void generateRandomTile()
{
    int emptyCells = 0;
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == 0)
            {
                emptyCells++;
            }
        }
    }

    if (emptyCells == 0)
    {
        return; // No empty cells to generate a tile
    }

    int randomIndex = rand() % emptyCells + 1;
    int count = 0;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == 0)
            {
                count++;
                if (count == randomIndex)
                {
                    board[i][j] = (rand() % 2 + 1) * 2; // Generates 2 or 4 randomly
                    return;
                }
            }
        }
    }
}

void printBoard()
{
    clear();

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            printw("%4d", board[i][j]);
        }
        printw("\n");
    }

    refresh();
}

bool moveTiles(Direction direction)
{
    bool moved = false;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            int currentTile = board[i][j];

            if (currentTile == 0)
                continue;

            int nextI = i;
            int nextJ = j;

            switch (direction)
            {
            case Direction::UP:
                while (nextI > 0 && board[nextI - 1][j] == 0)
                {
                    nextI--;
                    moved = true;
                }
                break;
            case Direction::DOWN:
                while (nextI < BOARD_SIZE - 1 && board[nextI + 1][j] == 0)
                {
                    nextI++;
                    moved = true;
                }
                break;
            case Direction::LEFT:
                while (nextJ > 0 && board[i][nextJ - 1] == 0)
                {
                    nextJ--;
                    moved = true;
                }
                break;
            case Direction::RIGHT:
                while (nextJ < BOARD_SIZE - 1 && board[i][nextJ + 1] == 0)
                {
                    nextJ++;
                    moved = true;
                }
                break;
            }

            if (nextI != i || nextJ != j)
            {
                board[i][j] = 0;
                board[nextI][nextJ] = currentTile;
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE - 1; ++j)
        {
            if (board[i][j] == board[i][j + 1] && board[i][j] != 0)
            {
                board[i][j] *= 2;
                board[i][j + 1] = 0;
                moved = true;
            }
        }
    }

    return moved;
}

bool isGameOver()
{
    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            if (board[i][j] == 0 ||
                (i > 0 && board[i][j] == board[i - 1][j]) ||
                (i < BOARD_SIZE - 1 && board[i][j] == board[i + 1][j]) ||
                (j > 0 && board[i][j] == board[i][j - 1]) ||
                (j < BOARD_SIZE - 1 && board[i][j] == board[i][j + 1]))
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    initializeBoard();
    generateRandomTile();
    generateRandomTile();
    printBoard();

    while (!isGameOver())
    {
        int ch = getch();
        Direction direction;

        switch (ch)
        {
        case KEY_UP:
            direction = Direction::UP;
            break;
        case KEY_DOWN:
            direction = Direction::DOWN;
            break;
        case KEY_LEFT:
            direction = Direction::LEFT;
            break;
        case KEY_RIGHT:
            direction = Direction::RIGHT;
            break;
        default:
            continue; // Ignore other keys
        }

        if (moveTiles(direction))
        {
            generateRandomTile();
            printBoard();
        }
    }

    endwin(); // End ncurses mode
    return 0;
}
