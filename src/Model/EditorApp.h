#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include <fstream>
#include <vector>
#include <limits>

class Buffer
{
public:
    Buffer();
    void readText(std::fstream& file, unsigned int maxLineLen);
    std::vector<std::vector<MyString>>* returnText();
    std::vector<MyString>* returnLine(unsigned int lineIndex);

    bool isWhitespace(char c);
    int findStartOfWordL(MyString& line, int startIndex);
    int findStartOfWordR(MyString& line, int startIndex);

    void writeText(std::fstream& file);

    unsigned int getLinesNumber();

private:
    unsigned int _textLinesNumber;
    std::vector<std::vector<MyString>> _text;
};

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    void readToBuffer(unsigned int maxLineLen);
    int vectorOfIntsToInt(const std::vector<int>& vector);
    ~EditorApp();

    Buffer buf;
private:
    unsigned int _maxLineLength;
    std::fstream _fileDescr;
};

#endif