#include <scenes/ServerListScene.hpp>
#include <scenes/SceneManager.hpp>

ServerListScene::ServerListScene(std::string nickName) : currentPage(0), clientName(nickName) {
    this->prepareResources();

    // Create info
    this->info = std::make_unique<Text>( "Searching", Vector2{0, 0}, Vector2{(float)GetScreenWidth() / 2, 200}, MINECRAFT_FONT_KEY, TITLE_SIZE, TEXT_SPACING, MY_ORANGE);
    this->info->tCenterOffset();

    // Create back button
    this->backButton = std::make_unique<TextButton>(
        Vector2{ (float)GetScreenWidth() / 2 - (Vector2)SMALL_BUTTON_SIZE.x / 2 - 10, (float)GetScreenHeight() / 2 + 395 }, 
        (Vector2)MENU_BUTTON_SIZE, 
        (Vector2)MENU_BUTTON_SHEET_OFFSET,
        "Back", 
        BUTTON_TXT_KEY, 
        MINECRAFT_FONT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            // Go back to main menu scene
            SCENE_MANAGER.popScene();
        }
    );

    this->reloadButton = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2 + (Vector2)MENU_BUTTON_SIZE.x / 2 + 10, (float)GetScreenHeight() / 2 + 395},
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_RELOAD_OFFSET,
        BUTTON_TXT_KEY,
        [this](){
            // This is a function that will be called when the button is clicked
            // Reload servers
            if (!this->isFinished)
                return;

            // Start looking for a servers
            // Set finished to false
            this->isFinished = false;
            this->getServersThread = std::async(std::launch::async, &ServerListScene::getAvailableServers, this);
            this->currentPage = 0;
            this->pageText->tSetText(std::to_string(this->currentPage + 1));
        }
    );

    this->leftArrow = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2 - 50, (float)GetScreenHeight() / 2 + 315},
        (Vector2)TINY_BUTTON_SIZE,
        (Vector2)TINY_BUTTON_LEFTARROW_OFFSET,
        BUTTON_TXT_KEY,
        [this](){
            // This is a function that will be called when the button is clicked
            if(this->currentPage <= 0)
                return;

            this->currentPage--;
            this->pageText->tSetText(std::to_string(this->currentPage + 1));
        }
    );

    this->rightArrow = std::make_unique<Button>(
        Vector2{ (float)GetScreenWidth() / 2 + 50, (float)GetScreenHeight() / 2 + 315},
        (Vector2)TINY_BUTTON_SIZE,
        (Vector2)TINY_BUTTON_RIGHTARROW_OFFSET,
        BUTTON_TXT_KEY,
        [this](){
            // This is a function that will be called when the button is clicked
            if(this->pages.size() <= this->currentPage + 1)
                return;

            this->currentPage++;
            this->pageText->tSetText(std::to_string(this->currentPage + 1));
        }
    );

    // page text
    this->pageText = std::make_unique<Text>( 
        "1", 
        Vector2{0, 0}, 
        Vector2{(float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 + 315}, 
        MINECRAFT_FONT_KEY, 
        TEXT_SIZE, 
        TEXT_SPACING, 
        MY_ORANGE
    );
    this->pageText->tCenterOffset();

    // Start looking for a servers
    // Set finished to false
    this->isFinished = false;
    this->getServersThread = std::async(std::launch::async, &ServerListScene::getAvailableServers, this);
}

ServerListScene::~ServerListScene() {
    this->cleanUp();
}

void ServerListScene::getAvailableServers() {
    std::lock_guard<std::mutex> lock(this->getServersMutex);

    this->info->tSetText("Searching");
    this->pages.clear();

    // Start looking for a servers
    this->broadcastSocket.searchOnce();
    this->discoveredServers = this->broadcastSocket.getResults();

    for (size_t i = 1; i <= 20; i++) {
        this->discoveredServers.emplace_back(std::make_pair("Server " + std::to_string(i), sockaddr_in{}));
    }

    // Create server buttons
    this->pages.emplace_back(std::vector<std::unique_ptr<TextButton>>());

    // padding
    static int padding = 2;

    // Create server buttons
        int col = 0;
        int maxCol = (int)GetScreenWidth() / (Vector2)MENU_BUTTON_SIZE.x;
        int row = 0;
        int maxRow = ((int)GetScreenHeight() - 500) / (Vector2)MENU_BUTTON_SIZE.y;
        int page = 0;
    // Starting pos
        Vector2 startingPos = Vector2{ (float) (Vector2)MENU_BUTTON_SIZE.x / 2 + (GetScreenWidth() - maxCol * (Vector2)MENU_BUTTON_SIZE.x) / 2 - padding * maxCol, (float)300 + (Vector2)MENU_BUTTON_SIZE.y / 2 };
    // Create button for each server
    for (auto& server : this->discoveredServers) {
        // Create button
        // Calc position
        if (col >= maxCol) {
            col = 0;
            row++;
            if (row >= maxRow) {
                row = 0;
                page++;
                this->pages.emplace_back(std::vector<std::unique_ptr<TextButton>>());
            }
        }

        // Create button
        // And add to proper page
        this->pages[page].emplace_back(
            std::make_unique<TextButton>(
                Vector2{ startingPos.x + col * (Vector2)MENU_BUTTON_SIZE.x + (padding * col), startingPos.y + row * (Vector2)MENU_BUTTON_SIZE.y + (padding * row) },
                (Vector2)MENU_BUTTON_SIZE,
                (Vector2)MENU_BUTTON_SHEET_OFFSET,
                server.first,
                BUTTON_TXT_KEY,
                MINECRAFT_FONT_KEY,
                [this, server]() {
                    // This is a function that will be called when the button is clicked
                    GAME.setup(this->clientName, PlayerType::Client, server.second);
                    this->isServerSelected = true;
                }
            )
        );
        
        col++;
    }

    // Text 
    this->info->tSetText("Found " + std::to_string(this->discoveredServers.size()) + " servers");

    this->isFinished = true;
}

void ServerListScene::prepareResources() {
    // Init background if not done already
    M_BG;
    
    // Load button png sprite sheet
    RESOURCE_MANAGER.loadTexture(BUTTON_TXT_KEY, "assets/buttons.png");
    // Load font
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void ServerListScene::update(float deltaTime) {
    // Join thread if finished
    if (this->isFinished) {
        if(this->getServersThread.valid())
            this->getServersThread.get();
    }

    // Set cursor to default
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    if (this->isFinished)
        // Update server buttons
        for (auto& page : pages)
            for (auto& server : page) 
                server->update(deltaTime);

    // Update reload button
    if(this->reloadButton)
        this->reloadButton->update(deltaTime);

    // Update left arrow
    if (this->leftArrow)
        this->leftArrow->update(deltaTime);

    // Update right arrow
    if (this->rightArrow)
        this->rightArrow->update(deltaTime);

    M_BG.update(deltaTime);

    if (this->isServerSelected) {   
        // Then deinitialize
        SCENE_MANAGER.popScene();
        SCENE_MANAGER.pushScene(SceneType::LOADING_SCENE);
        return;
    }

    // Update back button
    if(this->backButton)
        this->backButton->update(deltaTime);
}

void ServerListScene::draw() const {
    M_BG.draw();

    // Draw back button
    this->backButton->draw();

    // Draw reload button
    this->reloadButton->draw();

    // Draw left arrow
    if (this->leftArrow)
        this->leftArrow->draw();

    // Draw right arrow
    if (this->rightArrow)
        this->rightArrow->draw();

    // Draw title
    if (this->info)
        this->info->draw();

    // Draw page text
    if (this->pageText)
        this->pageText->draw();

    if (this->isFinished)
        // Draw servers
        if (this->currentPage < this->pages.size())
            for (auto& server : this->pages[this->currentPage])
                server->draw();
}

void ServerListScene::cleanUp() {
    // Join thread
    if (this->getServersThread.valid())
        this->getServersThread.get();
}
