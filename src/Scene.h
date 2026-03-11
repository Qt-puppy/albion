#pragma once
#include <SDL2/SDL.h>
#include "InputManager.h"

class Scene {
public:
    virtual ~Scene() = default;

// We added float deltaTime here!
    virtual void SpawnEnemy() {}
    virtual void Update(InputManager& input, float deltaTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
};