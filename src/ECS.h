#pragma once
#include <vector>
#include <cstdint>
#include <string>

using Entity = uint32_t;

struct TransformComponent { 
    float x, y; 
    float prevX, prevY; // NEW: The memory of the last safe frame
};
struct VelocityComponent { float dx, dy; };
struct SpriteComponent { std::string textureId; int width, height; };

// --- NEW COMPONENT ---
struct BoxColliderComponent {
    int width, height;
    int offsetX, offsetY;
};

class Registry {
private:
    uint32_t nextEntityId = 0;

    std::vector<TransformComponent> transforms;
    std::vector<VelocityComponent> velocities;
    std::vector<SpriteComponent> sprites;
    std::vector<BoxColliderComponent> colliders; // NEW MEMORY POOL

public:
    Entity CreateEntity() {
        Entity newId = nextEntityId++;
        if (newId >= transforms.size()) {
            transforms.resize(newId + 1);
            velocities.resize(newId + 1);
            sprites.resize(newId + 1);
            colliders.resize(newId + 1); // Resize the new pool
        }
        return newId;
    }

    void AddTransform(Entity entity, float x, float y) { transforms[entity] = {x, y ,x ,y}; }
    void AddVelocity(Entity entity, float dx, float dy) { velocities[entity] = {dx, dy}; }
    void AddSprite(Entity entity, std::string id, int w, int h) { sprites[entity] = {id, w, h}; }
    
    // Attach a hitbox to an entity
    void AddBoxCollider(Entity entity, int w, int h, int ox = 0, int oy = 0) { 
        colliders[entity] = {w, h, ox, oy}; 
    }

    TransformComponent& GetTransform(Entity entity) { return transforms[entity]; }
    VelocityComponent& GetVelocity(Entity entity) { return velocities[entity]; }
    SpriteComponent& GetSprite(Entity entity) { return sprites[entity]; }
    BoxColliderComponent& GetBoxCollider(Entity entity) { return colliders[entity]; }
};