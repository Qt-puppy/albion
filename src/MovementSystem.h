#pragma once
#include "ECS.h"
#include "InputManager.h"

class MovementSystem {
public:
    void Update(Registry& registry, InputManager& input, Entity entity, float deltaTime) {
        
        TransformComponent& transform = registry.GetTransform(entity);
        VelocityComponent& velocity = registry.GetVelocity(entity);

        // Player Controls (Entity 0)
        if (entity == 0) {
            bool isMoving = false;
            float speed = 250.0f; 
            
            if (input.IsKeyPressed(SDL_SCANCODE_W)) { velocity.dy = -speed; isMoving = true; }
            if (input.IsKeyPressed(SDL_SCANCODE_S)) { velocity.dy = speed; isMoving = true; }
            if (input.IsKeyPressed(SDL_SCANCODE_A)) { velocity.dx = -speed; isMoving = true; }
            if (input.IsKeyPressed(SDL_SCANCODE_D)) { velocity.dx = speed; isMoving = true; }  

            // If player isn't pressing keys, apply friction so they slow down but can still be bounced!
            if (!isMoving) {
                velocity.dx *= 0.95f; 
                velocity.dy *= 0.95f;
            }
        }

        // Apply velocity to ALL entities
        transform.x += velocity.dx * deltaTime;
        transform.y += velocity.dy * deltaTime;

        // --- WALL BOUNCE FOR EVERYONE ---
        if (transform.x <= 0) {
            transform.x = 1.0f;
            velocity.dx *= -1.0f;
        } else if (transform.x >= 800 - 64) {
            transform.x = 800 - 65.0f;
            velocity.dx *= -1.0f;
        }

        if (transform.y <= 0) {
            transform.y = 1.0f;
            velocity.dy *= -1.0f;
        } else if (transform.y >= 600 - 64) {
            transform.y = 600 - 65.0f;
            velocity.dy *= -1.0f;
        }
    }
};