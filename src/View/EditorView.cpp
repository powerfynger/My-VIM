#include "EditorView.h"

EditorView::EditorView(Buffer& buffer) : editorBuffer(buffer)
{
    ncurses.getSize(&_screenSizeX, &_screenSizeY);
    _contentWindowId = ncurses.addWindow(0, 0, _screenSizeY - 1, _screenSizeX, 0);
    _commandWindowId = ncurses.addWindow(_screenSizeY - 1, 0, 1, _screenSizeX, 1);
    _contentWindowCords.x = 0;
    _contentWindowCords.y = 0;
    _currentLine = 0;
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
    bool f = true;
    for(const auto lineBuffer : *editorBuffer.returnText()){
        for(const auto line : lineBuffer){
            ncurses.writeAppendWindow(getContentWindowId(), line);
            _contentWindowCords.y += 1;
            if (_contentWindowCords.y + 1 >= _screenSizeY)
            {
                f = false;
                break;
            }
        }
        if (!f) break;
    }
    _contentWindowCords.y = 0; _contentWindowCords.x = 0;
    ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
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
    ScrollingCode scrl;
    if(isContent == true){
        _contentWindowCords.y -= 1;
        scrl = ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
        if (scrl == ScrollingCode::SCROLL_UP)
        {
            if(_currentLine == 0) return;
            _currentLine -= 1;
            ncurses.scrollWindowUp(getContentWindowId());
            // TODO: Обработка в цикле всех подстрок в строке
            MyString tmp = (*editorBuffer.returnLine(_currentLine))[0];
            ncurses.writeWindow(getContentWindowId(), tmp);
            ncurses.refreshWindow(getContentWindowId());

        }
        if (scrl == ScrollingCode::STAY)
        {
            _currentLine -= 1;
        }
    } else
    {
        _commandWindowCords.y -= 1;
        ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
void EditorView::moveCursorDown(bool isContent)
{
    ScrollingCode scrl;
    if(isContent == true){
        _contentWindowCords.y += 1;
        scrl = ncurses.setCursor(getContentWindowId(), &_contentWindowCords.y, &_contentWindowCords.x);
        if (scrl == ScrollingCode::SCROLL_DOWN)
        {
            if(_currentLine + 1 >= (editorBuffer.getLinesNumber())) return;
            _currentLine += 1;
            // TODO: Обработка в цикле всех подстрок в строке
            MyString tmp = (*editorBuffer.returnLine(_currentLine))[0];
            ncurses.writeAppendWindow(getContentWindowId(), tmp);
            ncurses.refreshWindow(getContentWindowId());
        }
        if (scrl == ScrollingCode::STAY)
        {
            _currentLine += 1;
        }
    } else
    {
        _commandWindowCords.y += 1;
        scrl = ncurses.setCursor(getCmdWindowId(), &_commandWindowCords.y, &_commandWindowCords.x);
    }
}
