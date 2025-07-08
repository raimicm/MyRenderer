#pragma once

#include <SDL3/SDL.h>
#include <iostream>
#include <cstdint>
#include <limits>

template <uint8_t S = 1> // each pixel has S * S supersamples
class Canvas {
static_assert(S <= 4, "Supersample size is too large");
static_assert(S > 0, "Supersample size is too small");
public:
    Canvas(int width, int height, const char* name = "Canvas");
    ~Canvas();

    int width() const;
    int height() const;

    void update();

    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
    void setSample(float x, float y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
    void addSample(float x, float y, uint8_t r, uint8_t g, uint8_t b, float z);

private:
    struct Pixel {
        struct Sample {
            Sample();
            Sample(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
            Sample(uint8_t r, uint8_t g, uint8_t b, float z);

            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
            float z;
        };

        Pixel();

        void reset();
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
        uint32_t averageColor() const;

        Sample samples[S * S];
    };


    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint32_t* frame_buffer;
    Pixel* pixel_buffer;

    int w;
    int h;
};