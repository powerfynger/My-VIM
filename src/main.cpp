#include "Controller/EditorController.h"
#include "Model/EditorApp.h"
#include "View/EditorView.h"

#include <iostream>

int main(int argc, char **argv)
{
    
    if (argc > 1)
    {
        MyString fileName(argv[1]);
        EditorView view;
        EditorApp app(fileName);
        EditorController controller(app, view);
        MyString a("--NAVIGATION--");
        view.ncurses.writeWindow(view.getCmdWindowId(), a);
        view.ncurses.refreshWindow(view.getCmdWindowId());
        a = "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111";
        view.ncurses.writeWindow(view.getContentWindowId(), a);
        view.ncurses.writeWindow(view.getContentWindowId(), a);
        a = "bbbbbbbbbbbbbbbbbbbbbbbbbbbb";
        view.ncurses.writeWindow(view.getContentWindowId(), a);
        view.ncurses.refreshWindow(view.getContentWindowId());
        

        // while (true) controller.handleInput();
        controller.handleInput();

    }
    else
    {
        std::cout << "No file provided!";
    }
    return 0;
}