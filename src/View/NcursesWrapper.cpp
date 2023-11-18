#include "NcursesWrapper.h"

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

void NcursesWrapper::setCursor(unsigned int y, unsigned int x)
{
    move(y, x);
}

char NcursesWrapper::getInput()
{
    return getch();
}