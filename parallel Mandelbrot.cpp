#include <SDL2/SDL.h>
#include <complex>
#include <omp.h>
#include <vector>
#include <iostream>
#include <cmath>

// Window dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Mandelbrot parameters
const int MAX_ITERATIONS = 1000;
double zoom = 1.0;
double offsetX = -0.5;
double offsetY = 0.0;

// Color palettes
enum ColorPalette {
    CLASSIC = 0,
    FIRE = 1,
    ELECTRIC = 2,
    OCEAN = 3,
    GRAYSCALE = 4
};

ColorPalette currentPalette = CLASSIC;

// Function to compute mandelbrot value for a single point
int computeMandelbrot(double real, double imag) {
    std::complex<double> c(real, imag);
    std::complex<double> z(0.0, 0.0);
    
    int iterations = 0;
    while (std::abs(z) <= 2.0 && iterations < MAX_ITERATIONS) {
        z = z * z + c;
        iterations++;
    }
    
    return iterations;
}

// Apply color based on iteration count
SDL_Color getColor(int iterations, ColorPalette palette) {
    SDL_Color color = {0, 0, 0, 255}; // Black for points in the set
    
    if (iterations == MAX_ITERATIONS) {
        return color; // Return black for points inside the set
    }
    
    double hue = 0.0;
    double saturation = 0.8;
    double value = 1.0;
    
    // Smooth coloring
    double smoothed = iterations + 1 - std::log(std::log(std::sqrt(iterations))) / std::log(2.0);
    smoothed = fmod(smoothed, 256.0) / 256.0;
    
    switch (palette) {
        case CLASSIC:
            // Classic blue-purple gradient
            hue = 0.6 + smoothed * 0.4; // Between blue and purple
            break;
        case FIRE:
            // Fire colors
            hue = fmod(smoothed * 0.2, 1.0); // Red to yellow range
            value = std::min(1.0, 0.5 + smoothed);
            break;
        case ELECTRIC:
            // Electric blue and purple
            hue = 0.5 + 0.3 * smoothed;
            saturation = 0.8 + 0.2 * sin(smoothed * 6.28);
            break;
        case OCEAN:
            // Ocean blues and greens
            hue = 0.3 + 0.3 * smoothed;
            break;
        case GRAYSCALE:
            // Grayscale
            color.r = color.g = color.b = (Uint8)(255.0 * (1.0 - smoothed));
            return color;
    }
    
    // HSV to RGB conversion
    if (palette != GRAYSCALE) {
        int hi = (int)(hue * 6.0);
        double f = hue * 6.0 - hi;
        double p = value * (1.0 - saturation);
        double q = value * (1.0 - f * saturation);
        double t = value * (1.0 - (1.0 - f) * saturation);
        
        switch (hi % 6) {
            case 0: color.r = (Uint8)(value * 255); color.g = (Uint8)(t * 255); color.b = (Uint8)(p * 255); break;
            case 1: color.r = (Uint8)(q * 255); color.g = (Uint8)(value * 255); color.b = (Uint8)(p * 255); break;
            case 2: color.r = (Uint8)(p * 255); color.g = (Uint8)(value * 255); color.b = (Uint8)(t * 255); break;
            case 3: color.r = (Uint8)(p * 255); color.g = (Uint8)(q * 255); color.b = (Uint8)(value * 255); break;
            case 4: color.r = (Uint8)(t * 255); color.g = (Uint8)(p * 255); color.b = (Uint8)(value * 255); break;
            case 5: color.r = (Uint8)(value * 255); color.g = (Uint8)(p * 255); color.b = (Uint8)(q * 255); break;
        }
    }
    
    return color;
}

// Render the Mandelbrot set in parallel
void renderMandelbrot(SDL_Renderer* renderer) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Calculate mandelbrot values for each pixel in parallel
    std::vector<std::vector<int>> iterations(SCREEN_WIDTH, std::vector<int>(SCREEN_HEIGHT));
    
    double scale = 4.0 / (zoom * std::min(SCREEN_WIDTH, SCREEN_HEIGHT));
    
    #pragma omp parallel for collapse(2) schedule(dynamic)
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            double real = (x - SCREEN_WIDTH / 2) * scale + offsetX;
            double imag = (y - SCREEN_HEIGHT / 2) * scale + offsetY;
            
            iterations[x][y] = computeMandelbrot(real, imag);
        }
    }
    
    // Render the pixels
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            SDL_Color color = getColor(iterations[x][y], currentPalette);
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    
    SDL_RenderPresent(renderer);
}

// Display help text
void displayHelp() {
    // Here you would ideally use SDL_ttf to render text
    // But for simplicity we'll just print to console
    std::cout << "\n=== Mandelbrot Set Explorer Controls ===" << std::endl;
    std::cout << "Arrow keys: Move around" << std::endl;
    std::cout << "Scroll wheel or +/-: Zoom in/out" << std::endl;
    std::cout << "C: Change color palette" << std::endl;
    std::cout << "R: Reset view" << std::endl;
    std::cout << "H: Show this help" << std::endl;
    std::cout << "ESC or Q: Quit" << std::endl;
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Create window
    SDL_Window* window = SDL_CreateWindow("Parallel Mandelbrot Explorer", 
                                         SDL_WINDOWPOS_UNDEFINED, 
                                         SDL_WINDOWPOS_UNDEFINED,
                                         SCREEN_WIDTH, 
                                         SCREEN_HEIGHT, 
                                         SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    
    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Display controls
    displayHelp();
    
    // Initial render
    renderMandelbrot(renderer);
    
    // Main loop flag
    bool quit = false;
    
    // Event handler
    SDL_Event e;
    
    // Mouse coordinates for zooming
    int mouseX = SCREEN_WIDTH / 2;
    int mouseY = SCREEN_HEIGHT / 2;
    
    // Main loop
    while (!quit) {
        // Handle events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                    case SDL_SCANCODE_Q:
                        quit = true;
                        break;
                    case SDL_SCANCODE_R:
                        // Reset view
                        zoom = 1.0;
                        offsetX = -0.5;
                        offsetY = 0.0;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_C:
                        // Change color palette
                        currentPalette = static_cast<ColorPalette>((currentPalette + 1) % 5);
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_H:
                        // Show help
                        displayHelp();
                        break;
                    case SDL_SCANCODE_EQUALS:
                    case SDL_SCANCODE_KP_PLUS:
                        // Zoom in
                        zoom *= 1.5;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_MINUS:
                    case SDL_SCANCODE_KP_MINUS:
                        // Zoom out
                        zoom /= 1.5;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_UP:
                        // Move up
                        offsetY -= 0.1 / zoom;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_DOWN:
                        // Move down
                        offsetY += 0.1 / zoom;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_LEFT:
                        // Move left
                        offsetX -= 0.1 / zoom;
                        renderMandelbrot(renderer);
                        break;
                    case SDL_SCANCODE_RIGHT:
                        // Move right
                        offsetX += 0.1 / zoom;
                        renderMandelbrot(renderer);
                        break;
                }
            } else if (e.type == SDL_MOUSEWHEEL) {
                // Get current mouse position for more intuitive zooming
                SDL_GetMouseState(&mouseX, &mouseY);
                
                // Calculate the point to zoom into before zooming
                double scale = 4.0 / (zoom * std::min(SCREEN_WIDTH, SCREEN_HEIGHT));
                double targetX = (mouseX - SCREEN_WIDTH / 2) * scale + offsetX;
                double targetY = (mouseY - SCREEN_HEIGHT / 2) * scale + offsetY;
                
                // Zoom in or out
                if (e.wheel.y > 0) {
                    zoom *= 1.2;
                } else if (e.wheel.y < 0) {
                    zoom /= 1.2;
                }
                
                // Adjust offset to keep the point under the mouse
                scale = 4.0 / (zoom * std::min(SCREEN_WIDTH, SCREEN_HEIGHT));
                offsetX = targetX - (mouseX - SCREEN_WIDTH / 2) * scale;
                offsetY = targetY - (mouseY - SCREEN_HEIGHT / 2) * scale;
                
                renderMandelbrot(renderer);
            }
        }
    }
    
    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    return 0;
}