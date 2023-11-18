#ifndef NCURSESWRAPPER_H
#define NCURSESWRAPPER_H
#include <curses.h>
#include <vector>
#include "../Utils/MyString.h"

struct WindowCords{
    unsigned int x, y;
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
    void setCursor(unsigned int y, unsigned int x);
    char getInput();

private:
    // unsigned int 
    std::vector<WINDOW *> _windows;
};

#endif