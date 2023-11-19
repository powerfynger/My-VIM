#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"
#include "../Model/EditorApp.h"

#include <memory>

class EditorView
{
public:
    NcursesWrapper ncurses;
    EditorView(Buffer& buf);
    unsigned int getScrSizeX();
    unsigned int getScrSizeY();
    unsigned int getCmdWindowId();
    unsigned int getContentWindowId();
    Buffer& editorBuffer;

private:
    WindowCords _commandWindowCords, _contentWindowCords;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;
};

#endif