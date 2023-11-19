#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"
#include "../Model/EditorApp.h"

#include <memory>

class EditorView
{
public:
    EditorView(Buffer& buf);

    unsigned int getScrSizeX();
    unsigned int getScrSizeY();
    unsigned int getCmdWindowId();
    unsigned int getContentWindowId();

    void displayAllText();

    void moveCursorRight(bool isContent);
    void moveCursorLeft(bool isContent);
    void moveCursorUp(bool isContent);
    void moveCursorDown(bool isContent);


    void endView();

    NcursesWrapper ncurses;
    Buffer& editorBuffer;
private:
    unsigned int _firstLine, _lastLine, _currentLine;
    WindowCords _commandWindowCords, _contentWindowCords;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;
};

#endif