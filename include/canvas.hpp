#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

class Canvas {
public:
    Canvas(int width, int height);
    ~Canvas();

    int width();
    int height();

    void update();
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* pixel_buffer;
    float* zbuffer;

    int w;
    int h;
};