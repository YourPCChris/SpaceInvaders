/* 
 * Christopher Bennett
 * Space Invaders Game
 *
 * */

#include <iostream>
#include <vector>
#include <memory>
#include "raylib.h"
#include "class.h"


int main()
{
    std::cout << "We Ball" << std::endl;
    std::cout << "Linked Raylib" << std::endl;

    std::unique_ptr<Game> myGame = std::make_unique<Game>();
    myGame->run();
    std::cout << "Done making stuff" << std::endl;

    return 0;
}

