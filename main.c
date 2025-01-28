#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define BLOCK_ROWS 5
#define BLOCK_COLUMNS 10
#define BLOCK_HEIGHT 30
#define BONUS_DURATION 10.0f

bool CheckCollisionBallRect(Vector2 ballPosition, float ballRadius, Rectangle rect) {
    return ballPosition.y + ballRadius >= rect.y &&
           ballPosition.y - ballRadius <= rect.y + rect.height &&
           ballPosition.x + ballRadius >= rect.x &&
           ballPosition.x - ballRadius <= rect.x + rect.width;
}

bool AreAllBlocksCleared(bool blockVisible[BLOCK_ROWS][BLOCK_COLUMNS]) {
    for (int row = 0; row < BLOCK_ROWS; row++) {
        for (int col = 0; col < BLOCK_COLUMNS; col++) {
            if (blockVisible[row][col]) {
                return false;
            }
        }
    }
    return true;
}

int main() {
    const int screenWidth = 1400;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "BlockKuzushi");

    Rectangle PlayerRectanglePosition = { screenWidth / 2.8f - 25, screenHeight / 1.2f - 1.5f - 25, 450, 30 };
    const float RectangleSpeed = 1800.0f;

    Vector2 ballPosition = { PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
    Vector2 ballVelocity = { 0, 0 };
    const float ballRadius = 10.0f;
    const float ballSpeed = 900.0f;
    bool ballLaunched = false;

    Rectangle blocks[BLOCK_ROWS][BLOCK_COLUMNS];
    bool blockVisible[BLOCK_ROWS][BLOCK_COLUMNS] = { 0 };
    int blockPoints[BLOCK_ROWS][BLOCK_COLUMNS];
    Color blockColors[BLOCK_ROWS][BLOCK_COLUMNS];

    float totalBlockWidth = screenWidth - 20;
    float blockWidth = totalBlockWidth / BLOCK_COLUMNS;
    float startX = 10;


    srand(time(NULL));

    for (int row = 0; row < BLOCK_ROWS; row++) {
        for (int col = 0; col < BLOCK_COLUMNS; col++) {
            blocks[row][col] = (Rectangle){
                startX + col * blockWidth,
                row * (BLOCK_HEIGHT + 10) + 10,
                blockWidth - 5,
                BLOCK_HEIGHT
            };
            blockVisible[row][col] = true;
            blockPoints[row][col] = (BLOCK_ROWS - row) * 100;

            if (blockPoints[row][col] <= 100) {
                blockColors[row][col] = BLUE;
            } else if (blockPoints[row][col] <= 200) {
                blockColors[row][col] = GREEN;
            } else if (blockPoints[row][col] <= 300) {
                blockColors[row][col] = YELLOW;
            } else if (blockPoints[row][col] <= 400) {
                blockColors[row][col] = ORANGE;
            } else {
                blockColors[row][col] = RED;
            }
        }
    }

    int score = 0;
    float pointMultiplier = 1.0f;
    float bonusTimer = 0.0f;
    bool isPaused = false;
    bool isGameOver = false;
    bool isVictory = false;
    int pauseMenuOption = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_B) && !isGameOver && !isVictory) {
            isPaused = !isPaused;
        }

        if (isPaused) {
            if (IsKeyPressed(KEY_UP)) pauseMenuOption = (pauseMenuOption + 3 - 1) % 3;
            if (IsKeyPressed(KEY_DOWN)) pauseMenuOption = (pauseMenuOption + 1) % 3;

            if (IsKeyPressed(KEY_ENTER)) {
                switch (pauseMenuOption) {
                    case 0:
                        isPaused = false;
                        break;
                    case 1:
                        ballPosition = (Vector2){PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10};
                        ballVelocity = (Vector2){0, 0 };
                        ballLaunched = false;
                        isPaused = false;
                        score = 0;
                        bonusTimer = 0.0f;
                        pointMultiplier = 1.0f;

                        for (int row = 0; row < BLOCK_ROWS; row++) {
                            for (int col = 0; col < BLOCK_COLUMNS; col++) {
                                blockVisible[row][col] = true;
                            }
                        }
                        break;
                    case 2:
                        CloseWindow();
                        return 0;
                }
            }
        }

        if (bonusTimer > 0.0f) {
            bonusTimer -= deltaTime;
            if (bonusTimer <= 0.0f) {
                pointMultiplier = 1.0f;
            }
        }
        if (isGameOver) {
            if (IsKeyPressed(KEY_SPACE)) {
                ballPosition = (Vector2){ PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
                ballVelocity = (Vector2){ 0, 0 };
                ballLaunched = false;
                isGameOver = false;
                isPaused = false;
                isVictory = false;
                score = 0;
                bonusTimer = 0.0f;
                pointMultiplier = 1.0f;

                for (int row = 0; row < BLOCK_ROWS; row++) {
                    for (int col = 0; col < BLOCK_COLUMNS; col++) {
                        blockVisible[row][col] = true;
                    }
                }
            }
        } else if (isVictory) {
            if (IsKeyPressed(KEY_SPACE)) {
                ballPosition = (Vector2){ PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
                ballVelocity = (Vector2){ 0, 0 };
                ballLaunched = false;
                isVictory = false;
                isPaused = false;
                score = 0;
                bonusTimer = 0.0f;
                pointMultiplier = 1.0f;

                for (int row = 0; row < BLOCK_ROWS; row++) {
                    for (int col = 0; col < BLOCK_COLUMNS; col++) {
                        blockVisible[row][col] = true;
                    }
                }
            }
        } else if (!isPaused) {
            if (IsKeyDown(KEY_RIGHT)) PlayerRectanglePosition.x += RectangleSpeed * deltaTime;
            if (IsKeyDown(KEY_LEFT)) PlayerRectanglePosition.x -= RectangleSpeed * deltaTime;

            if (PlayerRectanglePosition.x < 0) PlayerRectanglePosition.x = 0;
            if (PlayerRectanglePosition.x + PlayerRectanglePosition.width > screenWidth)
                PlayerRectanglePosition.x = screenWidth - PlayerRectanglePosition.width;

            if (!ballLaunched) {
                ballPosition.x = PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2;
                ballPosition.y = PlayerRectanglePosition.y - ballRadius - 5;

                if (IsKeyPressed(KEY_SPACE)) {
                    ballLaunched = true;
                    ballVelocity.x = (rand() % 2 == 0 ? -1 : 1) * ballSpeed;
                    ballVelocity.y = -ballSpeed;
                }
            } else {
                ballPosition.x += ballVelocity.x * deltaTime;
                ballPosition.y += ballVelocity.y * deltaTime;

                if (ballPosition.x - ballRadius <= 0 || ballPosition.x + ballRadius >= screenWidth) {
                    ballVelocity.x *= -1;
                }
                if (ballPosition.y - ballRadius <= 0) {
                    ballVelocity.y *= -1;
                }

                if (CheckCollisionBallRect(ballPosition, ballRadius, PlayerRectanglePosition)) {
                    ballVelocity.y *= -1;

                    float hitPosition = (ballPosition.x - PlayerRectanglePosition.x) / PlayerRectanglePosition.width;
                    ballVelocity.x = (hitPosition - 0.5f) * ballSpeed * 2;
                }

                for (int row = 0; row < BLOCK_ROWS; row++) {
                    for (int col = 0; col < BLOCK_COLUMNS; col++) {
                        if (blockVisible[row][col] &&
                            CheckCollisionBallRect(ballPosition, ballRadius, blocks[row][col])) {
                            ballVelocity.y *= -1;

                            score += blockPoints[row][col] * pointMultiplier;

                            if ((rand() % 100) < 10 && bonusTimer <= 0.0f) {
                                pointMultiplier = 2.0f; // Double points
                                bonusTimer = BONUS_DURATION;
                            }

                            blockVisible[row][col] = false;
                        }
                    }
                }

                if (AreAllBlocksCleared(blockVisible)) {
                    isVictory = true;
                }

                if (ballPosition.y - ballRadius > screenHeight) {
                    isGameOver = true;
                }
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);

        if (isGameOver) {
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 50) / 2, screenHeight / 2 - 100, 50, RED);
            DrawText("PRESS SPACE TO RESTART", screenWidth / 2 - MeasureText("PRESS SPACE TO RESTART", 20) / 2, screenHeight / 2, 20, WHITE);
            DrawText(TextFormat("FINAL SCORE: %d", score), screenWidth / 2 - MeasureText(TextFormat("FINAL SCORE: %d", score), 20) / 2, screenHeight / 2 + 50, 20, WHITE);
        } else if (isVictory) {
            DrawText("YOU WIN!", screenWidth / 2 - MeasureText("YOU WIN!", 50) / 2, screenHeight / 2 - 100, 50, GREEN);
            DrawText("PRESS SPACE TO PLAY AGAIN", screenWidth / 2 - MeasureText("PRESS SPACE TO PLAY AGAIN", 20) / 2, screenHeight / 2, 20, WHITE);
            DrawText(TextFormat("FINAL SCORE: %d", score), screenWidth / 2 - MeasureText(TextFormat("FINAL SCORE: %d", score), 20) / 2, screenHeight / 2 + 50, 20, WHITE);
        } else if (isPaused) {
            DrawText("PAUSE MENU", screenWidth / 2 - MeasureText("PAUSE MENU", 40) / 2, 200, 40, YELLOW);
            const char *menuOptions[] = { "Resume", "Restart", "Quit" };

            for (int i = 0; i < 3; i++) {
                Color color = (i == pauseMenuOption) ? WHITE : GRAY;
                DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 30) / 2, 300 + i * 50, 30, color);
            }

        } else {
            DrawText(TextFormat("SCORE: %d", score), 660, 700, 20, WHITE);

            if (bonusTimer > 0.0f) {
                DrawText("BONUS ACTIVE!", screenWidth / 2 - MeasureText("BONUS ACTIVE!", 30) / 2, 600, 30, YELLOW);
            }

            DrawRectangleRec(PlayerRectanglePosition, WHITE);
            DrawCircleV(ballPosition, ballRadius, RED);

            for (int row = 0; row < BLOCK_ROWS; row++) {
                for (int col = 0; col < BLOCK_COLUMNS; col++) {
                    if (blockVisible[row][col]) {
                        DrawRectangleRec(blocks[row][col], blockColors[row][col]);

                        int displayPoints = blockPoints[row][col];
                        if (bonusTimer > 0.0f) {
                            displayPoints *= (int)pointMultiplier;
                        }
                        const char *pointsText = TextFormat("%d", displayPoints);
                        int textWidth = MeasureText(pointsText, 10);
                        DrawText(pointsText, blocks[row][col].x + (blocks[row][col].width - textWidth) / 2,
                                blocks[row][col].y + (blocks[row][col].height - textWidth) / 2, 10, BLACK);
                    }
                }
            }
        }

        EndDrawing();
    }

    CloseWindow();
}
