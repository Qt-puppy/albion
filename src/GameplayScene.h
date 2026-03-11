#pragma once
#include "Scene.h"
#include "ECS.h"
#include "ResourceManager.h"
#include "MovementSystem.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include <vector>
#include <cstdlib>

class GameplayScene : public Scene {
private:
    Registry registry;
    ResourceManager assets;
    MovementSystem movementSystem;
    RenderSystem renderSystem;
    PhysicsSystem physicsSystem;
    
    SDL_Rect camera;
    std::vector<Entity> activeEntities; 

public:
    GameplayScene(SDL_Renderer* renderer) {
        assets.LoadTexture(renderer, "player_image", "../assets/player.png");
        assets.LoadTexture(renderer, "enemy_image", "../assets/enemy.png");

        camera = {0, 0, 800, 600}; // Fixed Arena Camera

        // 1. Spawn Player (Entity 0)
        Entity player = registry.CreateEntity();
        registry.AddTransform(player, 400.0f, 300.0f);
        registry.AddVelocity(player, 0.0f, 0.0f);
        registry.AddSprite(player, "player_image", 64, 64);
        registry.AddBoxCollider(player, 64, 64);
        activeEntities.push_back(player);

        // 2. Spawn Exactly ONE Enemy to start
        SpawnEnemy(); 
    }

    void SpawnEnemy() override {
        Entity newEnemy = registry.CreateEntity();
        
        float x = rand() % (800 - 64);
        float y = rand() % (600 - 64);
        
        // Random starting speed!
        float dx = (rand() % 300) - 150.0f;
        float dy = (rand() % 300) - 150.0f;
        if (dx == 0) dx = 150.0f;
        if (dy == 0) dy = 150.0f;

        registry.AddTransform(newEnemy, x, y);
        registry.AddVelocity(newEnemy, dx, dy);
        registry.AddSprite(newEnemy, "enemy_image", 64, 64);
        registry.AddBoxCollider(newEnemy, 64, 64);

        activeEntities.push_back(newEnemy);
    }

    void Update(InputManager& input, float deltaTime) override {
        // Update movement for ALL active entities
        for (Entity e : activeEntities) {
            movementSystem.Update(registry, input, e, deltaTime);
        }

        // Check collisions for ALL active entities
        physicsSystem.Update(registry, activeEntities, deltaTime);
    }

    void Render(SDL_Renderer* renderer) override {
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);
        
        for (Entity e : activeEntities) {
            renderSystem.Render(renderer, registry, e, assets, camera);
        }
    }
};