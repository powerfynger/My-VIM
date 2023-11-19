#include "Controller/EditorController.h"
#include "Model/EditorApp.h"
#include "View/EditorView.h"

#include <iostream>

int main(int argc, char **argv)
{
    argc = 2;
    if (argc > 1)
    {
        // MyString fileName(argv[1]);
        MyString fileName("aa");
        EditorApp app(fileName);
        app.readToBuffer();
        EditorView view(app.buf);
        EditorController controller(app, view);
        view.displayAllText();
        while (true) controller.handleInput();
        // controller.handleInput();
    }
    else
    {
        std::cout << "No file provided!";
    }
    return 0;
}