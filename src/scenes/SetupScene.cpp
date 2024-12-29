#include <scenes/SetupScene.hpp>
#include <scenes/SceneManager.hpp>

SetupScene::SetupScene() {
    // This will create background object 
    // Load button png sprite sheet
    this->prepareResources();

    // Prepare title
    this->titlePosition = Vector2{ (float)GetScreenWidth() / 2, 200 };
    Vector2 titleMeasurement = MeasureTextEx(*RESOURCE_MANAGER.getFont("minecraft-font"), this->titleText.c_str(), TITLE_SIZE, TEXT_SPACING);
    this->titleOffset = Vector2{ titleMeasurement.x / 2.f, titleMeasurement.y / 2.f };

    // Nick Step 
    // -------------------------------------------------------------------------------------------
    // Then you can create button of confirmation
    this->confirmButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 200 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        Vector2{0, 0},
        "Confirm", 
        "button", 
        "minecraft-font",
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
        Vector2{0, 0},
        "button", 
        "minecraft-font",
        Vector2{ 800, 100 }
    );
    // -------------------------------------------------------------------------------------------

    // Menu Step
    // -------------------------------------------------------------------------------------------
    // Then you can create button of host or client
    this->offlineButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 - 115 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        Vector2{0, 0},
        "Play offline", 
        "button", 
        "minecraft-font",
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerType = PlayerType::Offline;
            this->playerTypeGiven = true;
        }
    );

    this->hostButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        Vector2{0, 0},
        "Host Game", 
        "button", 
        "minecraft-font",
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerType = PlayerType::Host;
            this->playerTypeGiven = true;
        }
    );

    this->clientButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 115 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        Vector2{0, 0},
        "Join game", 
        "button", 
        "minecraft-font",
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerType = PlayerType::Client;
            this->playerTypeGiven = true;
        }
    );

    this->backButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 345 }, 
        (Vector2)MENU_BUTTON_SIZE,
        Vector2{0, 0},
        "Back", 
        "button", 
        "minecraft-font",
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
    RESOURCE_MANAGER.loadTexture("button", "assets/menuButton.png");
    // Load font
    RESOURCE_MANAGER.loadFont("minecraft-font", "assets/Minecraft.ttf");
}

void SetupScene::update(float deltaTime) {
    M_BG.update(deltaTime);
    // Set Mouse Cursor to Default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (!this->nickGiven) {
        // Update button
        this->confirmButton->update();
        this->nameInput->update(deltaTime);
    } else {
        // Update menu buttons
        this->offlineButton->update();
        this->hostButton->update();
        this->clientButton->update();
        this->backButton->update();
    }

    if (this->nickGiven && this->playerTypeGiven) {
        // Setup game
        GAME.setup(this->nick, this->playerType);

        // Go to proper scene
        switch( this->playerType ) {
            case PlayerType::Host:
                SCENE_MANAGER.pushScene( SceneType::HOST_LOBBY );
            break;
        }

        // If player bo back menu will be shown again
        this->playerTypeGiven = false;
    }
}

void SetupScene::draw() const {
    M_BG.draw();

    DrawTextPro(
        *RESOURCE_MANAGER.getFont("minecraft-font"),
        this->titleText.c_str(),
        this->titlePosition,
        this->titleOffset,
        0,
        TITLE_SIZE,
        TEXT_SPACING,
        MY_ORANGE
    );

    // First step of the setup scene
    if (!this->nickGiven) {
        // Draw text above input for information
        DrawTextEx(
            *RESOURCE_MANAGER.getFont("minecraft-font"),
            "Enter your name: ",
            Vector2{ (float)GetScreenWidth() / 2 - 400, (float)GetScreenHeight() / 2 - 150 },
            50,
            1,
            MY_DARKGRAY
        );

        DrawTextEx(
            *RESOURCE_MANAGER.getFont("minecraft-font"),
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
}

void SetupScene::cleanUp() {
    // Clean up the input field
    RESOURCE_MANAGER.unloadTexture("button");
    RESOURCE_MANAGER.unloadFont("minecraft-font");
}
