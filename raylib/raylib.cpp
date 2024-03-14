#include <iostream>
#include "raylib.h"
#include "Shape.hpp"
#include "GameObject.hpp"
#include "Jugador.hpp"

int main() {
    float width = 800;
    float height = 800;
    // Initialization
    InitWindow(width, height, "Drawing Shapes");
    SetTargetFPS(60);

    // Create an instance of GameObject
    GameObject gameObject;
    Jugador player;

    // Initial position of the triangle
    int xa = width/2 -20, ya = height/2;
    int xb = width/2 +20, yb = height/2;
    int xc = width/2, yc = height/2 -20;



    // Main loop
    while (!WindowShouldClose()) {
        // Handle input


        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        gameObject.RotateMatrix(-1);
        // Apply translation to the triangle
         // Clear previous points

        player.Update();
        // Draw the triangle
        player.Draw(xa, ya, xb, yb, xc, yc);

        EndDrawing();
        gameObject.linePoints.clear();
    }

    CloseWindow();
    return 0;
}