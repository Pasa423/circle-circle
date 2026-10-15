
#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

// -------------------- DATA STRUCTURES --------------------

struct Rabbit {
    Vector2 pos;
};

struct Fox {
    Vector2 pos;
    float hunger = 0.0f;
};

// -------------------- RANDOM NUMBER GENERATOR --------------------

std::mt19937 rng(std::random_device{}());

float randomFloat(float min, float max) {
    return std::uniform_real_distribution<float>(min, max)(rng);
}

bool chance(float probability) {
    return randomFloat(0.0f, 1.0f) < probability;
}

Vector2 randomPosition() {
    return {
        randomFloat(10.0f, 690.0f),
        randomFloat(10.0f, 680.0f)
    };
}

// -------------------- MAIN PROGRAM --------------------

int main() {
    const int WIDTH = 1100;
    const int HEIGHT = 700;

    const float RABBIT_SPEED = 50.0f;
    const float FOX_SPEED = 75.0f;
    const float HUNT_DISTANCE = 12.0f;
    const float STARVATION_TIME = 1.0f;

    InitWindow(WIDTH, HEIGHT, "Rabbit-Fox Ecosystem");
    SetTargetFPS(60);

    // Initial populations
    std::vector<Rabbit> rabbits;
    std::vector<Fox> foxes;

    for (int i = 0; i < 100; ++i) {
        rabbits.push_back({randomPosition()});
    }

    for (int i = 0; i < 15; ++i) {
        foxes.push_back({randomPosition(), 0.0f});
    }

    // Simulation timers
    float simulationTimer = 0.0f;
    float graphTimer = 0.0f;

    // Population history
    std::vector<int> rabbitHistory;
    std::vector<int> foxHistory;

    bool paused = false;

    // -------------------- GAME LOOP --------------------

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Controls
        if (IsKeyPressed(KEY_SPACE)) {
            paused = !paused;
        }

        if (IsKeyPressed(KEY_R)) {
            rabbits.clear();
            foxes.clear();

            for (int i = 0; i < 100; ++i) {
                rabbits.push_back({randomPosition()});
            }

            for (int i = 0; i < 15; ++i) {
                foxes.push_back({randomPosition(), 0.0f});
            }

            rabbitHistory.clear();
            foxHistory.clear();

            simulationTimer = 0.0f;
            graphTimer = 0.0f;
        }

        // -------------------- SIMULATION --------------------

        if (!paused) {
            simulationTimer += dt;
            graphTimer += dt;

            // 1. Rabbit movement
            for (auto& rabbit : rabbits) {
                rabbit.pos.x +=
                    randomFloat(-1.0f, 1.0f) * RABBIT_SPEED * dt;

                rabbit.pos.y +=
                    randomFloat(-1.0f, 1.0f) * RABBIT_SPEED * dt;

                rabbit.pos.x = Clamp(rabbit.pos.x, 10.0f, 690.0f);
                rabbit.pos.y = Clamp(rabbit.pos.y, 10.0f, 680.0f);
            }

            // 2. Fox movement toward nearest rabbit
            for (auto& fox : foxes) {
                if (rabbits.empty()) {
                    break;
                }

                int nearest = 0;
                float bestDistance = INFINITY;

                for (int i = 0; i < (int)rabbits.size(); ++i) {
                    float distance = Vector2Distance(
                        fox.pos, rabbits[i].pos
                    );

                    if (distance < bestDistance) {
                        bestDistance = distance;
                        nearest = i;
                    }
                }

                Vector2 direction = Vector2Subtract(
                    rabbits[nearest].pos, fox.pos
                );

                float length = Vector2Length(direction);

                if (length > 0.0f) {
                    direction = Vector2Scale(
                        direction, 1.0f / length
                    );

                    fox.pos = Vector2Add(
                        fox.pos,
                        Vector2Scale(direction, FOX_SPEED * dt)
                    );
                }

                fox.pos.x = Clamp(fox.pos.x, 10.0f, 700.0f);
                fox.pos.y = Clamp(fox.pos.y, 10.0f, 690.0f);
            }

            // 3. Hunting
            // Each fox catches at most one rabbit per frame.
            for (auto& fox : foxes) {
                if (rabbits.empty()) {
                    break;
                }

                for (int i = 0; i < (int)rabbits.size(); ++i) {
                    if (Vector2Distance(
                            fox.pos, rabbits[i].pos
                        ) < HUNT_DISTANCE) {

                        rabbits.erase(rabbits.begin() + i);

                        // Eating resets hunger.
                        fox.hunger = 0.0f;
                        break;
                    }
                }
            }

            // 4. Hunger increases over time
            for (auto& fox : foxes) {
                fox.hunger += dt;
            }

            // 5. Population updates every second
            while (simulationTimer >= 1.0f) {
                simulationTimer -= 1.0f;

                // Rabbit reproduction: 20% per step
                int rabbitBirths =
                    static_cast<int>(rabbits.size() * 0.20f);

                for (int i = 0; i < rabbitBirths; ++i) {
                    rabbits.push_back({randomPosition()});
                }

                // Foxes starve if they have not eaten in time
                foxes.erase(
                    std::remove_if(
                        foxes.begin(),
                        foxes.end(),
                        [STARVATION_TIME](const Fox& fox) {
                            return fox.hunger >= STARVATION_TIME;
                        }
                    ),
                    foxes.end()
                );

                // Fed foxes have a chance to reproduce
                int existingFoxes = static_cast<int>(foxes.size());

                for (int i = 0; i < existingFoxes; ++i) {
                    if (foxes[i].hunger < 3.0f &&
                        chance(0.10f)) {

                        foxes.push_back({
                            randomPosition(),
                            0.0f
                        });
                    }
                }
            }

            // 6. Record populations every half second
            if (graphTimer >= 0.5f) {
                graphTimer = 0.0f;

                rabbitHistory.push_back(
                    static_cast<int>(rabbits.size())
                );

                foxHistory.push_back(
                    static_cast<int>(foxes.size())
                );

                // Keep the graph bounded
                if (rabbitHistory.size() > 250) {
                    rabbitHistory.erase(rabbitHistory.begin());
                    foxHistory.erase(foxHistory.begin());
                }
            }
        }

        // -------------------- RENDERING --------------------

        BeginDrawing();
        ClearBackground(Color{25, 30, 38, 255});

        // Ecosystem field
        DrawRectangle(
            0, 0, 710, HEIGHT,
            Color{38, 75, 48, 255}
        );

        // Draw rabbits
        for (const auto& rabbit : rabbits) {
            DrawCircleV(rabbit.pos, 4.0f, RAYWHITE);
        }

        // Draw foxes
        for (const auto& fox : foxes) {
            DrawCircleV(fox.pos, 7.0f, ORANGE);
        }

        // -------------------- POPULATION PANEL --------------------

        DrawText("ECOSYSTEM", 730, 20, 24, RAYWHITE);

        DrawText(
            TextFormat("Rabbits: %d", (int)rabbits.size()),
            730, 65, 20, SKYBLUE
        );

        DrawText(
            TextFormat("Foxes: %d", (int)foxes.size()),
            730, 95, 20, ORANGE
        );

        DrawText("SPACE: Pause / Resume",
                 730, 140, 16, LIGHTGRAY);

        DrawText("R: Reset", 730, 165, 16, LIGHTGRAY);

        DrawText(
            paused ? "PAUSED" : "RUNNING",
            730, 190, 16,
            paused ? YELLOW : GREEN
        );

        // -------------------- POPULATION GRAPH --------------------

        const int gx = 735;
        const int gy = 600;
        const int gw = 330;
        const int gh = 300;

        DrawText(
            "POPULATION HISTORY",
            gx, 220, 18, RAYWHITE
        );

        DrawRectangleLines(gx, 280, gw, gh, GRAY);

        DrawText("Rabbits", gx, 250, 14, SKYBLUE);
        DrawText("Foxes", gx + 100, 250, 14, ORANGE);

        // Find the largest population for graph scaling
        int maxPopulation = 10;

        for (int population : rabbitHistory) {
            maxPopulation = std::max(maxPopulation, population);
        }

        for (int population : foxHistory) {
            maxPopulation = std::max(maxPopulation, population);
        }

        // Draw population curves
        for (int i = 1; i < (int)rabbitHistory.size(); ++i) {
            float x1 = gx + (i - 1) * gw / 250.0f;
            float x2 = gx + i * gw / 250.0f;

            float y1 = gy -
                rabbitHistory[i - 1] * (gh / (float)maxPopulation);

            float y2 = gy -
                rabbitHistory[i] * (gh / (float)maxPopulation);

            DrawLineEx(
                {x1, y1},
                {x2, y2},
                2.0f,
                SKYBLUE
            );

            y1 = gy -
                foxHistory[i - 1] * (gh / (float)maxPopulation);

            y2 = gy -
                foxHistory[i] * (gh / (float)maxPopulation);

            DrawLineEx(
                {x1, y1},
                {x2, y2},
                2.0f,
                ORANGE
            );
        }

        EndDrawing();
    }

    // -------------------- CLEANUP --------------------

    CloseWindow();

    return 0;
}

