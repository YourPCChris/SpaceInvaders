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
            objs.push_back(std::make_unique<Alien>());
            double objX = i*(objs[i]->getWidth()+20) + 50;
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
    objs[0]->update();
    Player* player = dynamic_cast<Player*>(objs[0].get());
    Alien* alien = dynamic_cast<Alien*>(objs[1].get());
    if (!alien){ std::cerr << "Failed to cast to type alien" << std::endl;}
    std::vector<std::unique_ptr<Bullet>>& bullets = player->getBullets();

    if (player){
        for (int i=1; i < objs.size();)
        {
            alien = dynamic_cast<Alien*>(objs[i].get());
            if (!alien) { std::cerr << "Failed to cast to type Alien" << std::endl;}

            for (auto& bullet : bullets)
            {
                bool isCollision = CheckCollisionCircleRec((Vector2){(float)bullet->getX(), (float)bullet->getY()},
                        (float)bullet->getWidth()/2, 
                        (Rectangle){(float)alien->getX(), (float)alien->getY(), 
                        (float)alien->getWidth(), (float)alien->getHeight()});
                if (isCollision){
                    alien->hit();
                    break;
                }
            }
            if (alien->getHit()){
                objs.erase(objs.begin() + i);
            }else {++i;}
        }
    }else {std::cerr << "Failed to cast to Player type" << std::endl;}
}

void Game::run()
{
    const char* gameTitle = window->getTitle();
    InitWindow(window->getWidth(), window->getHeight(), gameTitle);
    /*
    if (!IsAudioDeviceReady()){
        InitAudioDevice();
    }
    */

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

void GameObj::changeX(double newX) { x = newX;}
void GameObj::changeY(double newY) { y = newY;}
void GameObj::changeDx(double newDy) { dy = newDy;}
void GameObj::changeDy(double newDx) { dx = newDx;}
void GameObj::changeWidth(int newWidth) { width = newWidth;}
void GameObj::changeHeight(int newHeight) { height = newHeight;}
void GameObj::changeColor(Color newColor) { color = newColor;}
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
            //std::cout << "Moving Left" << std::endl;
        }
    }
    else if (IsKeyDown(KEY_RIGHT)){
        if (x + dx <= GetScreenWidth() - (width/2 + 5)){
            x += dx;
            //std::cout << "Moving Right" << std::endl;
        }
    }
    if (IsKeyPressed(KEY_SPACE)){
        bullets.push_back(std::make_unique<Bullet>(x, y, BulletType::Player));
        bullets.back()->changeWidth(10);
        bullets.back()->changeHeight(30);
        bullets.back()->changeColor(RED);
        bullets.back()->changeDy(0.5);
    }
    
    for (int i=0; i < bullets.size() ;)
    {
        if (bullets[i]){
            if (bullets[i]->getY() <= 0){
                bullets.erase(bullets.begin() + i);
            }else{
                bullets[i]->update();
                ++i;
            }
        }else{
            ++i;
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

    //Draw Player Bullets
    for (auto& bullet : bullets)
    {
        bullet->draw();
    }
}

std::vector<std::unique_ptr<Bullet>>& Player::getBullets() { return bullets;}

//-------------Alien------------------
void Alien::update(){}
void Alien::hit() { isHit = true;}
bool Alien::getHit() { return isHit;}

//--------------Bullet-----------------
void Bullet::update()
{
    if (bulletType == BulletType::Player){
        y -= dy;
    }
    else if (bulletType == BulletType::Alien){
        y += dy;
    }
}

void Bullet::draw()
{
    if (bulletType == BulletType::Alien){
        DrawRectangle(x, y, width, height, color);
    }
    else if (bulletType == BulletType::Player){
        DrawCircle(x, y, width/2, color);
    }
}
