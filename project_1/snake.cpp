#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

using namespace std;

int xMax, yMax, height, width;
mutex mx;
condition_variable cv;
char snakeChar = 'o';
WINDOW *topLeftWin;
WINDOW *topRightWin;
WINDOW *botLeftWin;
WINDOW *botRightWin;

struct point
{
    int x, y;
};

void moveTopLeftSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        cv.notify_all();
        int x = rand() % 10 + 1;
        wclear(topLeftWin);
        box(topLeftWin, 0, 0);
        wmove(topLeftWin, x, x);
        waddch(topLeftWin, snakeChar);
        wrefresh(topLeftWin);
        cv.wait(lock);
    } while (true);
}

void moveTopRightSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        cv.notify_all();
        int x = rand() % 10 + 1;
        wclear(topRightWin);
        box(topRightWin, 0, 0);
        wmove(topRightWin, x, x);
        waddch(topRightWin, snakeChar);
        wrefresh(topRightWin);
        cv.wait(lock);
    } while (true);
}

void moveBotLeftSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        cv.notify_all();
        int x = rand() % 10 + 1;
        wclear(botLeftWin);
        box(botLeftWin, 0, 0);
        wmove(botLeftWin, x, x);
        waddch(botLeftWin, snakeChar);
        wrefresh(botLeftWin);
        cv.wait(lock);
    } while (true);
}

void moveBotRightSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        cv.notify_all();
        int x = rand() % 10 + 1;
        wclear(botRightWin);
        box(botRightWin, 0, 0);
        wmove(botRightWin, x, x);
        waddch(botRightWin, snakeChar);
        wrefresh(botRightWin);
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

    // point topLeftSnake;
    // point topRightSnake;
    // point botLeftSnake;
    // point botRightSnake;

    // topLeftSnake.x = 5;
    // topLeftSnake.y = 15;
    // wmove(topLeftWin, 5, 15);
    // waddch(topLeftWin, snakeChar);
    // wrefresh(topLeftWin);

    // topRightSnake.x = 5;
    // topRightSnake.y = 15;
    // wmove(topRightWin, 5, 15);
    // waddch(topRightWin, snakeChar);
    // wrefresh(topRightWin);

    // botLeftSnake.x = 5;
    // botLeftSnake.y = 15;
    // wmove(botLeftWin, 5, 15);
    // waddch(botLeftWin, snakeChar);
    // wrefresh(botLeftWin);

    // botRightSnake.x = 5;
    // botRightSnake.y = 15;
    // wmove(botRightWin, 5, 15);
    // waddch(botRightWin, snakeChar);
    // wrefresh(botRightWin);

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