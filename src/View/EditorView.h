#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"

class EditorView
{
public:
    NcursesWrapper ncurses;
    EditorView();
    unsigned int getScrSizeX();
    unsigned int getScrSizeY();
    unsigned int getCmdWindowId();
    unsigned int getContentWindowId();

private:
    WindowCords _commandWindowCords, _contentWindowCords;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;
};

#endif