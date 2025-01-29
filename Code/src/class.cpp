/*
 * Christopher Bennett
 * Space invader classes
 * */


#include "class.h"
#include <iostream>
#include "raylib.h"

//Game
Game::Game()
{
    window = std::make_unique<Window>();
}

void Game::run()
{
    const char* gameTitle = window->getTitle();
    InitWindow(window->getWidth(), window->getHeight(), gameTitle);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(window->getColor());
        EndDrawing();
    }
}
//Window
Window::Window(int newWidth, int newHeight, Color newColor)
{
    width = newWidth;
    height = newHeight;
    title = "Space Invaders";
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
