#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include "NcursesWrapper.h"
#include "../Model/EditorApp.h"

#include <memory>
#include <thread>

class EditorView
{
public:
    EditorView(EditorApp& app);

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
    void moveCursorEndWord(bool isContent);
    void moveCursorLineNumber(int lineNumber);

    void moveCursorFirstPage();
    void moveCursorLastPage();
    void moveCursorPageUp();
    void moveCursorPageDown();




    void endView();

    NcursesWrapper ncurses;
private:
    EditorApp& _editorApp;
    int _firstLine, _lastLine, _currentTextLine, _currentSubtextLine;
    WindowCords _commandWindowCords, _contentWindowCords;
    unsigned int _screenSizeX, _screenSizeY;
    unsigned int _commandWindowId, _contentWindowId;

    bool _fixCordXOutsideString();
    bool _incCurrentLine();
    bool _decCurrentLine();
    bool _handleScrollDown();
    bool _handleScrollUp();
};

#endif