#include "raylib.h"
#include "Vec2.h"
#include "entities.h"


int main() {
    // 1. Initialize a window (800x600 resolution)
    InitWindow(800, 600, "Custom C++ Physics Engine");
    SetTargetFPS(60);


    
    //test the rendering and SIMD math
    
    ObjectManager manager;
    std::vector<int> found;
    found.reserve(2000);
    for (int i = 0; i < 10001; i++){
        manager.AddObject(
            (float)(rand() % 800), // x
            (float)(rand() % 600), // y
            (float)(rand() % 200 - 100) / 1.0f, // vx
            (float)(rand() % 200 - 100) / 1.0f, // vy
            0.0f, // ax
            100.0f, // ay
            1.0f, // mass
            1.0f, // radius
            (float)(rand() % 100) / 100.0f  // restitution
        );
    }

    // 4. The main engine loop
    while (!WindowShouldClose()) {
        
        // Update the physics engine (this will be where you call your SIMD-optimized update function)
        manager.Update();
        manager.ApplyConstraints(800.0f, 600.0f);
    

        BeginDrawing();
        ClearBackground(BLACK);

        // Draw the object 
        for (int i = 0; i < manager.activeObjects; i++) {
            Vec2 position(manager.posX[i], manager.posY[i]);
            DrawCircle((int)position.x, (int)position.y, manager.radii[i], RED);
        }

        
        //DrawText("SIMD Math + Raylib Rendering Working!", 10, 10, 20, YELLOW);
        DrawFPS(10, 30);
        EndDrawing();
    }

    // 5. Cleanup
    CloseWindow();
    return 0;
}