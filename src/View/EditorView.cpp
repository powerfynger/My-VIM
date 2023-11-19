#include "EditorView.h"

EditorView::EditorView(Buffer& buffer) : editorBuffer(buffer)
{
    ncurses.getSize(&_screenSizeX, &_screenSizeY);
    _contentWindowId = ncurses.addWindow(0, 0, _screenSizeY - 1, _screenSizeX, 0);
    _commandWindowId = ncurses.addWindow(_screenSizeY - 1, 0, 1, _screenSizeX, 1);
}

unsigned int EditorView::getScrSizeX()
{
    return _screenSizeX;
}
unsigned int EditorView::getScrSizeY()
{
    return _screenSizeY;
}
unsigned int EditorView::getCmdWindowId()
{
    return _commandWindowId;
}
unsigned int EditorView::getContentWindowId()
{
    return _contentWindowId;
}