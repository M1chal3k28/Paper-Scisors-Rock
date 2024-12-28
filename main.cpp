
#include <Config.hpp>
#include <scenes/Scenes.hpp>

int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
        // Init random number generator
        srand(time(NULL));
        //----------------------------------------------------------------------------------

    // Set config flag to enable V-Sync
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Rock-Paper-Scissors");
    // Set full screen
    SetWindowState(FLAG_FULLSCREEN_MODE);

    // Initialize scenes
    SCENE_MANAGER.pushScene( SceneType::INTRO );
    //--------------------------------------------------------------------------------------

    // Main game loop
    while ( !WindowShouldClose() )    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // Update currentScene
        SCENE_MANAGER.update( GetFrameTime() );
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(RAYWHITE);
            // Draw current scene
            SCENE_MANAGER.draw();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
    return 0;
}
