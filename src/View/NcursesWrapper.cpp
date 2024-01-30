#include "NcursesWrapper.h"


void handleCtrlC(int sig) {
    endwin();
    exit(EXIT_SUCCESS);
}

NcursesWrapper::NcursesWrapper()
{
    initscr();
    cbreak();
    curs_set(2);
    raw();
    nonl();
    noecho();
    keypad(stdscr, TRUE);
    // Colors
    start_color();
    use_default_colors();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_WHITE, COLOR_BLACK);
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

    scrollok(new_window, TRUE); // Включение прокрутки для окна

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

void NcursesWrapper::scrollWindowUp(unsigned int windowId)
{
    WINDOW *cur_window = _windows[windowId];
    wmove(cur_window, 0, 0);
    wscrl(cur_window, -1);
}


void NcursesWrapper::writeAppendWindow(unsigned int windowId, MyString str){
    WindowCords cords;
    unsigned int maxY, maxYAnother;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, cords.y, cords.x);
    maxY = getmaxy(cur_window);
    maxYAnother = getmaxy(_windows[1-windowId]);
    if (cords.y >= maxY - 1)
    {
        scroll(cur_window);
        wmove(cur_window, cords.y, 0);
        wclrtoeol(cur_window);
    }
    else
    {
        wmove(cur_window, cords.y + 1, 0);
    }
    wprintw(cur_window, "%s", str.c_str());
    // wmove(cur_window, cords.y + 1, 0);
}

void NcursesWrapper::writeToCurrentLine(unsigned int windowId, MyString str)
{
    WindowCords cords, oldCords;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, cords.y, cords.x);
    oldCords = cords;
    cords.x = 0;
    setCursor(windowId, &cords.y, &cords.x);
    int len_to_erase = getmaxx(cur_window) - cords.x;
    for (int i = 0; i < len_to_erase; i++) {
        mvwdelch(cur_window, cords.y, cords.x);
    }
    wprintw(cur_window, "%s", str.c_str());
    cords = oldCords;
    setCursor(windowId, &cords.y, &cords.x);
}

void NcursesWrapper::clearWindowDown(unsigned int windowId)
{
    WindowCords cords, oldCords;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, cords.y, cords.x);
    WindowCords maxCords;
    getmaxyx(cur_window, maxCords.y, maxCords.x);

    oldCords = cords;
    cords.x = 0;
    int len_to_erase = getmaxx(cur_window) - cords.x;
    while (cords.y < maxCords.y){
        setCursor(windowId, &cords.y, &cords.x);
        for (int i = 0; i < len_to_erase; i++) {
            mvwdelch(cur_window, cords.y, cords.x);
        }
        cords.y++;
    }
    cords = oldCords;
    setCursor(windowId, &cords.y, &cords.x);
}



ScrollingCode NcursesWrapper::setCursor(int windowId, int* y, int* x)
{
    WindowCords maxCords, currentCords;
    WINDOW *cur_window = _windows[windowId];
    getmaxyx(cur_window, maxCords.y, maxCords.x);

    if (*x < 0)
    {
        *x = maxCords.x - 1;
        *y -= 1;
    }
    if (*y < 0)
    {
        // scroll up
        *y = 0;
        *x = 0;
        wmove(cur_window, *y, *x);
        refreshWindow(windowId);
        return ScrollingCode::SCROLL_UP;
    }

    if (*x >= maxCords.x){
        *y += 1;
        *x = 0;
    }
    if (*y >= maxCords.y){
        // SCROLL DOWN
        *y = maxCords.y - 1;
        wmove(cur_window, *y, *x);
        refreshWindow(windowId);
        return ScrollingCode::SCROLL_DOWN;
    }
    wmove(cur_window, *y, *x);
    refreshWindow(windowId);
    return ScrollingCode::STAY;
}

WindowCords NcursesWrapper::getCursorCords(unsigned int windowId){
    WindowCords c;
    WINDOW *cur_window = _windows[windowId];
    getyx(cur_window, c.y, c.x);
    return c;

}

void NcursesWrapper::writeAppendCharWindow(unsigned int windowId, int y, int x, char ch)
{
    WINDOW *cur_window = _windows[windowId];

    // Сохранение оригинального символа
    int originalChar = mvwinch(cur_window, y, x);

    // Установка нового символа
    mvwaddch(cur_window, y, x, ch);

    // Обновление экрана
    wrefresh(cur_window);

    // Восстановление оригинального символа на его исходные координаты
    mvwaddch(cur_window, y, x, originalChar);
}

int NcursesWrapper::getInput()
{
    timeout(10000);
    return getch();
}

void NcursesWrapper::endNcurses()
{
    endwin();
}