#pragma once

#include <scenes/Scene.hpp>
#include <ResourceManager.hpp>
#include <Background.hpp>
#include <Button.hpp>
#include <InputField.hpp>

#include <Game.hpp>

class SetupScene : public Scene {
    std::string nick;
    PlayerType::Value playerType;
    std::unique_ptr<Button> confirmButton = nullptr;
    std::unique_ptr<InputField> nameInput = nullptr;
public:
    SetupScene();
    ~SetupScene();

    void prepareResources() override;
    void update(float deltaTime) override;
    void draw() const override;
    void cleanUp() override;
};