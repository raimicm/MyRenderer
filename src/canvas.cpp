#include "canvas.hpp"
#include <SDL3/SDL.h>
#include <iostream>
#include <cstdint>
#include <limits>
#include <cmath>

template <uint8_t S>
Canvas<S>::Canvas(int width, int height, const char* name) : w(width), h(height) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
    }

    window = SDL_CreateWindow(name, width, height, 0);
    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    renderer = SDL_CreateRenderer(window, NULL);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

    frame_buffer = new uint32_t[width * height];
    for (int i = 0; i < width * height; i++) {
        frame_buffer[i] = 0;
    }

    pixel_buffer = new Pixel[width * height];
    for (int i = 0; i < width * height; i++) {
        pixel_buffer[i] = Pixel();
    }
}

template <uint8_t S>
Canvas<S>::~Canvas() {
    delete[] frame_buffer;
    delete[] pixel_buffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

template <uint8_t S>
int Canvas<S>::width() const {
    return w;
}

template <uint8_t S>
int Canvas<S>::height() const {
    return h;
}

template <uint8_t S>
void Canvas<S>::update() {
    for (int i = 0; i < w * h; i++) {
        frame_buffer[i] = pixel_buffer[i].averageColor();
        pixel_buffer[i].reset(); // clear pixel buffer
    }
    SDL_UpdateTexture(texture, NULL, frame_buffer, w * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

template <uint8_t S>
void Canvas<S>::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (x < 0 || y < 0 || x >= w || y >= h) return;
    pixel_buffer[y * w + x].setColor(r, g, b, a);
}

template <uint8_t S>
void Canvas<S>::setSample(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    int px = static_cast<int>(floor(x));
    int py = static_cast<int>(floor(y));
    if (px < 0 || py < 0 || px >= w || py >= h) return;

    int tx = static_cast<int>(floor((x - floor(x)) * S));
    int ty = static_cast<int>(floor((y - floor(y)) * S));
    pixel_buffer[py * w + px].samples[ty * S + tx] = typename Canvas<S>::Pixel::Sample(r, g, b, a);
}

template <uint8_t S>
void Canvas<S>::addSample(float x, float y, uint8_t r, uint8_t g, uint8_t b, float z) {
    int px = static_cast<int>(floor(x));
    int py = static_cast<int>(floor(y));
    if (px < 0 || py < 0 || px >= w || py >= h) return;

    int tx = static_cast<int>(floor((x - floor(x)) * S));
    int ty = static_cast<int>(floor((y - floor(y)) * S));

    if (z < pixel_buffer[py * w + px].samples[ty * S + tx].z)
        pixel_buffer[py * w + px].samples[ty * S + tx] = typename Canvas<S>::Pixel::Sample(r, g, b, z);
}


template <uint8_t S>
Canvas<S>::Pixel::Sample::Sample() : r(0), g(0), b(0), a(0xFF), z(std::numeric_limits<float>::max()) {}

template <uint8_t S>
Canvas<S>::Pixel::Sample::Sample(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a), z(std::numeric_limits<float>::max()) {}

template <uint8_t S>
Canvas<S>::Pixel::Sample::Sample(uint8_t r, uint8_t g, uint8_t b, float z) : r(r), g(g), b(b), a(0xFF), z(z) {}


template <uint8_t S>
Canvas<S>::Pixel::Pixel() {
    for (int i = 0; i < S * S; i++) {
        samples[i] = typename Canvas<S>::Pixel::Sample();
    }
}

template <uint8_t S>
void Canvas<S>::Pixel::reset() {
    for (int i = 0; i < S * S; i++) {
        samples[i].r = 0;
        samples[i].g = 0;
        samples[i].b = 0;
        samples[i].a = 255;
        samples[i].z = std::numeric_limits<float>::max();
    }
}

template <uint8_t S>
void Canvas<S>::Pixel::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    for (int i = 0; i < S * S; i++) {
        samples[i].r = r;
        samples[i].g = g;
        samples[i].b = b;
        samples[i].a = a;
    }
}

template <uint8_t S>
uint32_t Canvas<S>::Pixel::averageColor() const {
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    uint16_t a = 255; // only opaque objects for now

    for (int i = 0; i < S * S; i++) {
        r += samples[i].r;
        g += samples[i].g;
        b += samples[i].b;
    }
    
    r /= S * S;
    g /= S * S;
    b /= S * S;

    return (r << 24) | (g << 16) | (b << 8) | a;
}

// explicitly instantiated templates
template class Canvas<1>;
template class Canvas<2>;
template class Canvas<3>;
template class Canvas<4>;