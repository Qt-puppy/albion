#pragma once
#include "ECS.h"
#include <vector>

// Define our grid constraints
const int CELL_SIZE = 64; 
const int ARENA_WIDTH = 800;
const int ARENA_HEIGHT = 600;
const int COLS = (ARENA_WIDTH / CELL_SIZE) + 1; // 13 columns
const int ROWS = (ARENA_HEIGHT / CELL_SIZE) + 1; // 10 rows

class PhysicsSystem {
public:
    static void CheckCollisionTask(Registry& registry, Entity e1, Entity e2, float deltaTime) {
        auto& t1 = registry.GetTransform(e1);
        auto& c1 = registry.GetBoxCollider(e1);
        auto& v1 = registry.GetVelocity(e1);

        auto& t2 = registry.GetTransform(e2);
        auto& c2 = registry.GetBoxCollider(e2);
        auto& v2 = registry.GetVelocity(e2); 

        bool collisionX = (t1.x + c1.width > t2.x) && (t1.x < t2.x + c2.width);
        bool collisionY = (t1.y + c1.height > t2.y) && (t1.y < t2.y + c2.height);

        if (collisionX && collisionY) {
            // Swap velocities (Billiard Bounce)
            float tempDx = v1.dx;
            float tempDy = v1.dy;
            v1.dx = v2.dx;
            v1.dy = v2.dy;
            v2.dx = tempDx;
            v2.dy = tempDy;

            // Push apart
            t1.x += v1.dx * deltaTime * 2.0f;
            t1.y += v1.dy * deltaTime * 2.0f;
            t2.x += v2.dx * deltaTime * 2.0f;
            t2.y += v2.dy * deltaTime * 2.0f;
        }
    }

    void Update(Registry& registry, const std::vector<Entity>& entities, float deltaTime) {
        // 1. Create our flat grid of buckets (130 cells total)
        std::vector<Entity> grid[COLS * ROWS];

        // 2. THE HASH: Drop every entity into its proper cell O(N)
        for (Entity e : entities) {
            auto& t = registry.GetTransform(e);
            
            int col = static_cast<int>(t.x) / CELL_SIZE;
            int row = static_cast<int>(t.y) / CELL_SIZE;
            
            // Safety bounds check
            if (col >= 0 && col < COLS && row >= 0 && row < ROWS) {
                grid[row * COLS + col].push_back(e);
            }
        }

        // 3. THE RESOLUTION: Only check neighbors O(N)
        for (Entity e1 : entities) {
            auto& t1 = registry.GetTransform(e1);
            int col = static_cast<int>(t1.x) / CELL_SIZE;
            int row = static_cast<int>(t1.y) / CELL_SIZE;

            // Loop through the 9 neighboring cells (including the one e1 is in)
            for (int y = -1; y <= 1; y++) {
                for (int x = -1; x <= 1; x++) {
                    int checkCol = col + x;
                    int checkRow = row + y;

                    // If the neighbor cell is actually on screen
                    if (checkCol >= 0 && checkCol < COLS && checkRow >= 0 && checkRow < ROWS) {
                        int index = checkRow * COLS + checkCol;
                        
                        for (Entity e2 : grid[index]) {
                            // Engine Pro-Tip: "e1 < e2" prevents double-resolving!
                            // If e1 checks e2, we don't want e2 to bounce e1 again later.
                            if (e1 < e2) { 
                                CheckCollisionTask(registry, e1, e2, deltaTime);
                            }
                        }
                    }
                }
            }
        }
    }
};