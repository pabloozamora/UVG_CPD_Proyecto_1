#include <SDL2/SDL.h>
#include <omp.h>
 #include <sstream>
#include "framebuffer.h"
#include "circle.h"
#include "pendulum.h"

Uint32 frameStart, frameTime;

void renderBuffer(SDL_Renderer *renderer)
{
    // Create a texture
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        FRAMEBUFFER_WIDTH,
        FRAMEBUFFER_HEIGHT);

    // Update the texture with the pixel data from the framebuffer
    SDL_UpdateTexture(
        texture,
        NULL,
        framebuffer,
        FRAMEBUFFER_WIDTH * sizeof(Color));

    // Copy the texture to the renderer
    SDL_RenderCopy(renderer, texture, NULL, NULL);

    // Destroy the texture
    SDL_DestroyTexture(texture);
}

int main(int argv, char** args)
{
    // iniciar ventana sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Universe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    SDL_Event event;

    int pivotX = 400;
    int pivotY = 300;

    // Crear un círculo en el pivote
    Circle pivotCircle(pivotX, pivotY, 10, {255, 255, 255});

    Pendulum pendulum(pivotX, pivotY, 300, PI / 2 + 10, 20, Color(255, 0, 0));
    Pendulum pendulum2(pivotX, pivotY, 350, PI / 2 + 10.5, 20, Color(0, 255, 0));
    Pendulum pendulum3(pivotX, pivotY, 200, PI / 2 - 1, 20, Color(0, 0, 255));



    float deltaTime = 1.5;

    while (running)
    {
        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
        
        }

        clear();





        // Dibujar el círculo
        pivotCircle.draw();


        //drawFilledRectangle(100, 100, 200, 200, 50, {255, 0, 0});

        pendulum.updatePhysics(deltaTime);
        pendulum.draw();

        pendulum2.updatePhysics(deltaTime);
        pendulum2.draw();

        pendulum3.updatePhysics(deltaTime);
        pendulum3.draw();

        renderBuffer(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime > 0)
        {
            std::ostringstream titleStream;
            titleStream << "FPS: " << 1000.0 / frameTime; // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}