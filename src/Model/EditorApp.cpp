#include "EditorApp.h"


void EditorApp::_readText(std::fstream &file)
{
    MyString line;
    std::vector<MyString> lineBuffer;
    while (file >> line)
    {
        if (line.length() <= _maxLineLength)
        {
            lineBuffer.push_back(line);
            _textToDisplayLinesNumber++;
            _text.push_back(lineBuffer);
        }
        else
        {
            MyString subStr;
            int tmp = line.length() / _maxLineLength;
            for (int i = 0; i < tmp; i++)
            {
                subStr = line.substr(i * _maxLineLength, _maxLineLength);
                lineBuffer.push_back(subStr);
                _textToDisplayLinesNumber++;
            }
            subStr = line.substr(tmp * _maxLineLength, line.length() - tmp * _maxLineLength);
            lineBuffer.push_back(subStr);
            _textToDisplayLinesNumber++;
            
            _text.push_back(lineBuffer);
        }
        lineBuffer.clear();
    }
    _textLinesNumber = _text.size();
}

unsigned int EditorApp::getTextLinesNumbers()
{
    return _textLinesNumber;
}

int EditorApp::getTextToDisplayLinesNumber()
{
    return _textToDisplayLinesNumber;
}

std::vector<std::vector<MyString>> *EditorApp::returnText()
{
    return &_text;
}

std::vector<MyString> *EditorApp::returnLine(unsigned int lineIndex)
{
    if (lineIndex >= _textLinesNumber)
        return nullptr;
    return &_text[lineIndex];
}

bool EditorApp::_isWhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int EditorApp::findStartOfWordL(MyString &line, int startIndex)
{
    int currentIndex = startIndex;
    if (currentIndex >= line.length())
    {
        currentIndex = line.length();
    }
    if (currentIndex == 0)
    {
        return 0;
    }
    do
    {
        --currentIndex;
    } while ((currentIndex > 0 && !_isWhitespace(line[currentIndex - 1])));

    return currentIndex;
}

int EditorApp::findStartOfWordR(MyString &line, int startIndex)
{
    int currentIndex = startIndex;
    if (currentIndex >= line.length())
    {
        return currentIndex;
    }
    do
    {
        ++currentIndex;
    } while ((currentIndex < line.length() && !_isWhitespace(line[currentIndex - 1])));

    return currentIndex;
}

EditorApp::EditorApp(MyString &fileName)
{
    _fileDescr.open(fileName.c_str());
}

void EditorApp::addView(EditorView* view)
{
    _editorView = view;
}

void EditorApp::readToBuffer(unsigned int maxLineLen)
{
    _maxLineLength = maxLineLen;
    _readText(_fileDescr);
}

int EditorApp::vectorOfIntsToInt(const std::vector<int> &vector)
{
    int result = 0;
    for (int digit : vector) {

        int digitValue = digit - '0';

        // Check for overflow before updating the result
        if (result > (std::numeric_limits<int>::max() - digitValue) / 10)
        {
            std::cerr << "Error: Overflow occurred.\n";
            return 0; // or handle the overflow in your specific way
        }

        result = result * 10 + digitValue;
    }
    return result;
}

int EditorApp::rebalanceLine(int lineNumber)
{
    enum actionFlag {
        STAY = 0,
        NEED_NEW_LINE = 1,
        NEED_DELETE_LINE = -1
    } flag = STAY;

    int subLineNumber = 0;

    for (auto subLine : _text[lineNumber])
    {
        int subLineLen = subLine.length();
        if (subLineLen > _maxLineLength)
        {
            flag = NEED_NEW_LINE;
            break;
        }
        if (subLineLen <= 0)
        {
            flag = NEED_DELETE_LINE;
            // subLineNumber = 0;
            break;
        }
        subLineNumber++;
    }
    
    if (flag == STAY) return 0;
    
    // Actually need to rebalance 

    if (flag == NEED_NEW_LINE)
    {
        if (subLineNumber == _text[lineNumber].size() - 1)
        {
            // Create new line with only one char -- last char of the current full substring and add it to the substrings
            MyString newLine;
            newLine.append(1, _text[lineNumber][subLineNumber][_text[lineNumber][subLineNumber].size()-1]);
            _text[lineNumber][subLineNumber].erase(_text[lineNumber][subLineNumber].length() - 1, 1);
            _text[lineNumber].push_back(newLine);
            _textToDisplayLinesNumber++;
            return 1;
        }
        

        // Delete the last char of current substring and add it to the begin of next; Rebalance can be needed again  
        _text[lineNumber][subLineNumber + 1].insert(0, 1, _text[lineNumber][subLineNumber][_text[lineNumber][subLineNumber].size()-1]);
        _text[lineNumber][subLineNumber].erase(_text[lineNumber][subLineNumber].length() - 1, 1);
        rebalanceLine(lineNumber);
        return 1;
    }

    if (flag == NEED_DELETE_LINE)
    {
        _text[lineNumber].erase(_text[lineNumber].begin() + subLineNumber);
        _textToDisplayLinesNumber--;
        if (_text[lineNumber].size() == 0)
        {
            _text.erase(_text.begin() + lineNumber);
            _textLinesNumber--;
        }

        return -1;
    }
}

// unsigned int EditorApp::getRealLinesNumbers

int EditorApp::_deleteChar(unsigned int lineNumber, unsigned int subLineNumber, unsigned int charIndex)
{
    MyString* aa = &_text[lineNumber][subLineNumber];
    _text[lineNumber][subLineNumber].erase(charIndex, 1);
    return rebalanceLine(lineNumber);
}

int EditorApp::_insertChar(unsigned int lineNumber, unsigned int subLineNumber, int c, unsigned int charIndex)
{
    MyString* aa = &_text[lineNumber][subLineNumber];
    _text[lineNumber][subLineNumber].insert(charIndex, 1, c);
    return rebalanceLine(lineNumber);
}
// 1234 68901 121314
void EditorApp::deleteCharAfterCursor()
{
    
    _editorView->updateContentLine(_deleteChar(_editorView->getCurrentTextLine(), _editorView->getCurrentSubTextLine(), _editorView->getContentCurrentLineX()));

}

void EditorApp::deleteWordAfterCursor()
{
    WindowCords newCords;
    auto textLine = _text[_editorView->getCurrentTextLine()][_editorView->getCurrentSubTextLine()];
    int begWordIndex = findStartOfWordL(textLine, _editorView->getContentCurrentLineX());
    int i = 0, _oldCurrentLine = _textToDisplayLinesNumber, _oldCurrentSubLine = _editorView->getCurrentSubTextLine();
    for (i = begWordIndex; i < textLine.size() && textLine[i] != ' '; i++)
    {
        _deleteChar(_editorView->getCurrentTextLine(), _editorView->getCurrentSubTextLine(), begWordIndex);

    }
    if (_oldCurrentLine != _textToDisplayLinesNumber)
    {
        _editorView->updateContentLine(-1);
        return;
    }


    if (begWordIndex < textLine.size()) _deleteChar(_editorView->getCurrentTextLine(), _editorView->getCurrentSubTextLine(), begWordIndex);
    _editorView->updateContentLine(0);



}

void EditorApp::copyCurrentLineToBuffer()
{
    _userBuffer = _text[_editorView->getCurrentTextLine()];
}

void EditorApp::pasteUserBuffer()
{
    if (_userBuffer.size() == 0) return;
    _insertNewLine(_userBuffer);

}

void EditorApp::_insertNewLine(std::vector<MyString> line)
{   
    // _text[lineNumber][subLineNumber + 1].insert(0, 1, _text[lineNumber][subLineNumber].length() - 1);
    _text.insert(_text.begin() + _editorView->getCurrentTextLine(), line);
    _textLinesNumber++;
    _textToDisplayLinesNumber += line.size();
    _editorView->updateContentLine(-1);
    
}

void EditorApp::_deleteLine(int lineNumber)
{
    _textLinesNumber--;
    _textToDisplayLinesNumber -= _text[lineNumber].size();
    _text.erase(_text.begin() + lineNumber);   
    _editorView->updateContentLine(-1);

}

void EditorApp::deleteCurrentLine()
{
    _deleteLine(_editorView->getCurrentTextLine());
}

void EditorApp::copyCurrentWordToBuffer()
{
    // WindowCords newCords;
    MyString tmp;
    auto textLine = _text[_editorView->getCurrentTextLine()][_editorView->getCurrentSubTextLine()];
    int begWordIndex = findStartOfWordL(textLine, _editorView->getContentCurrentLineX());
    int i = 0;
    for (i = begWordIndex; i < textLine.size() && textLine[i] != ' '; i++)
    {
        tmp.append(1, textLine[i]);
    }
    _userBuffer.clear();
    _userBuffer.push_back(tmp);
}

void EditorApp::insertCharAfterCursor(int c)
{
    _editorView->updateContentLine(_insertChar(
        _editorView->getCurrentTextLine(), 
        _editorView->getCurrentSubTextLine(), 
        c, 
        _editorView->getContentCurrentLineX()
    ));
    _editorView->moveCursorRight(true);
}

void EditorApp::insertEmptyLine()
{
    std::vector<MyString> tmp;
    tmp.push_back(MyString(""));
    _insertNewLine(tmp);
}


EditorApp::~EditorApp()
{
    _fileDescr.close();
}
