#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include "canvas.hpp"
#include "camera.hpp"
#include "model.hpp"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 500;
const uint64_t FRAME_RATE = 60; // in frames per second

int main(int argc, char* argv[]) {
    Canvas<2> canvas(WINDOW_WIDTH, WINDOW_HEIGHT, "ObjRenderer");
    Model model(argv[1]);
    Camera camera(static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT, 2.0f * model.maxOriginDistance());

    bool mouse_down = false;
    glm::vec2 mouse_dpos;
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_down = true;
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_down = false;
                    }
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    mouse_dpos.x = event.motion.xrel;
                    mouse_dpos.y = -event.motion.yrel;
                    break;
                case SDL_EVENT_MOUSE_WHEEL:
                    camera.changeZoom(0.1f * event.wheel.y);
            }

            if (mouse_down) {
                camera.orbit(glm::vec3(0.0f, 1.0f, 0.0f), 0.01f * mouse_dpos.x);
                camera.orbit(cross(glm::vec3(0.0f, 1.0f, 0.0f), camera.pos), -0.01f * mouse_dpos.y);
            }
        }

        uint64_t t0 = SDL_GetTicks();
        model.render(canvas, camera);
        canvas.update();
        uint64_t dt = SDL_GetTicks() - t0;
        if (dt < 1000 / FRAME_RATE)
            SDL_Delay(1000 / FRAME_RATE - dt); // caps frame rate
    }

    return 0;
}