#include <SDL2/SDL.h>
#include <cmath>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

struct Wave {
    float amplitude;
    float frequency;
    float phase;
    float speed;
};

void updateWavePosition(Wave& wave) {
    wave.phase += wave.speed;
    if (wave.phase >= 2 * M_PI) {
        wave.phase -= 2 * M_PI;
    }
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Ondas Secuenciales", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Wave wave1 = {50.0f, 0.1f, 0.0f, 0.02f};
    Wave wave2 = {30.0f, 0.05f, 0.0f, 0.05f};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        updateWavePosition(wave1);
        updateWavePosition(wave2);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            int y1 = SCREEN_HEIGHT / 2 + static_cast<int>(wave1.amplitude * sin(wave1.frequency * x + wave1.phase));
            int y2 = SCREEN_HEIGHT / 2 + static_cast<int>(wave2.amplitude * sin(wave2.frequency * x + wave2.phase));

            SDL_RenderDrawPoint(renderer, x, y1);
            SDL_RenderDrawPoint(renderer, x, y2);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
