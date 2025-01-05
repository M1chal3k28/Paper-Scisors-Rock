#include <scenes/GameScene.hpp>
// #include "GameScene.hpp"

GameScene::~GameScene() {
    this->cleanUp();

    GAME.deinitialize();

    if (this->enemyChoiceThread.valid())
        this->enemyChoiceThread.get();
}

GameScene::GameScene() {
    this->prepareResources();

    // Receive Players
    this->player = GAME.movePlayer();
    this->enemy = GAME.moveEnemy();

    this->player->setPosition(Vector2{ 0.f + (Vector2)PLAYER_SIZE.x / 2.f, GetScreenHeight() / 2.f });
    this->enemy->setPosition(Vector2{ GetScreenWidth() - (Vector2)PLAYER_SIZE.x / 2.f, GetScreenHeight() / 2.f });

    // Initialize the game scene's properties
    // Buttons
    this->homeButton = std::make_unique<Button>(
        Vector2{ GetScreenWidth() - (Vector2)SMALL_BUTTON_SIZE.x / 2 - 10, (Vector2)SMALL_BUTTON_SIZE.y / 2 + 10 },
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_HOME_OFFSET,
        BUTTON_TXT_KEY,
        [this]() {
            SCENE_MANAGER.popScene();

            // Deinitialize game 
            GAME.deinitialize();
        }
    );

    this->exitButton = std::make_unique<Button>(
        Vector2{ GetScreenWidth() - (Vector2)SMALL_BUTTON_SIZE.x * 1.5f - 20, (Vector2)SMALL_BUTTON_SIZE.y / 2 + 10 },
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

    // Enemy stuff
    std::string enemyType = this->enemy->getName();
    Vector2 measure = MeasureTextEx(*RESOURCE_MANAGER.getFont(MINECRAFT_FONT_KEY), enemyType.c_str(), TITLE_SIZE / 2, TEXT_SPACING);
    this->enemyName = std::make_unique<Text>(
        enemyType,
        Vector2{0, 0},
        Vector2{(float)GetScreenWidth() - 10 - measure.x,  (float)GetScreenHeight() - 10 - measure.y},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );
    this->enemyReady = std::make_unique<Sprite>(
        READY_TEXTURE_KEY,
        1,
        Vector2 READY_SIZE,
        Vector2 { 0, 0 }
    );
    this->enemyReady->setPosition(Vector2{ this->enemyName->tGetPosition().x - (Vector2)READY_SIZE.x / 2.f - 10, this->enemyName->tGetPosition().y + (Vector2)READY_SIZE.y / 2 });

    this->enemyScore = std::make_unique<Text>(
        "",
        Vector2{0, 0},
        Vector2{ this->enemyReady->getPosition().x - (Vector2)READY_SIZE.x - 20, GetScreenHeight() - TITLE_SIZE / 4.f - 10},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );
    this->enemyScore->tCenterOffset();

    // Player stuff
    this->playerName = std::make_unique<Text>(
        "(You) " + this->player->getName(),
        Vector2{0, 0},
        Vector2{10, 10},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );

    this->playerReady = std::make_unique<Sprite>(
        READY_TEXTURE_KEY,
        1,
        Vector2 READY_SIZE,
        Vector2 { 0, 0 }
    );

    this->playerReady->setFrame(0);
    this->playerReady->setPosition(Vector2{ this->playerName->tGetPosition().x + this->playerName->tMeasure().x + (Vector2)READY_SIZE.x / 2.f + 10, (Vector2)READY_SIZE.y / 2 + 10 });

    this->playerScore = std::make_unique<Text>(
        "",
        Vector2{0, 0},
        Vector2{ this->playerReady->getPosition().x + (Vector2)READY_SIZE.x + 20, TITLE_SIZE / 4.f + 10},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );
    this->playerScore->tCenterOffset();

    // Choose buttons
    this->paperButton = std::make_unique<Button>(
        Vector2{(Vector2)SMALL_BUTTON_SIZE.x / 2.f + 10, GetScreenHeight() - (Vector2)SMALL_BUTTON_SIZE.y / 2.f - 10},
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_PAPER_OFFSET,
        BUTTON_TXT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerChoose(Choice::Paper);
        }
    );

    this->rockButton = std::make_unique<Button>(
        Vector2{(Vector2)SMALL_BUTTON_SIZE.x * 1.5f + 20, GetScreenHeight() - (Vector2)SMALL_BUTTON_SIZE.y / 2.f - 10},
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_ROCK_OFFSET,
        BUTTON_TXT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerChoose(Choice::Rock);
        }
    );

    this->scissorsButton = std::make_unique<Button>(
        Vector2{(Vector2)SMALL_BUTTON_SIZE.x * 2.5f + 30, GetScreenHeight() - (Vector2)SMALL_BUTTON_SIZE.y / 2.f - 10},
        (Vector2)SMALL_BUTTON_SIZE,
        (Vector2)SMALL_BUTTON_SCISSORS_OFFSET,
        BUTTON_TXT_KEY,
        [this]() {
            // This is a function that will be called when the button is clicked
            this->playerChoose(Choice::Scissors);
        }    
    );

    this->infoText = std::make_unique<Text>(
        "",
        Vector2{0, 0},
        Vector2{GetScreenWidth() / 2.f, GetScreenHeight() / 2.f},
        MINECRAFT_FONT_KEY,
        TITLE_SIZE / 2,
        TEXT_SPACING,
        MY_ORANGE
    );

    // Start new round !
    this->newRound();
}

void GameScene::playerChoose(Choice::Value choice) {
    this->rockButton->disable();
    this->paperButton->disable();
    this->scissorsButton->disable();

    this->player->choose(choice);
    this->playerReady->setFrame(this->player->hasChosen());
    if (this->player->hasChosen()) 
        this->infoText->tSetText("Waiting !");
}

void GameScene::enemyChoose() {
    try {
        this->enemy->choose();
    } catch (const std::exception& e) {
        std::cout << "Error reciving enemy's choice\n";
        // Handle exception
        WSACleanup();
        this->fatalError = true;
    }
    this->enemyReady->setFrame(this->enemy->hasChosen());
}

void GameScene::newRound() {
    this->player->resetChosen();
    this->enemy->resetChosen();

    this->playerReady->setFrame(0);
    this->enemyReady->setFrame(0);

    this->playerScore->tSetText(to_string(this->player->getScore()));
    this->enemyScore->tSetText(to_string(this->enemy->getScore()));

    this->rockButton->enable();
    this->paperButton->enable();
    this->scissorsButton->enable();

    this->resultsTimer = RESULTS_OFFSET;
    this->newRoundTimer = NEW_ROUND_OFFSET;
    this->showedResults = false;

    this->infoText->tSetText("Choose !");

    // this->enemyChoose();
    this->enemyChoiceThread = std::async(std::launch::async, &GameScene::enemyChoose, this);
}

void GameScene::prepareResources() {
    // Prepare the game scene's resources
    // Ready texture
    RESOURCE_MANAGER.loadTexture(READY_TEXTURE_KEY, "assets/player_ready.png");
    // Player sprite
    RESOURCE_MANAGER.loadTexture(PLAYER_TEXTURE_KEY, "assets/player_sprite.png");

    // Font 
    RESOURCE_MANAGER.loadFont(MINECRAFT_FONT_KEY, "assets/Minecraft.ttf");
}

void GameScene::update(float deltaTime) {
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    // Update the game scene's properties
    M_BG.update(deltaTime);

    this->exitButton->update(deltaTime);
    this->playerReady->update(deltaTime);
    this->enemyReady->update(deltaTime);
    this->player->update(deltaTime);
    this->enemy->update(deltaTime);
    this->paperButton->update(deltaTime);
    this->rockButton->update(deltaTime);
    this->scissorsButton->update(deltaTime);

    if (this->player->hasChosen() && this->enemy->hasChosen()) {
        // Show results with 3 seconds delay
        if (!this->showedResults) {
            this->resultsTimer -= deltaTime;
            this->infoText->tSetText(to_string(int(this->resultsTimer)));
        }

        if (this->resultsTimer <= 0) {
            if (!this->showedResults) {
                if (this->enemyChoiceThread.valid())
                    this->enemyChoiceThread.get();
                
                this->player->showResult();
                this->enemy->showResult();
            
                int result = GAME.checkWin(this->player->getChoice(), this->enemy->getChoice());
                if (result == 0) { this->infoText->tSetText("Tie !"); }
                else if (result == 1) {
                    this->player->addScore();
                    this->infoText->tSetText("You Win!");
                } else {
                    this->enemy->addScore();
                    this->infoText->tSetText("You Lose!");
                }

                this->showedResults = true;
            }

            // Wait 2 seconds before starting a new round
            this->newRoundTimer -= deltaTime;
            if (this->newRoundTimer <= 0.f)
                this->newRound();
        }
    }

    // Home button must be last
    if( this->homeButton->bUpdate(deltaTime) ) // If back button is clicked skip fatal error cause scene is deleted anyway
        return;

    if (this->fatalError) {
        SCENE_MANAGER.popScene();
    }
}

void GameScene::draw() const {
    M_BG.draw();

    this->playerName->draw();
    this->playerScore->draw(); 
    this->enemyName->draw();
    this->homeButton->draw();
    this->exitButton->draw();
    this->playerReady->draw();
    this->enemyReady->draw();
    this->enemyScore->draw();
    this->enemy->draw();
    this->infoText->draw();
    this->player->draw();
    this->paperButton->draw();
    this->rockButton->draw();
    this->scissorsButton->draw();
}

void GameScene::cleanUp() {}