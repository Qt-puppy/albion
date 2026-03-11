#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <unordered_map>
#include <string>
#include <iostream>

class ResourceManager {
private:
    // Hash map to store our loaded GPU textures by name
    std::unordered_map<std::string, SDL_Texture*> textures;

public:
    ~ResourceManager() {
        // Prevent VRAM memory leaks!
        for (auto& pair : textures) {
            SDL_DestroyTexture(pair.second);
        }
        textures.clear();
    }

    void LoadTexture(SDL_Renderer* renderer, const std::string& id, const std::string& filePath) {
        SDL_Surface* surface = IMG_Load(filePath.c_str());
        if (!surface) {
            std::cerr << "Failed to load image: " << filePath << std::endl;
            return;
        }
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface); // Delete the raw CPU data, keep the GPU data
        
        textures[id] = texture;
    }

    SDL_Texture* GetTexture(const std::string& id) {
        return textures[id];
    }
};