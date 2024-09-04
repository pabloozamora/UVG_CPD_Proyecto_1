#include <SDL2/SDL.h>
#include <omp.h>
#include <sstream>
#include "framebuffer.h"
#include "circle.h"
#include "pendulum.h"
#include <vector>
#include <SDL2/SDL_log.h>
#include <map>
#include <random>
#include <iostream>


Uint32 frameStart, frameTime;

// Constantes
int N = 100;  // Cantidad de péndulos
float RADIUS = 10;  // Radio de los péndulos

// Parámetros de los péndulos
int MAX_SAME_LENGTH = 3;  // Máxima cantidad de péndulos con la misma longitud
float INITIAL_LENGTH = 150;  // Longitud inicial de los péndulos
float LENGTH_INCREMENT = 10;  // Incremento de longitud
int CUT_PROB_INV = 1000;  // Probabilidad inversa de cortar la cuerda

// Posición del pivote
int pivotX = WINDOW_WIDTH / 2;
int pivotY = WINDOW_HEIGHT / 4;

// Crear un mapa para almacenar los colores asociados a cada longitud
std::map<float, Color> lengthColorMap;

// Crear un vector para almacenar los péndulos
std::vector<Pendulum> pendulums;

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

void createPendulums() {

    int currentLengthCount = 0;
    
    float length = INITIAL_LENGTH;

    for (int i = 0; i < N; ++i) {
        if (currentLengthCount >= MAX_SAME_LENGTH) {
            length += LENGTH_INCREMENT;  // Incrementar la longitud después de maxSameLength péndulos
            currentLengthCount = 0;  // Reiniciar el contador
        }

        // Verificar si ya existe un color asignado para esta longitud
        if (lengthColorMap.find(length) == lengthColorMap.end()) {
            // Si no existe, asignar un nuevo color y guardarlo en el mapa
            Color newColor = {static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256), static_cast<Uint8>(std::rand() % 256)};
            lengthColorMap[length] = newColor;
        }

        // Usar el color asociado a la longitud actual
        Color color = lengthColorMap[length];

        float angle = PI / 2 + (currentLengthCount * 10 * PI) / 180;  // Diferentes ángulos para cada péndulo (en radianes)
        pendulums.emplace_back(pivotX, pivotY, length, angle, RADIUS, color);

        currentLengthCount++;  // Incrementar el contador de péndulos con la misma longitud
    }

}

int main(int argv, char** args)
{
    // iniciar ventana sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Universe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    SDL_Event event;

    // Crear un círculo en el pivote
    Circle pivotCircle(pivotX, pivotY, 10, {255, 255, 255});

    float deltaTime = 0.2;

    // Generador de nums aleatorios con distribución uniforme
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, CUT_PROB_INV);

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

        // Dibujar el pivote
        pivotCircle.draw();

        // Si ya no hay péndulos en el vector, volver a crearlos

        if (pendulums.empty()) {
            createPendulums();
        }

        //drawFilledRectangle(100, 100, 200, 200, 50, {255, 0, 0});

        // Eliminar péndulos que ya no están en pantalla (NO PARALELIZAR)
        pendulums.erase(
            std::remove_if(pendulums.begin(), pendulums.end(),
                [&](Pendulum& pendulum) {
                    return pendulum.isOffScreen();
                }),
            pendulums.end());

        // Actualizar la física de los péndulos y dibujarlos
        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < pendulums.size(); i++) {

            int random_number = distrib(gen);
            if(random_number == 1) {
                pendulums[i].cutRope();
            }

            pendulums[i].updatePhysics(deltaTime);
            pendulums[i].draw();
        }

        renderBuffer(renderer);

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(100 / 60);

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