#pragma once
#include "ECS.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

class LevelLoader {
public:
    void LoadLevel(Registry& registry, const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "CRITICAL: Could not open level file: " << filepath << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            // Skip comments and empty lines
            if (line.empty() || line[0] == '#') continue;

            std::istringstream iss(line);
            std::string type, textureId;
            float x, y;
            int w, h;

            // Parse the data
            if (iss >> type >> x >> y >> w >> h >> textureId) {
                Entity e = registry.CreateEntity();
                registry.AddTransform(e, x, y);
                registry.AddSprite(e, textureId, w, h);
                registry.AddBoxCollider(e, w, h);
                registry.AddVelocity(e, 0.0f, 0.0f); // Default to stationary
                
                std::cout << "Spawned " << type << " at " << x << "," << y << std::endl;
            }
        }
    }
};