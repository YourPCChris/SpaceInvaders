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
        Window(int newWidth=1000, int newHeight=800, std::string newTitle="Space Invaders", Color newColor=Color{0, 0, 139, 255});
        /*
        {
            width = newWidth;
            height = newHeight;
            title = newTitle;
            color = newColor;
        }
        */

        int getWidth() { return width;}
        int getHeight() { return height;}
        std::string getTitle() { return title;}

    private:
        int 
            width,
            height;
        std::string title;
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
        void run();

    private:
        std::vector<std::unique_ptr<GameObj>> objs;
        std::unique_ptr<Window> window = std::make_unique<Window>();
};

#endif

        
