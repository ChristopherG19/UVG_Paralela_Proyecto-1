#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <random>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int NUM_WAVES = 50;  // Cambia este valor según la cantidad de ondas que desees

struct Wave {
    float amplitude;
    float frequency;
    float phase;
    float speed;
    float direction;
    int startY;
};

void updateWavePosition(Wave& wave) {
    wave.phase += wave.speed * wave.direction;
    if (wave.phase >= 2 * M_PI || wave.phase <= 0.0f) {
        wave.direction *= -1; // Cambia la dirección al chocar con los bordes
    }
}

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Ondas en movimiento", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Wave> waves;

    // Inicializa las ondas con valores aleatorios
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist_amplitude(10.0f, 100.0f);
    std::uniform_real_distribution<float> dist_frequency(0.01f, 0.1f);
    std::uniform_real_distribution<float> dist_speed(0.005f, 0.02f);
    std::uniform_int_distribution<int> dist_startY(0, SCREEN_HEIGHT);

    for (int i = 0; i < NUM_WAVES; ++i) {
        Wave wave;
        wave.amplitude = dist_amplitude(gen);
        wave.frequency = dist_frequency(gen);
        wave.phase = 0.0f;
        wave.speed = dist_speed(gen);
        wave.direction = 1.0f; // Dirección inicial
        wave.startY = dist_startY(gen);

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

        for (auto& wave : waves) {
            updateWavePosition(wave);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        for (int x = 0; x < SCREEN_WIDTH; ++x) {
            for (const auto& wave : waves) {
                int y = wave.startY + static_cast<int>(wave.amplitude * sin(wave.frequency * x + wave.phase));
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
