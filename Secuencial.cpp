#include <iostream>
#include <SDL2/SDL.h>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int NUM_WAVES = 3;
const int AMPLITUDE = 100;
const int PERIOD = 200;
const double ANGLE_SPEED = 0.02;

struct Wave {
    int yOffset;
    double angle;
};

std::vector<Wave> waves;

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_CreateWindow("Wave Screensaver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    return true;
}

void closeSDL() {
    SDL_Quit();
}

void updateWaves() {
    for (auto& wave : waves) {
        wave.angle += ANGLE_SPEED;
    }
}

void renderWaves(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int x = 0; x < SCREEN_WIDTH; ++x) {
        int sum = 0;
        for (const auto& wave : waves) {
            int y = wave.yOffset + AMPLITUDE * std::sin(x * 2 * M_PI / PERIOD + wave.angle);
            sum += y;
        }
        int avgY = sum / NUM_WAVES;

        SDL_SetRenderDrawColor(renderer, rand() % 256, rand() % 256, rand() % 256, 255);
        SDL_RenderDrawPoint(renderer, x, SCREEN_HEIGHT / 2 + avgY);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (!initSDL()) {
        return 1;
    }

    srand(static_cast<unsigned int>(time(nullptr)));

    SDL_Window* window = SDL_CreateWindow("Wave Screensaver", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window creation failed. SDL Error: " << SDL_GetError() << std::endl;
        closeSDL();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer creation failed. SDL Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        closeSDL();
        return 1;
    }

    // Crear las ondas con desplazamientos verticales iniciales aleatorios
    for (int i = 0; i < NUM_WAVES; ++i) {
        Wave wave;
        wave.yOffset = rand() % SCREEN_HEIGHT;
        wave.angle = 0.0;
        waves.push_back(wave);
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        updateWaves();
        renderWaves(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    closeSDL();

    return 0;
}
