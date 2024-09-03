#pragma once
#include <cmath>
#include <algorithm>
#include "color.h"
#include "fragment.h"
#include "framebuffer.h"

// verificar si un punto está dentro de un rectángulo
bool isInsideRectangle(int x, int y, int x0, int y0, int x1, int y1, int x2, int y2, int x3, int y3) {
    auto sign = [](int x1, int y1, int x2, int y2, int x3, int y3) {
        return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
    };
    
    bool b1, b2, b3, b4;
    
    b1 = sign(x, y, x0, y0, x1, y1) < 0.0f;
    b2 = sign(x, y, x1, y1, x2, y2) < 0.0f;
    b3 = sign(x, y, x2, y2, x3, y3) < 0.0f;
    b4 = sign(x, y, x3, y3, x0, y0) < 0.0f;

    return (b1 == b2) && (b2 == b3) && (b3 == b4);
}

// establecer un píxel en el framebuffer
void setPixel(int px, int py, const Color& color) {
    if (px < 0 || px >= FRAMEBUFFER_WIDTH || py < 0 || py >= FRAMEBUFFER_HEIGHT) {
        return; // No dibujar fuera de los límites
    }
    Fragment f = {static_cast<uint16_t>(px), static_cast<uint16_t>(py), 0, color};
    point(f);
}

// dibujar un rectángulo relleno
void drawFilledRectangle(int x1, int y1, int x2, int y2, int width, const Color& color) {
    // Calcular la distancia entre los dos puntos
    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = std::sqrt(dx * dx + dy * dy);

    // Normalizar el vector director del lado largo del rectángulo
    float nx = dx / length;
    float ny = dy / length;

    // Calcular el vector perpendicular para el ancho del rectángulo
    float px = -ny * (width / 2);
    float py = nx * (width / 2);

    // Calcular los cuatro vértices del rectángulo
    int x0 = x1 + px; // Primer vértice
    int y0 = y1 + py;
    int x1n = x2 + px; // Segundo vértice
    int y1n = y2 + py;
    int x2n = x2 - px; // Tercer vértice
    int y2n = y2 - py;
    int x3 = x1 - px; // Cuarto vértice
    int y3 = y1 - py;

    // Usar std::min y std::max con inicializadores de lista
    int minY = std::min({y0, y1n, y2n, y3});
    int maxY = std::max({y0, y1n, y2n, y3});
    int minX = std::min({x0, x1n, x2n, x3});
    int maxX = std::max({x0, x1n, x2n, x3});

    // Dibujar líneas horizontales para rellenar el rectángulo
    for (int y = minY; y <= maxY; y++) {
        if (y < 0 || y >= FRAMEBUFFER_HEIGHT) continue; // Ignorar si y está fuera de los límites
        for (int x = minX; x <= maxX; x++) {
            if (x < 0 || x >= FRAMEBUFFER_WIDTH) continue; // Ignorar si x está fuera de los límites
            if (isInsideRectangle(x, y, x0, y0, x1n, y1n, x2n, y2n, x3, y3)) {
                setPixel(x, y, color);
            }
        }
    }
}
