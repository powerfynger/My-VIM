#ifndef NCURSESWRAPPER_H
#define NCURSESWRAPPER_H
#include <curses.h>
#include <vector>
#include <signal.h>

#include "../Utils/MyString.h"

struct WindowCords{
    int x, y;
};

enum class ScrollingCode
{
    SCROLL_UP,
    SCROLL_DOWN,
    STAY
};


class NcursesWrapper
{
public:
    NcursesWrapper();
    ~NcursesWrapper();
    void getSize(unsigned int *x, unsigned int *y);
    unsigned int addWindow(unsigned int sy, unsigned int sx,
                           unsigned int h, unsigned int w,
                           unsigned int colPairId);
    void delWindow(unsigned int windowId);
    void clearWindow(unsigned int windowId);
    void refreshWindow(unsigned int windowId);
    void writeWindow(unsigned int windowId, MyString str);
    void writeAppendWindow(unsigned int windowId, MyString str);
    void writeToCurrentLine(unsigned int windowId, MyString str);
    void scrollWindowUp(unsigned int windowId);
    void writeAppendCharWindow(unsigned int windowId, int y, int x,char ch);
    void clearWindowDown(unsigned int windowId);
    ScrollingCode setCursor(int windowId, int* y, int* x);
    WindowCords getCursorCords(unsigned int windowId);
    int getInput();
    void endNcurses();

private:
    // unsigned int 
    std::vector<WINDOW *> _windows;
};

#endif