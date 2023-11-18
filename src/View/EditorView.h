#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"

class EditorView
{
public:
    EditorView();
    void getInput();

private:
    NcursesWrapper _ncurses;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;
};

#endif