#include <SDL2/SDL.h>
#include <cmath>
#include <vector>
#include <random>
#include <iostream>
#include <string>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int WAVE_INTERVAL = 1000;
const int INITIAL_WAVE_LENGTH = 100; // Longitud inicial de las ondas
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
        // Si no se proporciona el número correcto de argumentos, muestra un mensaje de error y salida.
        std::cout << "Es necesario establecer la cantidad de figuras: ./prog <cantidad>" << std::endl;
        return 1;
    }

    if (argc > 1) {
        try {
            NUM_WAVES = std::stoi(args[1]);
            if (NUM_WAVES == 0) {
                // Si el valor es 0, utiliza el valor predeterminado y muestra un mensaje.
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist_amplitude(10.0f, 100.0f);
    std::uniform_real_distribution<float> dist_frequency(0.01f, 0.1f);
    std::uniform_real_distribution<float> dist_speed(0.005f, 0.02f);
    std::uniform_int_distribution<int> dist_startX(0, SCREEN_WIDTH);
    std::uniform_int_distribution<int> dist_startY(0, SCREEN_HEIGHT);
    std::uniform_real_distribution<float> dist_direction(-1.0f, 1.0f);

    Uint32 lastWaveTime = SDL_GetTicks();

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastWaveTime >= WAVE_INTERVAL && waves.size() < NUM_WAVES) {
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
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (auto& wave : waves) {
            updateWavePosition(wave);

            SDL_SetRenderDrawColor(renderer, (wave.color >> 24) & 0xFF, (wave.color >> 16) & 0xFF, (wave.color >> 8) & 0xFF, wave.color & 0xFF);

            for (int i = 0; i < wave.length; ++i) {
                int x = wave.startX + static_cast<int>(i * wave.directionX);
                int y = wave.startY + static_cast<int>(i * wave.directionY + wave.amplitude * sin(wave.frequency * i + wave.phase));
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