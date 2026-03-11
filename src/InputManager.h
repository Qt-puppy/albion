#pragma once
#include <SDL2/SDL.h>

class InputManager {
private:
    // A pointer to SDL's internal array of 1s and 0s (pressed or not pressed)
    const Uint8* keyboardState;

public:
    // Constructor: Runs once when we create the InputManager
    InputManager() {
        // Grab the memory address of SDL's keyboard array
        keyboardState = SDL_GetKeyboardState(nullptr);
    }

    // function for the rest of our engine to use
    bool IsKeyPressed(SDL_Scancode key) {
        // If the array at this key's index is 1, it returns true
        return keyboardState[key] == 1;
    }
};