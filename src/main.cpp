#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "canvas.hpp"

const int WINDOW_WIDTH = 1800;
const int WINDOW_HEIGHT = 900;

int main() {
    Canvas canvas(WINDOW_WIDTH, WINDOW_HEIGHT);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        for (int x = 400; x < 1400; x++) {
            for (int y = 200; y < 700; y++) {
                canvas.setPixel(x, y, 255, 0, 0);
            }
        }

        canvas.update();
        SDL_Delay(16);
    }

    return 0;
}