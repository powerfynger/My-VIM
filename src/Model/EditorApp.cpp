#include "EditorApp.h"


void EditorApp::readText(std::fstream &file, unsigned int maxLineLen)
{
    MyString line;
    std::vector<MyString> lineBuffer;
    while (file >> line)
    {
        if (line.length() <= maxLineLen)
        {
            lineBuffer.push_back(line);
            _textToDisplayLinesNumber++;
            _text.push_back(lineBuffer);
        }
        else
        {
            MyString subStr;
            int tmp = line.length() / maxLineLen;
            for (int i = 0; i < tmp; i++)
            {
                subStr = line.substr(i * maxLineLen, maxLineLen);
                lineBuffer.push_back(subStr);
                _textToDisplayLinesNumber++;
            }
            subStr = line.substr(tmp * maxLineLen, line.length() - tmp * maxLineLen);
            lineBuffer.push_back(subStr);
            _textToDisplayLinesNumber++;
            
            _text.push_back(lineBuffer);
        }
        lineBuffer.clear();
    }
    _textLinesNumber = _text.size();
}

unsigned int EditorApp::getRealLinesNumbers()
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

void EditorApp::readToBuffer(unsigned int maxLineLen)
{
    readText(_fileDescr, maxLineLen);
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

EditorApp::~EditorApp()
{
    _fileDescr.close();
}
