#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"
#include "../Model/EditorApp.h"

#include <memory>
#include <thread>

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

    void moveCursorStartLine(bool isContent);
    void moveCursorEndLine(bool isContent);
    void moveCursorBegWord(bool isContent);

    bool handleScrollDown();
    bool handleScrollUp();

    bool incCurrentLine();
    bool decCurrentLine();


    void endView();

    NcursesWrapper ncurses;
    Buffer& editorBuffer;
private:
    int _firstLine, _lastLine, _currentTextLine, _currentSubtextLine;
    WindowCords _commandWindowCords, _contentWindowCords;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;
};

#endif