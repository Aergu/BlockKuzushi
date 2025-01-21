#include <stdio.h>
#include <raylib.h>

int main() {
    const int screenWidth = 1400;
    const int screenHeight = 800;

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "BlockKuzushi");

    Rectangle PlayerRectanglePosition = { screenWidth/2.8f - 25, screenHeight/1.2f - 1.5f - 25, 450, 30};
    const float RectangleSpeed = 900.0f;
    


    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();
        
        if (IsKeyDown(KEY_RIGHT)) PlayerRectanglePosition.x += RectangleSpeed * deltaTime;
        if (IsKeyDown(KEY_LEFT)) PlayerRectanglePosition.x -= RectangleSpeed * deltaTime;

        if (PlayerRectanglePosition.x < 0) PlayerRectanglePosition.x = 0;
        if (PlayerRectanglePosition.y < 0) PlayerRectanglePosition.y = 0;
        if (PlayerRectanglePosition.x > screenWidth) PlayerRectanglePosition.x = screenWidth;
        if (PlayerRectanglePosition.y > screenHeight) PlayerRectanglePosition.y = screenHeight;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("USE THE LEFT AND RIGHT ARROW KEYS TO MOVE THE PLAYER", 350, 750, 20, WHITE);
        DrawRectangleRec(PlayerRectanglePosition, WHITE);

        EndDrawing();
    }
    CloseWindow();

}