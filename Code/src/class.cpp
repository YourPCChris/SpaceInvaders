/*
 * Christopher Bennett
 * Space invader classes
 * */


#include "class.h"
#include <iostream>
#include "raylib.h"

//Window
Window::Window(int newWidth, int newHeight, std::string newTitle, Color newColor)
{
    width = newWidth;
    height = newHeight;
    title = newTitle;
    color = newColor;
}


//GameObj
GameObj::GameObj(int newX, int newY, int newWidth, int newHeight, Color newColor)
{
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
    color = newColor;
}
