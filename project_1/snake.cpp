#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

using namespace std;

struct point
{
    int x, y, poprzedni;
};

int xMax, yMax, height, width;
mutex mx;
condition_variable cv;
char snakeChar = 'o';
WINDOW *topLeftWin;
WINDOW *topRightWin;
WINDOW *botLeftWin;
WINDOW *botRightWin;

point *topLeftSnake = new point[5];
point *topRightSnake = new point[5];
point *botLeftSnake = new point[5];
point *botRightSnake = new point[5];

void nextMove(point *snake)
{
    int direction = (rand() % 4) + 1;
    while(snake[0].poprzedni == direction + 2 || snake[0].poprzedni == direction - 2)
    {
        direction = (rand() % 4) + 1;
    }
    switch (direction)
    {
    case 1: //up
        
        for (int i = 0; i < 4; i++)
        {
            snake[i] = snake[i + 1];
        }
        snake[4].y--;
        snake[0].poprzedni = 1;
        break;

    case 2: //right
        for (int i = 0; i < 4; i++)
        {
            snake[i] = snake[i + 1];
        }
        snake[4].x++;
        snake[0].poprzedni = 2;
        break;

    case 3: //down
        for (int i = 0; i < 4; i++)
        {
            snake[i] = snake[i + 1];
        }
        snake[4].y++;
        snake[0].poprzedni = 3;
        break;

    default: //left
        for (int i = 0; i < 4; i++)
        {
            snake[i] = snake[i + 1];
        }
        snake[4].x--;
        snake[0].poprzedni = 4;
        break;
    }
}

void moveTopLeftSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        wclear(topLeftWin);
        box(topLeftWin, 0, 0);

        for (int i = 0; i < 5; i++) //draw
        {
            if (topLeftSnake[i].x >= width - 1)
                topLeftSnake[i].x = 1;
            if (topLeftSnake[i].y >= height - 1)
                topLeftSnake[i].y = 1;
            if (topLeftSnake[i].x < 2)
                topLeftSnake[i].x = width - 2;
            if (topLeftSnake[i].y < 2)
                topLeftSnake[i].y = height - 2;

            wmove(topLeftWin, topLeftSnake[i].y, topLeftSnake[i].x);
            waddch(topLeftWin, snakeChar);
            wrefresh(topLeftWin);
        }

        nextMove(topLeftSnake);
        this_thread::sleep_for(chrono::milliseconds(100));
        cv.notify_all();
        cv.wait(lock);
    } while (true);
}

void moveTopRightSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        wclear(topRightWin);
        box(topRightWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
           if (topRightSnake[i].x >= width - 1)
                topRightSnake[i].x = 1;
            if (topRightSnake[i].y >= height - 1)
                topRightSnake[i].y = 1;
            if (topRightSnake[i].x < 2)
                topRightSnake[i].x = width - 2;
            if (topRightSnake[i].y < 2)
                topRightSnake[i].y = height - 2;

            wmove(topRightWin, topRightSnake[i].y, topRightSnake[i].x);
            waddch(topRightWin, snakeChar);
            wrefresh(topRightWin);
        }

        nextMove(topRightSnake);
        this_thread::sleep_for(chrono::milliseconds(100));
        cv.notify_all();
        cv.wait(lock);
    } while (true);
}

void moveBotLeftSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        wclear(botLeftWin);
        box(botLeftWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
            if (botLeftSnake[i].x >= width - 1)
                botLeftSnake[i].x = 1;
            if (botLeftSnake[i].y >= height - 1)
                botLeftSnake[i].y = 1;
            if (botLeftSnake[i].x < 2)
                botLeftSnake[i].x = width - 2;
            if (botLeftSnake[i].y < 2)
                botLeftSnake[i].y = height - 2;

            wmove(botLeftWin, botLeftSnake[i].y, botLeftSnake[i].x);
            waddch(botLeftWin, snakeChar);
            wrefresh(botLeftWin);
        }

        nextMove(botLeftSnake);
        this_thread::sleep_for(chrono::milliseconds(100));
        cv.notify_all();
        cv.wait(lock);
    } while (true);
}

void moveBotRightSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        wclear(botRightWin);
        box(botRightWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
             if (botRightSnake[i].x >= width - 1)
                botRightSnake[i].x = 1;
            if (botRightSnake[i].y >= height - 1)
                botRightSnake[i].y = 1;
            if (botRightSnake[i].x < 2)
                botRightSnake[i].x = width - 2;
            if (botRightSnake[i].y < 2)
                botRightSnake[i].y = height - 2;

            wmove(botRightWin, botRightSnake[i].y, botRightSnake[i].x);
            waddch(botRightWin, snakeChar);
            wrefresh(botRightWin);
        }

        nextMove(botRightSnake);
        this_thread::sleep_for(chrono::milliseconds(100));
        cv.notify_all();
        cv.wait(lock);
    } while (true);
}

void prepareWindows()
{
    initscr();
    noecho();
    cbreak();
    //curs_set(FALSE);

    getmaxyx(stdscr, yMax, xMax);
    height = yMax / 2;
    width = xMax / 2;

    // WINDOW * win = newwin(height, width, start_y, start_x);
    topLeftWin = newwin(height, width, 0, 0);
    topRightWin = newwin(height, width, 0, width);
    botLeftWin = newwin(height, width, height, 0);
    botRightWin = newwin(height, width, height, width);
    refresh();

    //wborder(win, left, right, top, bottom, tlc, trc, blc, brc);
    box(topLeftWin, 0, 0);
    wrefresh(topLeftWin);

    box(topRightWin, 0, 0);
    wrefresh(topRightWin);

    box(botLeftWin, 0, 0);
    wrefresh(botLeftWin);

    box(botRightWin, 0, 0);
    wrefresh(botRightWin);

    move(10, 10);
}

int main()
{
    srand(time(NULL));
    prepareWindows();

    for (int i = 0; i < 5; i++)
    {
        point snakePoint;
        snakePoint.x = i + 2;
        snakePoint.y = 2;

        topLeftSnake[i].x = snakePoint.x;
        topLeftSnake[i].y = snakePoint.y;
        topRightSnake[i].x = snakePoint.x;
        topRightSnake[i].y = snakePoint.y;
        botLeftSnake[i].x = snakePoint.x;
        botLeftSnake[i].y = snakePoint.y;
        botRightSnake[i].x = snakePoint.x;
        botRightSnake[i].y = snakePoint.y;

        wmove(topLeftWin, topLeftSnake[i].y, topLeftSnake[i].x);
        waddch(topLeftWin, snakeChar);
        wrefresh(topLeftWin);

        wmove(topRightWin, topRightSnake[i].y, topRightSnake[i].x);
        waddch(topRightWin, snakeChar);
        wrefresh(topRightWin);

        wmove(botLeftWin, botLeftSnake[i].y, botLeftSnake[i].x);
        waddch(botLeftWin, snakeChar);
        wrefresh(botLeftWin);

        wmove(botRightWin, botRightSnake[i].y, botRightSnake[i].x);
        waddch(botRightWin, snakeChar);
        wrefresh(botRightWin);
    }

    thread moveTopLeftSnakeThread = thread(moveTopLeftSnake);
    thread moveTopRightSnakeThread = thread(moveTopRightSnake);
    thread moveBotLeftSnakeThread = thread(moveBotLeftSnake);
    thread moveBotRightSnakeThread = thread(moveBotRightSnake);

    moveTopLeftSnakeThread.join();
    moveTopRightSnakeThread.join();
    moveBotLeftSnakeThread.join();
    moveBotRightSnakeThread.join();

    getch();
    endwin();
    return 0;
}