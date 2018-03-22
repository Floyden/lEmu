#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstring>

#include <SDL2/SDL.h>

#include "Memory.h"
#include "CPU.h"
//
// std::vector<char> LoadRom(std::string filename)
// {
//     std::ifstream file(filename, std::ios::binary);
//     std::vector<char> buffer(
//         (std::istreambuf_iterator<char>(file)),
//         (std::istreambuf_iterator<char>())
//     );
//
//     return buffer;
// }

int main(int argv, char** args)
{
    CPU cpu;
    cpu.init();
    cpu.run();

    /*
    SDL_Window* window = nullptr;

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "Failed to init SDL video\n";
        return 0;
    }

    window = SDL_CreateWindow("Solis", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        std::cout << "Failed to create a window\n";
        SDL_Quit();
    }

    SDL_Quit();*/
}
