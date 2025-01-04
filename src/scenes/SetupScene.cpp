#include <scenes/SetupScene.hpp>
#include <scenes/SceneManager.hpp>

SetupScene::SetupScene() {
    // This will create background object 
    // Load button png sprite sheet
    this->prepareResources();

    // Create exit button
    this->exitButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() - 70, (float)GetScreenHeight() - 70 }, 
        (Vector2)SMALL_BUTTON_SIZE, 
        (Vector2)SMALL_BUTTON_EXIT_OFFSET,
        BUTTON_TXT_KEY, 
        [this]() {
            // This is a function that will be called when the button is clicked
            // Get window handle
            HWND hwnd = (HWND)GetWindowHandle(); 
            // Post close message to winapi
            PostMessage(hwnd, WM_CLOSE, 0, 0); 
            // Close window handle;
            CloseHandle(hwnd);
        }
    );

    // Prepare title
    this->title = std::make_unique<Text>(
        "Rock Paper Scissors", 
        Vector2{ 0, 0 }, 
        Vector2{ (float)GetScreenWidth() / 2, 200 }, 
        MINECRAFT_FONT_KEY, 
        TITLE_SIZE, 
        TEXT_SPACING, 
        MY_ORANGE
    );
    this->title->tCenterOffset();

    // Nick Step 
    // -------------------------------------------------------------------------------------------
    // Then you can create button of confirmation
    this->confirmButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 200 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Confirm", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            if (this->nameInput->getText().length() < 3) return;

            this->nickGiven = true;
            this->nick = this->nameInput->getText();
        }
    );

    // nickname input
    this->nameInput = std::make_unique<InputField>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        Vector2{ 800, 100 } // Scale to size
    );
    // -------------------------------------------------------------------------------------------

    // Menu Step
    // -------------------------------------------------------------------------------------------
    // Then you can create button of host or client
    this->offlineButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 - 115 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Play offline", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerType = PlayerType::Offline;
            this->playerTypeGiven = true;
        }
    );

    this->hostButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Host Game", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerType = PlayerType::Host;
            this->playerTypeGiven = true;
        }
    );

    this->clientButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 115 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Join game", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            SCENE_MANAGER.pushScene(std::make_unique<ServerListScene>(this->nick));
        }
    );

    this->backButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 345 }, 
        (Vector2)MENU_BUTTON_SIZE,
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Back", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->nickGiven = false;
            this->nick = "";
            this->nameInput->clear();
        }
    );
    // -------------------------------------------------------------------------------------------
}

SetupScene::~SetupScene() {
    this->cleanUp();
}

void SetupScene::prepareResources() {
    // This will create background object if it doesn't exist
    M_BG;
    // Load button png sprite sheet
    RESOURCE_MANAGER.loadTexture(BUTTON_TXT_KEY, "assets/buttons.png");
    // Load font
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void SetupScene::update(float deltaTime) {
    // Set Mouse Cursor to Default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    M_BG.update(deltaTime);
    this->exitButton->update(deltaTime);

    if (!this->nickGiven) {
        // Update button
        this->confirmButton->update(deltaTime);
        this->nameInput->update(deltaTime);
    } else {
        // Update menu buttons
        this->offlineButton->update(deltaTime);
        this->hostButton->update(deltaTime);
        this->clientButton->update(deltaTime);
        this->backButton->update(deltaTime);
    }

    if (this->nickGiven && this->playerTypeGiven) {
        // Setup game
        GAME.setup(this->nick, this->playerType);

        // Go to proper scene
        switch( this->playerType ) {
            case PlayerType::Host:
                SCENE_MANAGER.pushScene( SceneType::HOST_LOBBY );
            break;

            case PlayerType::Offline:
                SCENE_MANAGER.pushScene( SceneType::LOADING_SCENE );
        }

        // If player bo back menu will be shown again
        this->playerTypeGiven = false;
    }
}

void SetupScene::draw() const {
    M_BG.draw();

    // Draw title
    this->title->draw();

    // First step of the setup scene
    if (!this->nickGiven) {
        // Draw text above input for information
        DrawTextEx(
            *RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY),
            "Enter your name: ",
            Vector2{ (float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 150 },
            50,
            1,
            MY_DARKGRAY
        );

        DrawTextEx(
            *RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY),
            "3 characters minimum",
            Vector2{ (float)GetScreenWidth() / 2 - 380, (float)GetScreenHeight() / 2 - 100 },
            50,
            1,
            MY_DARKGRAY
        );

        this->confirmButton->draw();
        this->nameInput->draw();
    } else {
        // Draw menu buttons
        this->offlineButton->draw();
        this->hostButton->draw();
        this->clientButton->draw();
        this->backButton->draw();
    }

    this->exitButton->draw();
}

void SetupScene::cleanUp() {
    // Clean up the input field
    RESOURCE_MANAGER.unloadTexture(BUTTON_TXT_KEY);
    RESOURCE_MANAGER.unloadFont(MINECRAFT_FONT_KEY);
}
