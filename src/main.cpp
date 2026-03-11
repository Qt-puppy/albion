#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "InputManager.h"
#include "SceneManager.h"
#include "GameplayScene.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow("Albion Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // --- DAY 15: IMGUI SETUP ---
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    InputManager input;
    SceneManager sceneManager;
    sceneManager.ChangeScene(new GameplayScene(renderer));

    bool isRunning = true;
    SDL_Event event;
    Uint32 lastFrameTime = SDL_GetTicks(); 

    while (isRunning) {
        Uint32 currentTicks = SDL_GetTicks();
        float deltaTime = (currentTicks - lastFrameTime) / 1000.0f; 
        lastFrameTime = currentTicks; 

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event); // Forward inputs to UI
            if (event.type == SDL_QUIT) isRunning = false; 
        }

        // --- ENGINE LOGIC ---
        sceneManager.Update(input, deltaTime);

        // --- IMGUI NEW FRAME ---
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // --- YOUR DEV CONSOLE ---
        // --- YOUR DEV CONSOLE ---
        ImGui::Begin("Albion Core Tech Console");
        
        // 1. Live Framerate Monitor
        ImGui::Text("Performance: %.1f FPS (%.3f ms/frame)", ImGui::GetIO().Framerate, 1000.0f / ImGui::GetIO().Framerate);
        
        ImGui::Separator();

        // 2. Engine Controls
        ImGui::Text("Engine Controls");
        static float bgColor[3] = { 0.11f, 0.11f, 0.11f }; // Default dark gray
        if (ImGui::ColorEdit3("Background Color", bgColor)) {
            // This will change the scene's background color dynamically!
            SDL_SetRenderDrawColor(renderer, bgColor[0] * 255, bgColor[1] * 255, bgColor[2] * 255, 255);
        }

        ImGui::Separator();

        // 3. The Benchmark Spawner
        // 3. The Arena Spawner
        ImGui::Text("Arena Control");
        static int entityCount = 2; // 1 Player, 1 Enemy
        ImGui::Text("Active Entities: %d", entityCount);
        
        if (ImGui::Button("+ Spawn 1 Enemy")) {
            sceneManager.SpawnEnemy();
            entityCount += 1;
        }

        ImGui::End();

        // --- RENDER PIPELINE ---
        sceneManager.Render(renderer);

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

        SDL_RenderPresent(renderer);
    }

    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}