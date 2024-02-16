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

std::vector<std::vector<MyString>> *EditorApp::getText()
{
    return &_text;
}

std::vector<MyString> *EditorApp::returnLine(unsigned int lineIndex)
{
    if (lineIndex >= _textLinesNumber)
        return nullptr;
    return &_text[lineIndex];
}

MyString EditorApp::getCommand()
{
    return _commandBuffer;
}

void EditorApp::clearCommand()
{
    _commandBuffer.clear();
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
    _currFileName = fileName;
    _fileDescr.open(fileName.c_str(), std::ios::out|std::ios::in);
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
    return 0;
}

// unsigned int EditorApp::getRealLinesNumbers

int EditorApp::_deleteChar(unsigned int lineNumber, unsigned int subLineNumber, int charIndex, bool isContent)
{
    if(isContent)
    {
        if(charIndex < 0)
        {
            addCurrentLineToPrevious();
            return 0;
        }
        else _text[lineNumber][subLineNumber].erase(charIndex, 1);
        return rebalanceLine(lineNumber);
    }
    else
    {
        _commandBuffer.erase(charIndex, 1);
        return 0;
    }
}

int EditorApp::_insertCharToText(unsigned int lineNumber, unsigned int subLineNumber, int c, unsigned int charIndex)
{
    _text[lineNumber][subLineNumber].insert(charIndex, 1, c);
    return rebalanceLine(lineNumber);
}

int EditorApp::_insertCharToCommand(int c, unsigned int charIndex)
{
    _commandBuffer.insert(charIndex, 1, c);
    return 0;
}

// 1234 68901 121314
void EditorApp::deleteCharAfterCursor(bool isContent)
{

    if(isContent){
        _editorView->updateContentLine(_deleteChar(
            _editorView->getCurrentTextLine(), 
            _editorView->getCurrentSubTextLine(), 
            _editorView->getContentCurrentLineX(),
            isContent
        ));
    }
    else
    {
        
    }
}
void EditorApp::deleteCharBeforeCursor(bool isContent)
{

    if(isContent){

        _editorView->updateContentLine(_deleteChar(
            _editorView->getCurrentTextLine(), 
            _editorView->getCurrentSubTextLine(), 
            _editorView->getContentCurrentLineX() - 1,
            true
        ));
    }
    else
    {

        _deleteChar(0, 0, _editorView->getCommandCurrentLineX() - 1, false);
        _editorView->updateCommandLine();

        
    }
}

void EditorApp::deleteWordAfterCursor()
{
    WindowCords newCords;
    auto textLine = _text[_editorView->getCurrentTextLine()][_editorView->getCurrentSubTextLine()];
    int begWordIndex = findStartOfWordL(textLine, _editorView->getContentCurrentLineX());
    int i = 0, _oldCurrentLine = _textToDisplayLinesNumber, _oldCurrentSubLine = _editorView->getCurrentSubTextLine();
    for (i = begWordIndex; i < textLine.size() && textLine[i] != ' '; i++)
    {
        _deleteChar(_editorView->getCurrentTextLine(), _editorView->getCurrentSubTextLine(), begWordIndex, true);

    }
    if (_oldCurrentLine != _textToDisplayLinesNumber)
    {
        _editorView->updateContentLine(-1);
        return;
    }


    if (begWordIndex < textLine.size()) _deleteChar(_editorView->getCurrentTextLine(), _editorView->getCurrentSubTextLine(), begWordIndex, true);
    _editorView->updateContentLine(0);



}

void EditorApp::copyCurrentLineToBuffer()
{
    _userBuffer = _text[_editorView->getCurrentTextLine()];
}

void EditorApp::pasteUserBuffer()
{
    if (_userBuffer.size() == 0) return;
    _insertNewLine(_userBuffer, _editorView->getCurrentTextLine());

}

void EditorApp::_insertNewLine(std::vector<MyString> line, unsigned int lineNumber)
{   
    // _text[lineNumber][subLineNumber + 1].insert(0, 1, _text[lineNumber][subLineNumber].length() - 1);
    _text.insert(_text.begin() + lineNumber, line);
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


void EditorApp::_rebalanceVectorOfLines(int vectorNumber)
{
    MyString tmp;

    for(auto line : _text[vectorNumber])
    {
        _textToDisplayLinesNumber--;
        tmp.append(line.c_str());
    }
    _text[vectorNumber].clear();

    int tmpLen = tmp.length() / _maxLineLength;
    for (int i = 0; i < tmpLen; i++)
    {
        // subStr = line.substr(i * _maxLineLength, _maxLineLength);
        // lineBuffer.push_back(subStr);
        _text[vectorNumber].push_back(tmp.substr(i * _maxLineLength, _maxLineLength));
        _textToDisplayLinesNumber++;
    // _textToDisplayLinesNumber++;
    }
    // subStr = line.substr(tmp * _maxLineLength, line.length() - tmp * _maxLineLength);
    // lineBuffer.push_back(subStr);
    _text[vectorNumber].push_back(tmp.substr(tmpLen * _maxLineLength, tmp.length() - tmpLen * _maxLineLength));
    _textToDisplayLinesNumber++;

    
    // _text.push_back(lineBuffer);s


}

void EditorApp::divideCurrentLineAfterCursor()
{
    int tmpCurrentTextLine = _editorView->getCurrentTextLine();
    int tmpCurrentSubTextLine = _editorView->getCurrentSubTextLine();
    int tmpCurrentIndex = _editorView->getContentCurrentLineX();
    
    MyString tmpString = _text[tmpCurrentTextLine][tmpCurrentSubTextLine].substr(tmpCurrentIndex);
    _text[tmpCurrentTextLine][tmpCurrentSubTextLine].erase(tmpCurrentIndex, _text[tmpCurrentTextLine][tmpCurrentSubTextLine].length() - tmpCurrentIndex);

    std::vector<MyString> tmpVector(_text[tmpCurrentTextLine].begin() + tmpCurrentSubTextLine, _text[tmpCurrentTextLine].end());
    if (tmpCurrentSubTextLine == 0) tmpCurrentSubTextLine++;
    _text[tmpCurrentTextLine].erase(_text[tmpCurrentTextLine].begin() + tmpCurrentSubTextLine, _text[tmpCurrentTextLine].end());

    tmpVector[0] = tmpString;
    _insertNewLine(tmpVector, tmpCurrentTextLine + 1);
    _rebalanceVectorOfLines(tmpCurrentTextLine + 1);

    _editorView->updateContentLine(-1);
}

void EditorApp::addCurrentLineToPrevious()
{
    int tmpCurrentTextLine = _editorView->getCurrentTextLine();
    int tmpCurrentSubTextLine = _editorView->getCurrentSubTextLine();
    int tmpCurrentIndex = _editorView->getContentCurrentLineX();

    if (tmpCurrentTextLine == 0 && tmpCurrentSubTextLine == 0) return;

    _editorView->moveCursorLineNumber(tmpCurrentTextLine-1);
    _editorView->moveCursorEndLine(true);
    for(MyString& line : _text[tmpCurrentTextLine])
    {
        _text[tmpCurrentTextLine - 1][_text[tmpCurrentTextLine - 1].size()-1].append(line.c_str());
    }
    _text.erase(_text.begin() + tmpCurrentTextLine);    
    _textLinesNumber--;
    _rebalanceVectorOfLines(tmpCurrentTextLine-1);
    _editorView->updateContentLine(-1);

}

void EditorApp::deleteCurrentLine()
{
    if (_textLinesNumber <= 0) return;
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

void EditorApp::insertCharAfterCursor(int c, bool isContent)
{
    if (isContent)
    {
        _editorView->updateContentLine(_insertCharToText(
            _editorView->getCurrentTextLine(), 
            _editorView->getCurrentSubTextLine(), 
            c, 
            _editorView->getContentCurrentLineX()
        ));
        _editorView->moveCursorRight(true);
    }
    else
    {
        _insertCharToCommand(c, _editorView->getCommandCurrentLineX());
        _editorView->updateCommandLine();
        _editorView->moveCursorRight(false);
    }
}

void EditorApp::insertEmptyLine()
{
    std::vector<MyString> tmp;
    tmp.push_back(MyString(""));
    _insertNewLine(tmp, _editorView->getCurrentTextLine());
}

void EditorApp::processCommandInput()
{
    if (_commandBuffer.length() == 1) return;
    switch (_commandBuffer[0])
    {
    case ':':
        processCommand();
        break;
    case '?':
        _commandBuffer.erase(0, 1);
        processSearch(true);
        break;
    case '/':
        _commandBuffer.erase(0, 1);
        processSearch(false);
        break;
    default:
        break;
    }
    
}

void EditorApp::processSearch(bool isReverse)
{
    unsigned int tmpTextLine, tmpSubTextLine;
    if (_findText(_commandBuffer, &tmpTextLine, &tmpSubTextLine, isReverse))
    {
        _editorView->moveCursorLineNumber(tmpTextLine);
        for (int i = 0; i < tmpSubTextLine; i++)
        {
            _editorView->moveCursorDown(true);
        }
    }
}

void EditorApp::processCommand()
{
    _commandBuffer.erase(0, 1);

    if (_commandBuffer[0] == 'o')
    {
        processCommandOpenFile();
    }
    else if (!my_strcmp(_commandBuffer.c_str(), "q!") || !my_strcmp(_commandBuffer.c_str(), "q!"))
    {
        forceExit();
    }
    else if(_commandBuffer[0] == 'w')
    {
        processCommandSaveToFile();

    }
    else if (!my_strcmp(_commandBuffer.c_str(), "x") || !my_strcmp(_commandBuffer.c_str(), "wq!"))
    {
        _writeText(_currFileName);
        forceExit();
    }
    else
    {
        int tmp;
        if (_isNumeric(_commandBuffer))
        {
            if (_stringToNumber(_commandBuffer, tmp))
            {
                if (tmp < _textLinesNumber)
                {
                    _editorView->moveCursorLineNumber(tmp);
                    _editorView->ncurses.refreshWindow(_editorView->getContentWindowId());
                }
            }
        }
    }
}

void EditorApp::processCommandSaveToFile()
{
    MyString fileName(_commandBuffer.substr(2, _commandBuffer.length() - 2));
    if (fileName.length() != 0)
    {
        std::fstream tmp;
        tmp.open(fileName.c_str());
        _writeText(fileName);
    }
    else
    {
        _writeText(_currFileName);
    }
}


void EditorApp::processCommandOpenFile()
{
    MyString fileName(_commandBuffer.substr(2));

    if(_isFileExist(fileName)) readFromFile(fileName);
}

void EditorApp::forceExit()
{
    _editorView->ncurses.endNcurses();
    _fileDescr.close();
    exit(EXIT_SUCCESS);
}

void EditorApp::readFromFile(MyString fileName)
{
    _text.clear();
    _fileDescr.close();

    _fileDescr.open(fileName.c_str());
    readToBuffer(_editorView->getScrSizeX() - 2);
    _editorView->reloadView();
    _editorView->displayAllText();

}

bool EditorApp::_isFileExist (MyString& flineName) {
    if (FILE *file = fopen(flineName.data(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}

bool EditorApp::_findText(MyString userText, unsigned int *textLine, unsigned int *textSubLine, bool isReverse)
{
    if (!isReverse)
    {
        for (int i = _editorView->getCurrentTextLine() + 1; i < _text.size(); i++)
        {
            for (int j = 0; j < _text[i].size(); j++)
            {
                if (_text[i][j].find(userText.c_str()) != -1)
                {
                    *textLine = i; *textSubLine = j; return true;
                }
            }
        }
    }
    if (isReverse)
    {
        for (int i = _editorView->getCurrentTextLine() - 1; i >= 0; i--)
        {
            for (int j = _text[i].size() - 1; j >= 0; j--)
            {
                if (_text[i][j].find(userText.c_str()) != -1)
                {
                    *textLine = i; *textSubLine = j; return true;
                }
            }
        }
    }
    return false;
}


bool EditorApp::_isNumeric(MyString& line) {
    std::istringstream iss(line.c_str());
    double dummy;
    iss >> std::noskipws >> dummy;
    return iss.eof() && !iss.fail();
}

bool EditorApp::_stringToNumber(MyString& line, int& result) {
    std::istringstream iss(line.c_str());
    iss >> std::noskipws >> result;
    return !iss.fail() && iss.eof();
}

void EditorApp::_writeText(MyString fileName)
{   
    _test = fopen(fileName.c_str(), "w");
    for (const auto lineBuffer : _text)
    {
        for (auto line : lineBuffer)
        {
            // file.write(line.c_str(), line.length());
            fprintf(_test, "%s", line.c_str());
            // file << line.c_str();

        }
        // file << "\n";
        fprintf(_test, "\n");
    }
    // file.flush();

    fclose(_test);

}

EditorApp::~EditorApp()
{
    _fileDescr.close();
}
