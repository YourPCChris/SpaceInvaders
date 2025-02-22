/*
 * Christopher Bennett
 * Space invader classes
 * */


#include "class.h"
#include <iostream>
#include "raylib.h"
#include <cmath>

/*
#define LINEONE 50
#define LINETWO 100
#define LINETHREE 150
*/
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
            objs[i]->changeY((double)Lines::LINEONE);
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
    if (objs.size() <= 1){
        //std::cout << "Game Won!!" << std::endl;
        endGame();
    }else if (objs.size() > 1){
        Player* player = dynamic_cast<Player*>(objs[0].get());
        Alien* alien = dynamic_cast<Alien*>(objs[1].get());
        if (!alien){ std::cerr << "Failed to cast to type alien" << std::endl;}
        std::vector<std::unique_ptr<Bullet>>& bullets = player->getBullets();

        if (player){
            for (int i=1; i < objs.size();)
            {
                alien = dynamic_cast<Alien*>(objs[i].get());
                if (!alien) { std::cerr << "Failed to cast to type Alien" << std::endl;}

                if (!objs[i]->update()){endGame();}

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
}

void Game::run()
{
    const char* gameTitle = window->getTitle();
    InitWindow(window->getWidth(), window->getHeight(), gameTitle);
    SetTargetFPS(60);
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
            if (!gameOver){
                updateGameObjs();
                drawGameObjs();
            }
        EndDrawing();
    }
}

void Game::endGame() 
{
    gameOver = (objs.size() == 1) ? true : false;
    showEndScreen(gameOver);
}
void Game::showEndScreen(bool whoWins)
{
    if (whoWins){
        playerWon();
    }else { aliensWon();}
} 
void Game::playerWon() 
{
    window->changeColor(GOLD);
    std::cout << "Player Won!" << std::endl;
}
void Game::aliensWon() 
{
    window->changeColor(RED);
    std::cout << "Aliens Won!" << std::endl;
}

//----------Window----------------
Window::Window(int newWidth, int newHeight, Color newColor)
{
    width = newWidth;
    height = newHeight;
    title = "Space Invaders";
    color = newColor;
}

int Window::getWidth() {return width;}
int Window::getHeight() { return height;}
const char* Window::getTitle() { return title;}
Color Window::getColor() { return color;}
void Window::changeColor(Color newColor) { color = newColor;}

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

bool GameObj::update(){return true;}

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
bool Player::update()
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
    return true;
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
bool Alien::update()
{
   //Change to just increment by 75 to lower resource intensity
   moveTimer += GetFrameTime();

   //if (std::fabs(fmod(now, moveTime)) <= 0.01){
   if (moveTimer >= moveTime){
       x +=dx;

       if (x >= GetScreenWidth() - 20){
           x = minX + width*2;

           switch ((int)y)
           {
               case (int)Lines::LINEONE:
                   y = (double)Lines::LINETWO;
                   x = maxX - width;
                   dx *=-1;
                   break;
              case (int)Lines::LINETHREE:
                   y = (double)Lines::LINEFOUR;
                   x = maxX - width;
                   dx *=-1;
                   break;
              case (int)Lines::LINEFIVE:
                   y = (double)Lines::LINESIX;
                   x = maxX - width;
                   dx *=-1;
                   break;
              case (int)Lines::LASTLINE:
                   //std::cout << "Aliens Win" << std::endl;
                   return false;
                   break;
               default:
                   std::cerr << "Invalid Line for Alien" << std::endl;
           }
       }
       if (x <= 20){
           x = maxX - width;
           switch ((int)y)
           {
               case (int)Lines::LINETWO:
                   y = (double)Lines::LINETHREE;
                   x = minX + width;
                   dx *=-1;
                   break;
               case (int)Lines::LINEFOUR:
                   y = (double)Lines::LINEFIVE;
                   x = minX + width;
                   dx *=-1;
                   break;
               case (int)Lines::LINESIX:
                   y = (double)Lines::LASTLINE;
                   x = minX + width;
                   dx *=-1;
                   break;
               default:
                   std::cerr << "Invalid Line for Alien" << std::endl;
           }
       }

       moveTimer = 0.0f;
       /*
       if (moveTime > 0.5){
           moveTime -= 0.005;
       }
       */
   }
   return true;
}
void Alien::hit() { isHit = true;}
bool Alien::getHit() { return isHit;}

//--------------Bullet-----------------
bool Bullet::update()
{
    if (bulletType == BulletType::Player){
        y -= dy;
    }
    else if (bulletType == BulletType::Alien){
        y += dy;
    }
    return true;
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
