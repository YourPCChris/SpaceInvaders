/*
 * Class Header File 
 * */


#ifndef CLASS_H
#define CLASS_H 

#include <string>
#include "raylib.h"
#include <memory>
#include <vector>

class Window
{
    public:
        Window(int newWidth=1000, int newHeight=800, Color newColor=Color{0, 0, 139, 255});

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

class GameObj
{
    public:
        GameObj(int newX, int newY, int newWidth, int newHeight, Color newColor=BLACK);

    protected:
        int 
            width,
            height,
            x,
            y;
        Color color;
};

class Game 
{
    public:
        Game();
        //Make window 
        void run();

    private:
        std::vector<std::unique_ptr<GameObj>> objs;
        std::unique_ptr<Window> window = std::make_unique<Window>();
};

#endif

        
