#include <ncurses.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <condition_variable>

using namespace std;

struct point
{
    int x, y;
};

int xMax, yMax, height, width;
mutex mx;
condition_variable cv;
char snakeChar = 'o';
WINDOW *topLeftWin;
WINDOW *topRightWin;
WINDOW *botLeftWin;
WINDOW *botRightWin;

vector<point> topLeftSnake;
vector<point> topRightSnake;
vector<point> botLeftSnake;
vector<point> botRightSnake;

void moveTopLeftSnake()
{
    do
    {
        unique_lock<mutex> lock(mx);
        wclear(topLeftWin);
        box(topLeftWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
            if (topLeftSnake[i].x < width - 2)
                topLeftSnake[i].x++;
            else
                topLeftSnake[i].x = 1;
            wmove(topLeftWin, topLeftSnake[i].y, topLeftSnake[i].x);
            waddch(topLeftWin, snakeChar);
            wrefresh(topLeftWin);
        }
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
        int x = rand() % 10 + 1;
        wclear(topRightWin);
        box(topRightWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
            if (topRightSnake[i].x < width - 2)
                topRightSnake[i].x++;
            else
                topRightSnake[i].x = 1;
            wmove(topRightWin, topRightSnake[i].y, topRightSnake[i].x);
            waddch(topRightWin, snakeChar);
            wrefresh(topRightWin);
        }
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
        int x = rand() % 10 + 1;
        wclear(botLeftWin);
        box(botLeftWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
            if (botLeftSnake[i].x < width - 2)
                botLeftSnake[i].x++;
            else
                botLeftSnake[i].x = 1;
            wmove(botLeftWin, botLeftSnake[i].y, botLeftSnake[i].x);
            waddch(botLeftWin, snakeChar);
            wrefresh(botLeftWin);
        }
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
        int x = rand() % 10 + 1;
        wclear(botRightWin);
        box(botRightWin, 0, 0);
        for (int i = 0; i < 5; i++)
        {
            if (botRightSnake[i].x < width - 2)
                botRightSnake[i].x++;
            else
                botRightSnake[i].x = 1;
            wmove(botRightWin, botRightSnake[i].y, botRightSnake[i].x);
            waddch(botRightWin, snakeChar);
            wrefresh(botRightWin);
        }
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
        snakePoint.x = i + 1;
        snakePoint.y = 1;

        topLeftSnake.push_back(snakePoint);
        topRightSnake.push_back(snakePoint);
        botLeftSnake.push_back(snakePoint);
        botRightSnake.push_back(snakePoint);

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