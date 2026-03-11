#pragma once
#include <SDL2/SDL.h>
#include "ECS.h"
#include "ResourceManager.h"

class RenderSystem {
public:
    void Render(SDL_Renderer* renderer, Registry& registry, Entity entity, ResourceManager& assets, SDL_Rect& camera) {
        
        TransformComponent& transform = registry.GetTransform(entity);
        SpriteComponent& sprite = registry.GetSprite(entity);

        SDL_Rect destRect = {
            static_cast<int>(transform.x) - camera.x, 
            static_cast<int>(transform.y) - camera.y,
            sprite.width,
            sprite.height
        };

        SDL_Texture* texture = assets.GetTexture(sprite.textureId);
        SDL_RenderCopy(renderer, texture, NULL, &destRect);
    }
};