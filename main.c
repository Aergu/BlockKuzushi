#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

// Function to check collision between ball and rectangle
bool CheckCollisionBallRect(Vector2 ballPosition, float ballRadius, Rectangle rect) {
    return ballPosition.y + ballRadius >= rect.y &&
           ballPosition.y - ballRadius <= rect.y + rect.height &&
           ballPosition.x + ballRadius >= rect.x &&
           ballPosition.x - ballRadius <= rect.x + rect.width;
}

int main() {
    const int screenWidth = 1400;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "BlockKuzushi");

    // Player and ball properties
    Rectangle PlayerRectanglePosition = { screenWidth / 2.8f - 25, screenHeight / 1.2f - 1.5f - 25, 450, 30 };
    const float RectangleSpeed = 900.0f;

    Vector2 ballPosition = { PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
    Vector2 ballVelocity = { 0, 0 };
    const float ballRadius = 10.0f;
    const float ballSpeed = 500.0f;
    bool ballLaunched = false;

    // States
    bool isPaused = false;  // Pause state
    bool isGameOver = false;  // Game Over state
    int pauseMenuOption = 0;  // Pause menu selection

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // Toggle pause
        if (IsKeyPressed(KEY_B) && !isGameOver) {
            isPaused = !isPaused;
        }

        if (isGameOver) {
            // Game Over logic
            if (IsKeyPressed(KEY_ENTER)) {
                // Restart game
                ballPosition = (Vector2){ PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
                ballVelocity = (Vector2){ 0, 0 };
                ballLaunched = false;
                isGameOver = false;
                isPaused = false;
            }
        } else if (isPaused) {
            // Pause menu logic
            if (IsKeyPressed(KEY_DOWN)) pauseMenuOption = (pauseMenuOption + 1) % 3;
            if (IsKeyPressed(KEY_UP)) pauseMenuOption = (pauseMenuOption + 3 - 1) % 3;

            if (IsKeyPressed(KEY_ENTER)) {
                switch (pauseMenuOption) {
                    case 0:  // Resume
                        isPaused = false;
                        break;
                    case 1:  // Restart
                        ballPosition = (Vector2){ PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
                        ballVelocity = (Vector2){ 0, 0 };
                        ballLaunched = false;
                        isPaused = false;
                        break;
                    case 2:  // Quit
                        CloseWindow();
                        return 0;
                }
            }
        } else {
            // Main game logic
            float deltaTime = GetFrameTime();

            // Player movement
            if (IsKeyDown(KEY_RIGHT)) PlayerRectanglePosition.x += RectangleSpeed * deltaTime;
            if (IsKeyDown(KEY_LEFT)) PlayerRectanglePosition.x -= RectangleSpeed * deltaTime;

            // Clamp player rectangle to screen
            if (PlayerRectanglePosition.x < 0) PlayerRectanglePosition.x = 0;
            if (PlayerRectanglePosition.x + PlayerRectanglePosition.width > screenWidth)
                PlayerRectanglePosition.x = screenWidth - PlayerRectanglePosition.width;

            // Ball logic
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

                // Ball collision with screen edges
                if (ballPosition.x - ballRadius <= 0 || ballPosition.x + ballRadius >= screenWidth) {
                    ballVelocity.x *= -1;
                }
                if (ballPosition.y - ballRadius <= 0) {
                    ballVelocity.y *= -1;
                }

                // Ball collision with player rectangle
                if (CheckCollisionBallRect(ballPosition, ballRadius, PlayerRectanglePosition)) {
                    ballVelocity.y *= -1;

                    // Adjust horizontal velocity based on where the ball hits the paddle
                    float hitPosition = (ballPosition.x - PlayerRectanglePosition.x) / PlayerRectanglePosition.width;  // 0.0 (left) to 1.0 (right)
                    ballVelocity.x = (hitPosition - 0.5f) * ballSpeed * 2;
                }

                // Game Over condition
                if (ballPosition.y - ballRadius > screenHeight) {
                    isGameOver = true;
                }
            }
        }

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);

        if (isGameOver) {
            // Game Over screen
            DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 50) / 2, screenHeight / 2 - 100, 50, RED);
            DrawText("PRESS ENTER TO RESTART", screenWidth / 2 - MeasureText("PRESS ENTER TO RESTART", 20) / 2, screenHeight / 2, 20, WHITE);
        } else if (isPaused) {
            // Pause menu
            DrawText("PAUSE MENU", screenWidth / 2 - MeasureText("PAUSE MENU", 40) / 2, 200, 40, YELLOW);
            const char *menuOptions[] = { "Resume", "Restart", "Quit" };

            for (int i = 0; i < 3; i++) {
                Color color = (i == pauseMenuOption) ? WHITE : GRAY;
                DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 30) / 2, 300 + i * 50, 30, color);
            }
        } else {
            // Game elements
            DrawText("USE LEFT/RIGHT ARROW KEYS TO MOVE THE PLAYER", 350, 750, 20, WHITE);
            DrawRectangleRec(PlayerRectanglePosition, WHITE);
            DrawCircleV(ballPosition, ballRadius, RED);
        }

        EndDrawing();
    }

    CloseWindow();
}
