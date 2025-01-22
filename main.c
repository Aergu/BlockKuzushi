#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>


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

    Rectangle PlayerRectanglePosition = { screenWidth / 2.8f - 25, screenHeight / 1.2f - 1.5f - 25, 450, 30 };
    const float RectangleSpeed = 900.0f;

    Vector2 ballPosition = { PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10 };
    Vector2 ballVelocity = { 0, 0 };
    const float ballRadius = 10.0f;
    const float ballSpeed = 500.0f;
    bool ballLaunched = false;

    bool isPaused = false;
    int pauseMenuOption = 0;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        if (IsKeyPressed(KEY_B)) {
            isPaused = !isPaused;
        }

        if (!isPaused) {
            float deltaTime = GetFrameTime();


            if (IsKeyDown(KEY_RIGHT)) PlayerRectanglePosition.x += RectangleSpeed * deltaTime;
            if (IsKeyDown(KEY_LEFT)) PlayerRectanglePosition.x -= RectangleSpeed * deltaTime;


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


                    float hitPosition = (ballPosition.x - PlayerRectanglePosition.x) / PlayerRectanglePosition.width; // 0.0 (left) to 1.0 (right)
                    ballVelocity.x = (hitPosition - 0.5f) * ballSpeed * 2;
                }
            }


            if (PlayerRectanglePosition.x < 0) PlayerRectanglePosition.x = 0;
            if (PlayerRectanglePosition.x + PlayerRectanglePosition.width > screenWidth)
                PlayerRectanglePosition.x = screenWidth - PlayerRectanglePosition.width;
        } else {
            if (IsKeyPressed(KEY_DOWN)) pauseMenuOption = (pauseMenuOption + 1) % 3;
            if (IsKeyPressed(KEY_UP)) pauseMenuOption = (pauseMenuOption + 3 - 1) % 3;

            if (IsKeyPressed (KEY_ENTER)) {
                switch (pauseMenuOption) {
                    case 0: isPaused = false;
                            break;
                    case 1: ballPosition = (Vector2){PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y - 10};
                            ballVelocity = (Vector2){0, 0};
                            ballLaunched = false;
                            isPaused = false;
                            break;
                    case 2: CloseWindow();
                            return 0;
                }
            }
        }


        BeginDrawing();
        ClearBackground(BLACK);

        if (!isPaused) {
            DrawText("USE LEFT/RIGHT ARROW KEYS TO MOVE THE PLAYER", 350, 750, 20, WHITE);
            DrawRectangleRec(PlayerRectanglePosition, WHITE);
            DrawCircleV(ballPosition, ballRadius, RED);
        }
        else {
            DrawText("PAUSE MENU", screenWidth / 2 - MeasureText("PAUSE MENU", 40) / 2, 200, 40, YELLOW);
            const char *menuOptions[] = {"Resume", "Restart", "Quit"};

            for (int i = 0; i < 3; i++) {
                Color color = (i == pauseMenuOption) ? WHITE : GRAY;
                DrawText(menuOptions[i], screenWidth / 2 - MeasureText(menuOptions[i], 30) / 2, 300 + i * 50, 30, color);

            }
        }

        

        EndDrawing();
    }

    CloseWindow();
}
