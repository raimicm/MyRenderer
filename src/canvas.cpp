#include "canvas.hpp"
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <cstdint>
#include <limits>

Canvas::Canvas(int width, int height) : w(width), h(height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow("My Renderer", width, height, SDL_WINDOW_ALWAYS_ON_TOP);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    pixel_buffer = new uint32_t[width * height];
    for (int i = 0; i < width * height; i++) {
        pixel_buffer[i] = 0;
    }

    zbuffer = new float[width * height];
    for (int i = 0; i < width * height; i++) {
        zbuffer[i] = std::numeric_limits<float>::min();
    }
}

Canvas::~Canvas() {
    delete[] pixel_buffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Canvas::update() {
    SDL_UpdateTexture(texture, NULL, pixel_buffer, w * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

int Canvas::width() {
    return w;
}

int Canvas::height() {
    return h;
}

void Canvas::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    pixel_buffer[y * w + x] = (r << 24) | (g << 16) | (b << 8) | 0xFF;
}

void Canvas::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    pixel_buffer[y * w + x] = (r << 24) | (g << 16) | (b << 8) | a;
}