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
          pivotX(pivotX), pivotY(pivotY), length(length), angle(initialAngle), angularVelocity(0), angularAcceleration(0),
          isRopeCut(false), vx(0), vy(0){
        
        updatePosition();
    }

    // Actualizar la física del péndulo
    void updatePhysics(float deltaTime) {
        if (isRopeCut) {
            // Si la cuerda está cortada, la bola sigue una trayectoria de proyectil
            updateProjectileMotion(deltaTime);
        } else {
            // Física del péndulo
            angularAcceleration = (-GRAVITY / length) * std::sin(angle);
            angularVelocity += angularAcceleration * deltaTime;
            angle += angularVelocity * deltaTime;
            updatePosition();
        }
    }

    void draw() {
        if (!isRopeCut) {
            // Dibujar cuerda solo si no está cortada
            drawFilledRectangle(pivotX, pivotY, x, y, ROPE_WIDTH, ROPE_COLOR);
        }
        // Dibujar circulo
        Circle::draw();
    }

    // Método para cortar la cuerda
    void cutRope() {
        isRopeCut = true;
        // Calcular las velocidades iniciales basadas en la velocidad angular en el momento del corte
        vx = length * angularVelocity * std::cos(angle);
        vy = -length * angularVelocity * std::sin(angle);
    }

private:
    int pivotX, pivotY;          // Posición del pivote del péndulo
    float length;                // Longitud del péndulo
    float angle;                 // Ángulo actual del péndulo
    float angularVelocity;       // Velocidad angular
    float angularAcceleration;   // Aceleración angular
    bool isRopeCut;              // Indica si la cuerda está cortada
    float vx, vy;                // Velocidades en x e y tras el corte de la cuerda

    // Actualizar la posición del círculo basado en el ángulo actual del péndulo
    void updatePosition() {
        x = pivotX + length * std::sin(angle);
        y = pivotY + length * std::cos(angle);
    }

    // Actualizar el movimiento del proyectil después de cortar la cuerda
    void updateProjectileMotion(float deltaTime) {
        // Ecuaciones del movimiento proyectil
        x += vx * deltaTime;
        vy += GRAVITY * deltaTime;
        y += vy * deltaTime;
    }
};
