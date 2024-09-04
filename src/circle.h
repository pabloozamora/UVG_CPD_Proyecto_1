#pragma once
#include "color.h"
#include "fragment.h"
#include "framebuffer.h"

class Circle {
public:

    Circle(int x, int y, int z, int radius, const Color& color)
        : x(x), y(y), z(z), radius(radius), color(color) {}

    // Método para dibujar el círculo relleno en el framebuffer
    void draw() {
        // Algoritmo del círculo de Bresenham
        int x0 = x;
        int y0 = y;
        int r = radius;
        
        int dx = 0;
        int dy = r;
        int d = 1 - r;

        fillCircle(x0, y0, dx, dy);

        // Dibujar el círculo utilizando aritmética entera para la eficiencia
        while (dx < dy) {
            if (d < 0) {
                d += 2 * dx + 3;
            } else {
                d += 2 * (dx - dy) + 5;
                dy--;
            }
            dx++;

            fillCircle(x0, y0, dx, dy);
        }
    }

    protected:
    int x, y, z;        // Centro del círculo
    int radius;
    Color color;

    // dibujar y rellenar el círculo
    void fillCircle(int cx, int cy, int x, int y) {
        drawHorizontalLine(cx - x, cx + x, cy + y);
        drawHorizontalLine(cx - x, cx + x, cy - y);
        drawHorizontalLine(cx - y, cx + y, cy + x);
        drawHorizontalLine(cx - y, cx + y, cy - x);
    }

    // dibujar una línea horizontal entre dos puntos en el framebuffer
    void drawHorizontalLine(int x1, int x2, int y) {
        if (y < 0 || y >= FRAMEBUFFER_HEIGHT) return; // Verifica si 'y' está dentro del rango

        // Ajusta los límites para que se mantengan dentro del framebuffer
        x1 = std::max(0, x1);
        x2 = std::min(FRAMEBUFFER_WIDTH - 1, x2);

        for (int x = x1; x <= x2; ++x) {
            setPixel(x, y);
        }
    }

    // establecer un píxel en el framebuffer
    void setPixel(int px, int py) {
        Fragment f = {static_cast<uint16_t>(px), static_cast<uint16_t>(py), z, color};
        point(f);
    }
};
