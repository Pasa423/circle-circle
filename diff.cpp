
#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <algorithm>
#include <cmath>

const int WIDTH = 1100;
const int HEIGHT = 700;

const int GRID_W = 100;
const int GRID_H = 100;
const int CELL_SIZE = 7;

const float DIFFUSION = 0.30f;

using Grid = std::vector<float>;

int indexOf(int x, int y) {
    return y * GRID_W + x;
}

void resetGrid(Grid& temperature) {
    std::fill(temperature.begin(), temperature.end(), 20.0f);

    // Initial hot spot in the center
    int cx = GRID_W / 2;
    int cy = GRID_H / 2;

    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            float dx = x - cx;
            float dy = y - cy;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < 8.0f) {
                temperature[indexOf(x, y)] = 100.0f;
            }
        }
    }
} 
 

void updateGrid(const Grid& current, Grid& next) {
    // Keep edge cells insulated.
    for (int y = 0; y < GRID_H; y++) {
        for (int x = 0; x < GRID_W; x++) {
            int i = indexOf(x, y);

            // No heat transfer through the outer boundary.
            if (x == 0 || x == GRID_W - 1 ||
                y == 0 || y == GRID_H - 1) {
                next[i] = current[i];
                continue;
            }

            float center = current[i];

            float up    = current[indexOf(x, y - 1)];
            float down  = current[indexOf(x, y + 1)];
            float left  = current[indexOf(x - 1, y)];
            float right = current[indexOf(x + 1, y)];

            float difference =
                up + down + left + right - 4.0f * center;

            next[i] = center + DIFFUSION * difference;
        }
    }
}

Color temperatureColor(float temperature) {
    temperature = Clamp(temperature, 20.0f, 100.0f);

    float t = (temperature - 20.0f) / 80.0f;

    // Cold blue -> cyan -> yellow -> hot red
    if (t < 0.33f) {
        float u = t / 0.33f;

        return {
            0,
            (unsigned char)(40 + 180 * u),
            255,
            255
        };
    }

    if (t < 0.66f) {
        float u = (t - 0.33f) / 0.33f;

        return {
            (unsigned char)(255 * u),
            220,
            (unsigned char)(255 * (1.0f - u)),
            255
        };
    }

    float u = (t - 0.66f) / 0.34f;

    return {
        255,
        (unsigned char)(220 * (1.0f - u)),
        0,
        255
    };
}

void heatAtMouse(Grid& temperature, Vector2 mouse) {
    int cx = (int)mouse.x / CELL_SIZE;
    int cy = (int)mouse.y / CELL_SIZE;

    if (cx < 1 || cx >= GRID_W - 1 ||
        cy < 1 || cy >= GRID_H - 1) {
        return;
    }

    // Paint a small hot spot.
    for (int y = cy - 3; y <= cy + 3; y++) {
        for (int x = cx - 3; x <= cx + 3; x++) {
            float dx = x - cx;
            float dy = y - cy;

            if (dx * dx + dy * dy <= 9.0f) {
                temperature[indexOf(x, y)] = 100.0f;
            }
        }
    }
}

int main() {
    InitWindow(WIDTH, HEIGHT, "2D Heat Diffusion");
    SetTargetFPS(60);

    Grid temperature(GRID_W * GRID_H, 20.0f);
    Grid nextTemperature(GRID_W * GRID_H, 20.0f);

    resetGrid(temperature);

    bool paused = false;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_R)) {
            resetGrid(temperature);
            paused = false;
        }

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            Vector2 mouse = GetMousePosition();

            if (mouse.x < GRID_W * CELL_SIZE &&
                mouse.y < GRID_H * CELL_SIZE) {
                heatAtMouse(temperature, mouse);
            }
        }

        if (!paused) {
            // Each iteration advances simulated time by one step.
            updateGrid(temperature, nextTemperature);
            temperature.swap(nextTemperature);
        }

        BeginDrawing();
        ClearBackground(Color{22, 25, 32, 255});

        // Draw temperature grid.
        for (int y = 0; y < GRID_H; y++) {
            for (int x = 0; x < GRID_W; x++) {
                float t = temperature[indexOf(x, y)];

                DrawRectangle(
                    x * CELL_SIZE,
                    y * CELL_SIZE,
                    CELL_SIZE,
                    CELL_SIZE,
                    temperatureColor(t)
                );
            }
        }

        // Information panel
        int panelX = GRID_W * CELL_SIZE + 25;

        DrawText("HEAT DIFFUSION", panelX, 30, 24, RAYWHITE);
        DrawText("2D numerical simulation", panelX, 65, 18, LIGHTGRAY);

        DrawText("Blue = cold", panelX, 125, 18, SKYBLUE);
        DrawText("Yellow = warm", panelX, 155, 18, YELLOW);
        DrawText("Red = hot", panelX, 185, 18, RED);

        DrawText("Controls", panelX, 250, 20, RAYWHITE);
        DrawText("Hold LEFT MOUSE to heat", panelX, 285, 16, LIGHTGRAY);
        DrawText("SPACE: Pause / Resume", panelX, 315, 16, LIGHTGRAY);
        DrawText("R: Reset", panelX, 345, 16, LIGHTGRAY);
        DrawText("ESC: Quit", panelX, 375, 16, LIGHTGRAY);

        DrawText(
            paused ? "PAUSED" : "RUNNING",
            panelX, 430, 20,
            paused ? YELLOW : GREEN
        );

        DrawText("Diffusion coefficient r = 0.20",
                 panelX, 475, 16, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

