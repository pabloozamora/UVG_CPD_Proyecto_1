#pragma once
#include "color.h"
#include "fragment.h"
#include <array>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FRAMEBUFFER_WIDTH = 800;
const int FRAMEBUFFER_HEIGHT = 800;
const int FRAMEBUFFER_SIZE = FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT;

Color framebuffer[FRAMEBUFFER_SIZE];


Color clearColor = {0, 0, 0};

void clear() {
    std::srand(1);   
    for (int i = 0; i < FRAMEBUFFER_SIZE; i++) {
        int random = std::rand() % 1000;
        
        framebuffer[i] = clearColor;
    }
}

void point(Fragment f)
{
    int index = f.y * FRAMEBUFFER_WIDTH + f.x;

    // Verificar si el índice está dentro de la pantalla
    if (index >= 0 && index < FRAMEBUFFER_SIZE)
    {
        // Colocar color del pixel (x,y)
        framebuffer[index] = f.color;
    }
}