#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <string>
#include <omp.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int WAVE_INTERVAL = 1000;
const int INITIAL_WAVE_LENGTH = 100;
const float PI = 3.14159265359f;

struct Wave {
    float amplitude;
    float frequency;
    float phase;
    float speed;
    int startX;
    int startY;
    float directionX;
    float directionY;
    Uint32 color;
    int length;
};

void updateWavePosition(Wave& wave) {
    wave.phase += wave.speed;
    if (wave.phase >= 2 * PI) {
        wave.phase -= 2 * PI;
    }
}

void generateRandomColor(Wave& wave) {
    wave.color = SDL_MapRGB(SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888), rand() % 256, rand() % 256, rand() % 256);
}

int main(int argc, char* args[]) {
    int NUM_WAVES = 50;

    if (argc != 2) {
        std::cout << "Es necesario establecer la cantidad de figuras: ./prog <cantidad>" << std::endl;
        return 1;
    }

    if (argc > 1) {
        try {
            NUM_WAVES = std::stoi(args[1]);
            if (NUM_WAVES == 0) {
                NUM_WAVES = 50;
                std::cout << "Se usará el valor predeterminado de " << NUM_WAVES << std::endl;
                std::cout << "Cantidad de elementos a renderizar: " << NUM_WAVES << std::endl;
            } else {
                for (int i = 1; i < argc; i++) {
                    std::cout << "Cantidad de elementos a renderizar: " << i << ": " << args[i] << std::endl;
                }
            }
        } catch (std::invalid_argument& e) {
            std::cout << "Error: Ingreso incorrecto de datos. La cantidad de figuras debe ser un valor numérico." << std::endl;
            return 1;
        }
    }

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Ondas en movimiento", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    std::vector<Wave> waves;
    omp_lock_t wavesMutex;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist_amplitude(10.0f, 100.0f);
    std::uniform_real_distribution<float> dist_frequency(0.01f, 0.1f);
    std::uniform_real_distribution<float> dist_speed(0.005f, 0.02f);
    std::uniform_int_distribution<int> dist_startX(0, SCREEN_WIDTH);
    std::uniform_int_distribution<int> dist_startY(0, SCREEN_HEIGHT);
    std::uniform_real_distribution<float> dist_direction(-1.0f, 1.0f);

    Uint32 lastWaveTime = SDL_GetTicks();

    Uint32 frameCount = 0;
    Uint32 lastUpdateTime = 0;
    float currentFPS = 0.0f;

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        frameCount++;
        Uint32 currentTime = SDL_GetTicks();
        Uint32 elapsedTime = currentTime - lastUpdateTime;
        if (elapsedTime >= 1000) {
            currentFPS = static_cast<float>(frameCount) / (elapsedTime / 1000.0f);
            frameCount = 0;
            lastUpdateTime = currentTime;
        }
        // std::cout << "FPS: " << currentFPS << std::endl;

        if (currentTime - lastWaveTime >= WAVE_INTERVAL && waves.size() < NUM_WAVES) {
            double start_time, end_time;
            start_time = omp_get_wtime();

            Wave wave;
            wave.amplitude = dist_amplitude(gen);
            wave.frequency = dist_frequency(gen);
            wave.phase = 0.0f;
            wave.speed = dist_speed(gen);
            wave.startX = dist_startX(gen);
            wave.startY = dist_startY(gen);
            generateRandomColor(wave);
            wave.length = INITIAL_WAVE_LENGTH;
            wave.directionX = dist_direction(gen);
            wave.directionY = dist_direction(gen);

            waves.push_back(wave);
            lastWaveTime = currentTime;

            end_time = omp_get_wtime();
            double wave_creation_time = end_time - start_time;
            std::cout << "Tiempo de creación de onda: " << wave_creation_time << " segundos" << std::endl;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        omp_init_lock(&wavesMutex);

        #pragma omp parallel for schedule(auto)
        for (size_t i = 0; i < waves.size(); ++i) {
            Wave& wave = waves[i];

            updateWavePosition(wave);

            omp_set_lock(&wavesMutex);

            SDL_SetRenderDrawColor(renderer, (wave.color >> 24) & 0xFF, (wave.color >> 16) & 0xFF, (wave.color >> 8) & 0xFF, wave.color & 0xFF);

            for (int i = 0; i < wave.length; ++i) {
                int x = wave.startX + static_cast<int>(i * wave.directionX);
                int y = wave.startY + static_cast<int>(i * wave.directionY + wave.amplitude * sin(wave.frequency * i + wave.phase));
                SDL_RenderDrawPoint(renderer, x, y);
            }

            omp_unset_lock(&wavesMutex);
            
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
