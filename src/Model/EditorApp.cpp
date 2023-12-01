#include "EditorApp.h"

Buffer::Buffer()
{

}

void Buffer::readText(std::fstream& file, unsigned int maxLineLen){
    MyString line;
    std::vector<MyString> lineBuffer;
    while(file >> line){
        if(line.length() <= maxLineLen)
        {
            lineBuffer.push_back(line);
            _text.push_back(lineBuffer);
        } else
        {
            MyString subStr;
            int tmp = line.length() / maxLineLen;
            for (int i = 0; i < tmp; i++){
                subStr = line.substr(i * maxLineLen, i * maxLineLen + maxLineLen);
                lineBuffer.push_back(subStr);
            }
            subStr = line.substr(tmp * maxLineLen, line.length() - tmp * maxLineLen);
            lineBuffer.push_back(subStr);
            _text.push_back(lineBuffer);
        }
        lineBuffer.clear();
    }
    _textLinesNumber = _text.size();
}
/*
[
    ["asdasas ads",  "asss aaaa", " dsssssd ", " a "], 0
    ["asd", "aaa", "dd ", "a"], 1
    ["asd", "aaa", "dd ", "a"], 2
    ["asd", "aaa", "dd ", "a"], 3
]

*/
unsigned int Buffer::getLinesNumber()
{
    return _textLinesNumber;
}

std::vector<std::vector<MyString>>* Buffer::returnText(){
    return &_text;
}
std::vector<MyString>* Buffer::returnLine(unsigned int lineIndex)
{
    return &_text[lineIndex];
}

EditorApp::EditorApp(MyString &fileName){
    _fileDescr.open(fileName.c_str());

}

void EditorApp::readToBuffer(unsigned int maxLineLen){
    buf.readText(_fileDescr, maxLineLen);
}

EditorApp::~EditorApp(){
    _fileDescr.close();

}

