/*
 * Class Header File 
 * */


#ifndef CLASS_H
#define CLASS_H 

#include <iostream>
#include <string>
#include "raylib.h"
#include <memory>
#include <vector>


class GameObj
{
    public:
        GameObj(double newX=-1, double newY=-1, int newWidth=20, int newHeight=20, double newDx=0.5, double newDy=2, Color newColor=BLACK);

        void changeX(double newX);
        void changeY(double newY);
        void changeDx(double newDx);
        void changeDy(double newDy);
        void changeWidth(int newWidth);
        void changeHeight(int newHeight);
        void changeColor(Color newColor);

        int getX();
        int getY();
        int getWidth();
        int getHeight();
        int getDx();
        int getDy();
        Color getColor();

        virtual void draw();
        virtual void update();

        virtual ~GameObj() = default;

    protected:
        int 
            width,
            height;
        double
            x,
            y,
            dx,
            dy;
        Color color;
};

enum class BulletType { Player=0, Alien=1};
class Bullet : public GameObj
{
    public:
        Bullet(double bulletX, double bulletY, BulletType type) :
            GameObj(bulletX, bulletY)
        {
            bulletType = type;
        }

        void update() override;
        void draw() override;

    private:
        BulletType bulletType;
};

enum class Lines{ LINEONE=50, LINETWO=100, LINETHREE=150, LINEFOUR=200};
class Alien : public GameObj
{
    public:
        Alien() : GameObj()
        {
            isHit=false;
            maxX = GetScreenWidth() - width;
            dx = 4;
        }

        void update() override;
        void hit();
        bool getHit();

        ~Alien()
        {
            bullets.clear();
        }

    private:
        std::vector<std::unique_ptr<Bullet>> bullets;
        double moveTime = 2.00;
        double minX = 20;
        double maxX;
        bool isHit;
};

class Player: public GameObj
{
    public:
        Player() : GameObj(GetScreenWidth()/2, GetScreenHeight()*0.8, 20, 20, 1, 2, BLUE)
        {
            /*
            if (IsAudioDeviceReady()){
                zap = LoadSound("sound/zap.wav");
            }else {std::cerr << "Audio Device not ready" << std::endl;}
            */

            isHit=false;
        }

        void update() override;
        void draw() override;
        std::vector<std::unique_ptr<Bullet>>& getBullets();

        ~Player()
        {
            bullets.clear();
            /*
            if (IsSoundReady(zap)){
                UnloadSound(zap);
            }
            */
        }

    private:
        std::vector<std::unique_ptr<Bullet>> bullets;
        bool isHit;
        //Sound zap;
};



class Window
{
    public:
        Window(int newWidth=1000, int newHeight=800, Color newColor=Color{0, 0, 20, 255});

        int getWidth() { return width;}
        int getHeight() { return height;}
        const char* getTitle() { return title;}
        Color getColor() { return color;}

    private:
        int 
            width,
            height;
        const char* title;
        Color color;
};

class Game 
{
    public:
        Game();
        bool makeGameObjs(int num=20);
        void drawGameObjs();
        void updateGameObjs();
        void run();

        ~Game()
        {
            objs.clear();
            window.reset();
            /*
            if (IsAudioDeviceReady()){
                CloseAudioDevice();
            }
            */
            CloseWindow();
        }

    private:
        std::vector<std::unique_ptr<GameObj>> objs;
        std::unique_ptr<Window> window = std::make_unique<Window>();
};


#endif
