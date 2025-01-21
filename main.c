#include <stdio.h>
#include <raylib.h>
#include <stdlib.h>

int main() {
    const int screenWidth = 1400;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "BlockKuzushi");

    Rectangle PlayerRectanglePosition = { screenWidth/2.8f - 25, screenHeight/1.2f - 1.5f - 25, 450, 30};
    const float RectangleSpeed = 900.0f;

    Vector2 ballPosition = { PlayerRectanglePosition.x + PlayerRectanglePosition.width / 2, PlayerRectanglePosition.y + PlayerRectanglePosition.y - 10 };
    Vector2 ballVelocity = {0, 0};
    const float ballRadius = 10.0f;
    const float ballSpeed = 500.0f;
    bool ballLaunched = false;

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
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
        }
        else {
            ballPosition.x += ballVelocity.x * deltaTime;
            ballPosition.y += ballVelocity.y * deltaTime;

            if (ballPosition.x - ballRadius <= 0 || ballPosition.x + ballRadius >= screenWidth) {
                ballVelocity.x *= -1;
            }
            if (ballPosition.y - ballRadius <= 0 || ballPosition.y + ballRadius >= screenHeight) {
                ballVelocity.y *= -1;
            }
        }

        if (PlayerRectanglePosition.x < 0) PlayerRectanglePosition.x = 0;
        if (PlayerRectanglePosition.x + PlayerRectanglePosition.width > screenWidth)
            PlayerRectanglePosition.x = screenWidth - PlayerRectanglePosition.width;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("USE THE LEFT AND RIGHT ARROW KEYS TO MOVE THE PLAYER", 350, 750, 20, WHITE);
        DrawRectangleRec(PlayerRectanglePosition, WHITE);

        DrawCircleV(ballPosition, ballRadius, RED);

        EndDrawing();
    }
    CloseWindow();

}
