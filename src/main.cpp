#include "Controller/EditorController.h"
#include "Model/EditorApp.h"
#include "View/EditorView.h"

#include <iostream>
#include <thread>

int main(int argc, char **argv)
{
    argc = 2;
    if (argc > 1)
    {
        // MyString fileName("../src/main.cpp");
        // if(argv[1] == NULL) return 1;
        // MyString fileName(argv[1]);
        MyString fileName("example.txt");
        EditorApp app(fileName);
        EditorView view(app);
        app.addView(&view);
        EditorController controller(app, view);
        app.readToBuffer(view.getScrSizeX() - 2);
        view.displayAllText();
        
        std::thread inputThread([&controller]() {
            while (true) controller.handleInput();
        });
        inputThread.join();


    }
    else
    {
        std::cout << "No file provided!";
    }
    return 0;
}