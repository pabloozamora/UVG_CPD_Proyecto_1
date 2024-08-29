#include <SDL2/SDL.h>
#include <omp.h>

int x = 10;
int y = 10;

void render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderDrawPoint(renderer, x++, y++);
}

int main(int argv, char** args)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window* window = SDL_CreateWindow("hola", 100, 100, 800, 800, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event event;

    
    bool isRunning = true;
    while (isRunning) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                isRunning = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
        SDL_RenderClear(renderer);
        render(renderer);

        SDL_Delay(100);
        SDL_RenderPresent(renderer);
    }
}