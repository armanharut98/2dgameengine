#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

const int FPS = 60;
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game
{
private:
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool isRunning;
    int millisecsPreviousFrame = 0;

public:
    Game();
    ~Game();
    void Initialize();
    void Run();
    void Destroy();
    void ProcessInput();
    void Setup();
    void Update();
    void Render();
    int windowWidth;
    int windowHeight;
};

#endif
