#pragma once
#include "Scene.h"

class SceneManager {
private:
    Scene* currentScene;

public:
    SceneManager() : currentScene(nullptr) {}

    ~SceneManager() {
        if (currentScene != nullptr) {
            delete currentScene;
        }
    }
    // Passes the command from ImGui down to the active scene
    // Replace SpawnSwarm with this:
    void SpawnEnemy() {
        if (currentScene != nullptr) {
            currentScene->SpawnEnemy();
        }
    }

    void ChangeScene(Scene* newScene) {
        if (currentScene != nullptr) {
            delete currentScene;
        }
        currentScene = newScene;
    }

    // We added float deltaTime here and passed it to the scene!
    void Update(InputManager& input, float deltaTime) {
        if (currentScene != nullptr) {
            currentScene->Update(input, deltaTime);
        }
    }

    void Render(SDL_Renderer* renderer) {
        if (currentScene != nullptr) {
            currentScene->Render(renderer);
        }
    }
};