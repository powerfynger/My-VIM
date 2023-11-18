#include "EditorView.h"

EditorView::EditorView()
{
    _ncurses.getSize(&_screenSizeX, &_screenSizeY);
    _contentWindowId = _ncurses.addWindow(0, 0, _screenSizeY - 1, _screenSizeX, 0);
    _commandWindowId = _ncurses.addWindow(_screenSizeY - 1, 0, 1, _screenSizeX, 1);
}

void EditorView::getInput()
{
    _ncurses.getInput();
}
