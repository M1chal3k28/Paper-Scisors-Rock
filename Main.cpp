#include <Game.hpp>

#include <config.h>

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(800, 600, "Rock-Paper-Scissors");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second

    std::thread gameThread([&]() { 
        Game::getInstance().setup(); // setup game
        Game::getInstance().run(); // run game
    });   
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    gameThread.join(); // wait for game thread to finish
    //--------------------------------------------------------------------------------------
    return 0;
}