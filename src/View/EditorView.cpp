#include "EditorView.h"

EditorView::EditorView(Buffer& buffer) : editorBuffer(buffer)
{
    ncurses.getSize(&_screenSizeX, &_screenSizeY);
    _contentWindowId = ncurses.addWindow(0, 0, _screenSizeY - 1, _screenSizeX, 0);
    _commandWindowId = ncurses.addWindow(_screenSizeY - 1, 0, 1, _screenSizeX, 1);
    _contentWindowCords.x = 0;
    _contentWindowCords.y = 0;
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

void EditorView::displayAllText()
{
    for(const auto line : *editorBuffer.returnText()){
        ncurses.writeAppendWindow(getContentWindowId(), line);
    }
    ncurses.refreshWindow(getContentWindowId());
}

void EditorView::moveCursorRight(bool isContent)
{
    if(isContent == true){
        _contentWindowCords.x += 1;
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    } else
    {
        _commandWindowCords.x += 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
void EditorView::moveCursorLeft(bool isContent)
{
    if(isContent == true){
        _contentWindowCords.x -= 1;
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    } else
    {
        _commandWindowCords.x -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
void EditorView::moveCursorUp(bool isContent)
{
    if(isContent == true){
        _contentWindowCords.y -= 1;
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    } else
    {
        _commandWindowCords.y -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
void EditorView::moveCursorDown(bool isContent)
{
    if(isContent == true){
        _contentWindowCords.y += 1;
        ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
    } else
    {
        _commandWindowCords.y += 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
