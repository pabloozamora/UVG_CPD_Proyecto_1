#pragma once
#include "circle.h"
#include <cmath>
#include "drawRectangle.h"

// Definiciones de constantes
const float GRAVITY = 9.8f;
const float PI = 3.14159265f;
const Color ROPE_COLOR = {255, 255, 255};
const int ROPE_WIDTH = 2;

class Pendulum : public Circle {
public:
    Pendulum(int pivotX, int pivotY, int length, float initialAngle, int radius, const Color& color)
        : Circle(pivotX, pivotY + length, radius, color), 
          pivotX(pivotX), pivotY(pivotY), length(length), angle(initialAngle), angularVelocity(0), angularAcceleration(0) {
        
        updatePosition(); // Inicializa la posición del círculo
    }

    // Método para actualizar la física del péndulo
    void updatePhysics(float deltaTime) {
        // Calcula la aceleración angular basada en la gravedad y el ángulo actual
        angularAcceleration = (-GRAVITY / length) * std::sin(angle);

        // Actualiza la velocidad angular y el ángulo
        angularVelocity += angularAcceleration * deltaTime;
        angle += angularVelocity * deltaTime;

        // Fricción para simular pérdida de energía
        //angularVelocity *= 0.99;

        // Actualiza la posición del círculo basado en el ángulo actual
        updatePosition();
    }

        void draw() {
            
            // Dibujar rectángulo desde el circulo hasta el pivote (cuerda)
            drawFilledRectangle(pivotX, pivotY, x, y, ROPE_WIDTH, ROPE_COLOR);

            // Dibujar circulo
            Circle::draw();

        }

    private:
        int pivotX, pivotY;          // Posición del pivote del péndulo
    float length;                // Longitud del péndulo
    float angle;                 // Ángulo actual del péndulo
    float angularVelocity;       // Velocidad angular
    float angularAcceleration;   // Aceleración angular

    // Actualizar la posición del círculo basado en el ángulo actual del péndulo
    void updatePosition() {
        x = pivotX + length * std::sin(angle);
        y = pivotY + length * std::cos(angle);
    }
};