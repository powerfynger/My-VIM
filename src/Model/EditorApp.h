#ifndef EDITORAPP_H
#define EDITORAPP_H

#include "../Utils/MyString.h"
#include <fstream>
#include <vector>

class Buffer
{
public:
    Buffer() {};
    void readText(std::fstream& file);
    void writeText(std::fstream& file);
    void initTextToDisplay(unsigned int max_x, unsigned int max_y);
    void updateTextToDisplay();

    std::vector<MyString> textToDisplay;

private:
    int _firstLine, _lastLine;
    std::vector<MyString> _text;


};

class EditorApp
{
public:
    explicit EditorApp(MyString &fileName);
    ~EditorApp();
    void readToBuffer();

private:
    std::fstream _fileDescr;
    Buffer _buf;
};

#endif