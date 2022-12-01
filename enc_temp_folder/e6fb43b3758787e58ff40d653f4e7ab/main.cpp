#include "pch.h"
#include <SDL.h>
#include "image_loader.h"

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;

struct Application
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

SDL_Renderer* InitializeRenderer(SDL_Window* window)
{
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        printf("Failed to create renderer! SDL_Error: %s\n", SDL_GetError());
        return nullptr;
    }

    //Initialize PNG and JPEG loading
    constexpr int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;

    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return nullptr;
    }

    SDL_SetRenderDrawColor(renderer, 62, 53, 70, 0xFF);
    return renderer;
}

SDL_Window* InitializeApplication()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL! Error: %s\n", SDL_GetError());
        return nullptr;
    }

    SDL_Window* window = SDL_CreateWindow("SDL Space Shooter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    if (window == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return nullptr;
    }

    return window;
}

int main(int argc, char* args[])
{
    
    SDL_Window* window = InitializeApplication();
    
    if (!window)
    {
        printf("Failed to initalize application!");
        return 1;
    }

    SDL_Renderer* renderer = InitializeRenderer(window);

    SDL_Texture* bomb_texture = get_image("./resources/icon.png", renderer);

    SDL_Event e;
    bool is_running = true;
    size_t start_time = 0;

    while(is_running)
    {
        start_time = SDL_GetPerformanceCounter();

        //Poll all the events in the event queue
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                is_running = false;
                break;
            }
        }

        SDL_RenderClear(renderer);

        //Render texture to screen
        SDL_Rect image_rect { 320, 128, 64, 64 };
        SDL_RenderCopy(renderer, bomb_texture, NULL, &image_rect);

        SDL_RenderPresent(renderer);

        float elapsed = (SDL_GetPerformanceCounter() - start_time) / static_cast<float>(SDL_GetPerformanceFrequency());
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;

    IMG_Quit();
    SDL_Quit();
}


