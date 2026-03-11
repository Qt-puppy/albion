#pragma once
#include "ECS.h"
#include <vector>

class PhysicsSystem {
public:
    static void CheckCollisionTask(Registry& registry, Entity e1, Entity e2, float deltaTime) {
        auto& t1 = registry.GetTransform(e1);
        auto& c1 = registry.GetBoxCollider(e1);
        auto& v1 = registry.GetVelocity(e1);

        auto& t2 = registry.GetTransform(e2);
        auto& c2 = registry.GetBoxCollider(e2);
        auto& v2 = registry.GetVelocity(e2); // We need the second entity's velocity now!

        bool collisionX = (t1.x + c1.width > t2.x) && (t1.x < t2.x + c2.width);
        bool collisionY = (t1.y + c1.height > t2.y) && (t1.y < t2.y + c2.height);

        if (collisionX && collisionY) {
            // BILLIARD BALL ELASTIC BOUNCE
            // Swap their velocities so they transfer momentum to each other
            float tempDx = v1.dx;
            float tempDy = v1.dy;
            
            // Give entity 1 the speed of entity 2
            v1.dx = v2.dx;
            v1.dy = v2.dy;
            
            // Give entity 2 the speed of entity 1
            v2.dx = tempDx;
            v2.dy = tempDy;

            // Push them apart slightly so they don't get permanently stuck inside each other
            t1.x += v1.dx * deltaTime * 2.0f;
            t1.y += v1.dy * deltaTime * 2.0f;
            t2.x += v2.dx * deltaTime * 2.0f;
            t2.y += v2.dy * deltaTime * 2.0f;
        }
    }

    void Update(Registry& registry, const std::vector<Entity>& entities, float deltaTime) {
        for (size_t i = 0; i < entities.size(); i++) {
            for (size_t j = i + 1; j < entities.size(); j++) {
                CheckCollisionTask(registry, entities[i], entities[j], deltaTime);
            }
        }
    }
};