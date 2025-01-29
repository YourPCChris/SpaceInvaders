/*
 * Christopher Bennett
 * Space invader classes
 * */


#include "class.h"
#include <iostream>
#include "raylib.h"

#define LINEONE 50
#define LINETWO 100
#define LINETHREE 150

//------------------Game--------------------
Game::Game()
{
    window = std::make_unique<Window>();
}

bool Game::makeGameObjs(int num)
{
    try
    {
        objs.push_back(std::make_unique<Player>());
        for (int i=1; i < num ; i++)
        {
            objs.push_back(std::make_unique<GameObj>());
            int objX = i*(objs[i]->getWidth()+20) + 50;
            objs[i]->changeX(objX);
            objs[i]->changeY(LINEONE);
        }
    }catch (std::exception& e) { return false;}
    return true;
}

void Game::drawGameObjs()
{
    for (int j=0; j < objs.size() ; j++)
    {
        objs[j]->draw();
    }
}

void Game::updateGameObjs()
{
    for (auto& obj : objs)
    {
        obj->update();
    }

}

void Game::run()
{
    const char* gameTitle = window->getTitle();
    InitWindow(window->getWidth(), window->getHeight(), gameTitle);
    makeGameObjs(30);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(window->getColor());
            updateGameObjs();
            drawGameObjs();
        EndDrawing();
    }
}

//----------Window----------------
Window::Window(int newWidth, int newHeight, Color newColor)
{
    width = newWidth;
    height = newHeight;
    title = "Space Invaders";
    color = newColor;
}


//--------------GameObj-------------------------
GameObj::GameObj(double newX, double newY, int newWidth, int newHeight, double newDx, double newDy, Color newColor)
{
    x = newX;
    y = newY;
    width = newWidth;
    height = newHeight;
    color = newColor;
    dx = newDx;
    dy = newDy;
}

void GameObj::changeX(int newX) { x = newX;}
void GameObj::changeY(int newY) { y = newY;}
int GameObj::getWidth() { return width;}
int GameObj::getHeight() {return height;}
int GameObj::getX() { return x;}
int GameObj::getY() { return y;}

void GameObj::update(){}

void GameObj::draw()
{
    std::pair<int, int> innerStrip = {x - width/2 + width/3, x - width/2 + width*0.6};
    for (int i=x - width/2; i < (x + width/2) ; i++)
    {
        for (int j=y - height/2; j < (y+height/2) ; j++)
        {
            if (i < innerStrip.first || i > innerStrip.second){
                DrawPixel(i, j, WHITE);
            }
            else {
                DrawPixel(i, j, color);
            }
        }
    }
}

//---------------Player-------------------------------
void Player::update()
{
    if (IsKeyDown(KEY_LEFT)){
        if (x - dx >= width/2 + 5){
            x -= dx;
            std::cout << "Moving Left" << std::endl;
        }
    }
    else if (IsKeyDown(KEY_RIGHT)){
        if (x + dx <= GetScreenWidth() - (width/2 + 5)){
            x += dx;
            std::cout << "Moving Right" << std::endl;
        }
    }
}

void Player::draw()
{
    for (int i=x - width/2; i < x + width/2 ; i++)
    {
        for (int j=y - height/2; j < y + height/2 ; j++)
        {
            if ((i - x)*(i - x) + (j - y)*(j-y) <= (width/2)*(width/2)){
                if ((i - x) * (i - x) + (j - y)*(j-y) >= (width/4)*(width/4)){
                    DrawPixel(i, j, color);
                }
            }
        }
    }
    DrawCircleLines(x, y, width/2, RAYWHITE);
}
