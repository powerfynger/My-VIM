#include "NcursesWrapper.h"


void handleCtrlC(int sig) {
    endwin();
    exit(EXIT_SUCCESS);
}

NcursesWrapper::NcursesWrapper()
{
    initscr();
    cbreak();
    raw();
    nonl();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    // Colors
    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    // Show
    refresh();
}

NcursesWrapper::~NcursesWrapper()
{
    for (unsigned int i = 0; i < this->_windows.size(); i++)
        delwin(this->_windows[i]);
    endwin();
}

void NcursesWrapper::getSize(unsigned int *x, unsigned int *y)
{
    *x = getmaxx(stdscr);
    *y = getmaxy(stdscr);
}

unsigned int NcursesWrapper::addWindow(unsigned int sy, unsigned int sx,
                                       unsigned int h, unsigned int w,
                                       unsigned int colPairId)
{
    WINDOW *new_window = newwin(h, w, sy, sx);
    if (colPairId > 0)
        wbkgd(new_window, COLOR_PAIR(colPairId));
    signal(SIGINT, handleCtrlC);
    this->_windows.push_back(new_window);
    wrefresh(new_window);
    return this->_windows.size() - 1;
}

void NcursesWrapper::delWindow(unsigned int windowId)
{
    WINDOW *cur_window = this->_windows[windowId];
    this->_windows.erase(this->_windows.begin() + windowId);
    delwin(cur_window);
    refresh();
}

void NcursesWrapper::clearWindow(unsigned int windowId)
{
    WINDOW *cur_window = this->_windows[windowId];
    wclear(cur_window);
}

void NcursesWrapper::refreshWindow(unsigned int windowId)
{
    WINDOW *cur_window = this->_windows[windowId];
    wrefresh(cur_window);
}

void NcursesWrapper::writeWindow(unsigned int windowId, MyString str)
{
    WINDOW *cur_window = this->_windows[windowId];
    mvwprintw(cur_window, 0, 0, str.c_str());
}

void NcursesWrapper::writeAppendWindow(unsigned int windowId, MyString str){
    WindowCords cords;
    unsigned int maxY, maxYAnother;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, cords.y, cords.x);
    maxY = getmaxy(cur_window);
    maxYAnother = getmaxy(_windows[1-windowId]);
    /* 
    TODO 
    cords.y + maxYAnother + 1 or cords.y + 2 ???
    */
    if (cords.y + maxYAnother + 1 > maxY)
    {
        return;
    }
    wprintw(cur_window, "%s %d", str.c_str(),maxYAnother);
    wmove(cur_window, cords.y + 1, 0);
}

void NcursesWrapper::setCursor(int windowId, int* y, int* x)
{
    WindowCords maxCords, currentCords;
    WINDOW *cur_window = _windows[windowId];
    getmaxyx(cur_window, maxCords.y, maxCords.x);

    if (*x < 0)
    {
        *x = 0;
        return;
    }
    if (*y < 0)
    {
        *y = 0;
        return;
    }

    if (*x >= maxCords.x){
        if (*y >= maxCords.y){
            // scroll
            return;
        }
        *y += 1;
        *x = 0;
        move(*y, *x);
        return;
    }
    wattron(cur_window, A_REVERSE);

    move(*y, *x);
    wattroff(cur_window, A_REVERSE);

}

WindowCords NcursesWrapper::getCursorCords(unsigned int windowId){
    WindowCords c;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, c.y, c.x);
    return c;

}

int NcursesWrapper::getInput()
{
    return getch();
}

void NcursesWrapper::endNcurses()
{
    endwin();
}