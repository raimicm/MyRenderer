#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "canvas.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;

int main() {
    Canvas<2> canvas(WINDOW_WIDTH, WINDOW_HEIGHT, "ObjRenderer");

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        for (float x = 100.25f; x < 699.75f; x += 0.5f) {
            for (float y = 50.25f; y < 349.75f; y += 0.5f) {
                canvas.addSample(x, y, 255, 0, 255, 2.0f);
            }
        }

        for (float x = 300.5f; x < 899.5f; x += 0.5f) {
            for (float y = 150.5f; y < 449.5f; y += 0.5f) {
                canvas.addSample(x, y, 0, 255, 255, 1.0f);
            }
        }

        canvas.update();
        SDL_Delay(16);
    }

    return 0;
}