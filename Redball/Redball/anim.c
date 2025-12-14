#include "raylib.h"
#include "reasings.h" // Required for easing functions

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [shapes] example - easings circle anim");

    // Circle variables to be animated with easings
    Vector2 position = { (float)screenWidth / 2, -100 };
    float radius = 50.0f;
    float rotation = 0.0f;
    float alpha = 1.0f;

    int state = 0;
    int framesCounter = 0;

    SetTargetFPS(144); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        switch (state)
        {
            case 0: // Move circle down to center of screen
            {
                framesCounter++;

                position.y = EaseElasticOut((float)framesCounter, -100, GetScreenHeight() / 2.0f + 100, 120);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    state = 1;
                }
            } break;
            case 1: // Scale circle to a larger size
            {
                framesCounter++;

                radius = EaseBounceOut((float)framesCounter, 50, 100, 120);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    state = 2;
                }
            } break;
            case 2: // Rotate circle
            {
                framesCounter++;

                rotation = EaseQuadOut((float)framesCounter, 0.0f, 270.0f, 240);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    state = 3;
                }
            } break;
            case 3: // Increase circle size to fill all screen
            {
                framesCounter++;

                radius = EaseCircOut((float)framesCounter, 50, (float)GetScreenWidth() * 0.7f, 120);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    state = 4;
                }
            } break;
            case 4: // Fade out animation
            {
                framesCounter++;

                alpha = EaseSineOut((float)framesCounter, 1.0f, -1.0f, 160);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    state = 5;
                }
            } break;
            default: break;
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircleV(position, radius, Fade(BLACK, alpha));

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
