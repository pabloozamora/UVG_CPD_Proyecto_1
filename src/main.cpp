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
#include <cstdlib>
#include <cerrno> 
#include <cstring> 
#include <optional>
#include <climits>


Uint32 frameStart, frameTime;

// Constantes por defecto
int N = 100;  // Cantidad de péndulos
float RADIUS = 10;  // Radio de los círculos de los péndulos

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

// Variables para el cálculo del tiempo promedio
float totalRenderTime = 0;  // Tiempo total de renderizado
int frameCount = 0;         // Número de frames procesados

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
        int zPos = N - i + 1; // Primeros elementos más adelante siempre
        pendulums.emplace_back(pivotX, pivotY, zPos, length, angle, RADIUS, color);

        currentLengthCount++;  // Incrementar el contador de péndulos con la misma longitud
    }

}

// Conversión cadena a entero. Si es exitoso, devuelve el entero. Si no, devuelve un valor nulo.
std::optional<int> argToInt(const char* arg, int index) {
    char* end;
    errno = 0;

    // Convierte la cadena a número entero
    long number = std::strtol(arg, &end, 10);

    // Verifica si ocurrió un error durante la conversión
    if (errno != 0 || *end != '\0' || number < INT_MIN || number > INT_MAX) {
        SDL_Log("Argumento inválido, se espera un número entero en la posición %d", index);
        return std::nullopt;
    }

    return static_cast<int>(number);
}

// Conversión cadena a float. Si es exitoso, devuelve el entero. Si no, devuelve un valor nulo.
std::optional<float> argToFloat(const char* arg, int index) {
    char* end;
    errno = 0;

    float number = std::strtof(arg, &end);

    if (errno != 0 || *end != '\0') {
        SDL_Log("Argumento inválido, se espera un número flotante en la posición %d", index);
        return std::nullopt;
    }

    return number;
}

int main(int argc, char** argv)
{

    // Comprobar si se pasaron argumentos para constantes
    if (argc > 1) {
        std::optional<int>  res = argToInt(argv[1], 1); // Convertir el primer argumento a entero para N
        if (!res) return -1;
        N = *res;
    }
    if (argc > 2) {

        std::optional<float>  res = argToFloat(argv[2], 2);
        if (!res) return -1;

        if (*res <= 0){
            SDL_Log("El radio debe ser mayor a 0");
            return -1;
        }
        RADIUS = *res; // Convertir el segundo argumento a float para RADIUS
    }
    if (argc > 3) {
        std::optional<int>  res = argToInt(argv[3], 3); // Convertir el tercer argumento a entero para CUT_PROB_INV
        if (!res) return -1;
        CUT_PROB_INV = *res;
        if (CUT_PROB_INV <= 0){
            SDL_Log("El inverso de la probabilidad de corte debe ser mayor a 0.");
            return -1;
        }
    }
    
    if (argc > 4) {
        std::optional<float>  res = argToFloat(argv[4], 4); // cuarto argumento a initialLength
        if (!res) return -1;

        INITIAL_LENGTH = *res;
        if (INITIAL_LENGTH <= 0){
            SDL_Log("La longitud inicial debe ser mayor a 0");
            return -1;
        }
    }

    if (argc > 5) {
        std::optional<int>  res = argToInt(argv[5], 5); // quinto argumento a max_same_length
        if (!res) return -1;

        MAX_SAME_LENGTH = *res;
        if (MAX_SAME_LENGTH < 0){
            SDL_Log("La cantidad máxima de péndulos con la misma longitud debe ser mayor o igual a 0.");
            return -1;
        }
    }

    if (argc > 6) {
        std::optional<float>  res = argToFloat(argv[6], 6); // sexto argumento a length_increment
        if (!res) return -1;

        LENGTH_INCREMENT = *res;
        if (LENGTH_INCREMENT <= 0){
            SDL_Log("El incremento de longitud debe ser mayor a 0");
            return -1;
        }
    }

    // iniciar ventana sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow("Universe", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    bool running = true;
    SDL_Event event;

    // Crear un círculo en el pivote
    Circle pivotCircle(pivotX, pivotY, 500, 10, {255, 255, 255});

    float deltaTime = 0.2;

    // Generador de nums aleatorios con distribución uniforme
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, CUT_PROB_INV);

    // Variables para medir el tiempo de generación
    Uint32 generationStart, generationTime;
    generationStart = SDL_GetTicks(); // Inicia la medición del tiempo de generación

    // Generación de los péndulos
    createPendulums();

    generationTime = SDL_GetTicks() - generationStart; // Tiempo que tarda en generar los N péndulos
    SDL_Log("Tiempo de generación para N = %d: %u ms", N, generationTime);

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

        // Pasar el framebuffer a la ventana
        SDL_RenderPresent(renderer);

        // Limitar la velocidad de fotogramas
        SDL_Delay(100 / 60);

        // Medir el tiempo de renderizado de los N elementos en el frame actual
        frameTime = SDL_GetTicks() - frameStart;

        // Sumar el tiempo del frame al total
        totalRenderTime += frameTime;
        frameCount++;  // Incrementar el número de frames procesados

        if (frameTime > 0)
        {
            std::ostringstream titleStream;
            titleStream << "FPS: " << 1000.0 / frameTime; // Milisegundos a segundos
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    // Al final de la ejecución, calcular el promedio de tiempo de renderizado
    float averageRenderTime = totalRenderTime / frameCount;
    SDL_Log("Promedio de tiempo de renderizado por frame: %f ms", averageRenderTime);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}