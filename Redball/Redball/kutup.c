#include "raylib.h"
#include <math.h>
#include "reasings.h"

#define G 400
#define PLAYER_JUMP_SPD 350.0f
#define PLAYER_HOR_SPD 200.0f
#define SCROLL_SPEED 50.0f
#define BACKGROUND_MOVE_SPEED 5.0f 

#define OBSTACLE_WIDTH 80
#define OBSTACLE_HEIGHT 20

#define NUM_FRAMES 3 

// Fluid background movement t = time, b = beginning, c = change in value, d = duration
float easeInOutQuad(float t, float b, float c, float d) {
    t /= d / 2;
    if (t < 1) return c / 2 * t * t + b;
    t--;
    return -c / 2 * (t * (t - 2) - 1) + b;
}
typedef enum GameScreen {GAMEPLAY, PAUSE, MAIN_MENU, LOGO, ANIM, HOW_TO_PLAY, CREDITS} GameScreen;

typedef struct Player {
    Vector2 position;
    float speed;
    bool canJump;
    float rotationAngle;
} Player;

typedef struct Obstacle {
    Rectangle rect;
    Color color;
} Obstacle;

int main(void)
{
    // Initialization
    const int screenWidth = 1600;
    const int screenHeight = 650;

    InitWindow(screenWidth, screenHeight, "Redball");
    InitAudioDevice();

    float elapsedTime = 0.0f; //Cronometer
    int starsEarned = 0;
    
    GameScreen currentScreen = MAIN_MENU;
 
    Texture2D SPACE = LoadTexture("SPACE.png");
    Texture2D ARROWRIGHT = LoadTexture("ARROWRIGHT.png");
    Texture2D ARROWLEFT = LoadTexture("ARROWLEFT.png");
    Texture2D howtoplaybackground = LoadTexture("howtoplaybackground.png"); 
    Texture2D creditsbackground = LoadTexture("creditsbackground.png"); 
    Texture2D startbutton = LoadTexture("startbutton.png");
    Texture2D exitbutton = LoadTexture("exitbutton.png");
    Texture2D howtoplaybutton = LoadTexture("howtoplaybutton.png");
    Texture2D creditsbutton = LoadTexture("creditsbutton.png");
    Texture2D background = LoadTexture("background.png");
    Texture2D redball = LoadTexture("redball.png");
    Sound gameS = LoadSound("gameplay.mp3");
    Sound menuS = LoadSound("menu.mp3");
    
      
    float frameHeight = (float)startbutton.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)startbutton.width, frameHeight };
    
    float frameHeight2 = (float)howtoplaybutton.height/NUM_FRAMES;
    Rectangle sourceRec2 = { 0, 0, (float)howtoplaybutton.width, frameHeight2 };
    
    float frameHeight3 = (float)creditsbutton.height/NUM_FRAMES;
    Rectangle sourceRec3 = { 0, 0, (float)creditsbutton.width, frameHeight3 };
    
     float frameHeight4 = (float)exitbutton.height/NUM_FRAMES;
    Rectangle sourceRec4 = { 0, 0, (float)exitbutton.width, frameHeight4 };
    
    Rectangle btnBounds = { screenWidth/2.0f - startbutton.width/2.0f, screenHeight/2.0f - startbutton.height/2, (float)startbutton.width, frameHeight };
    Rectangle btnBounds2 = { screenWidth/2.0f - howtoplaybutton.width/2.0f, screenHeight/2.0f - howtoplaybutton.height/6, (float)howtoplaybutton.width, frameHeight2 };
    Rectangle btnBounds3 = { screenWidth/2.0f - creditsbutton.width/2.0f, screenHeight/2.0f + creditsbutton.height/6 , (float)creditsbutton.width, frameHeight3 };
    Rectangle btnBounds4 = { screenWidth/2.0f - exitbutton.width/2.0f, screenHeight/2.0f + exitbutton.height/2, (float)exitbutton.width, frameHeight4 };

    int btnState = 0;               
    bool btnAction = false;  
    
     int btnState2 = 0;               
    bool btnAction2 = false;  
    
     int btnState3 = 0;               
    bool btnAction3 = false;  
    
     int btnState4 = 0;               
    bool btnAction4 = false;  
    
    
    Vector2 mousePoint = { 0.0f, 0.0f };
    Vector2 mousePoint2 = { 0.0f, 0.0f }; 
    Vector2 mousePoint3 = { 0.0f, 0.0f };
    Vector2 mousePoint4 = { 0.0f, 0.0f };
    
    
    
    
    // Adjust the volume of the loaded sounds
    SetSoundVolume(gameS, 0.1f); 
    SetSoundVolume(menuS, 0.1f); 
    
    Player player = { 0 };
    player.position = (Vector2){ 300, 1370 }; // Adjusted player starting position
    player.speed = 0;
    player.canJump = false;
    player.rotationAngle = 0;

    float backgroundX = -1005.0f;
    float backgroundY = 0.0f; 
    
    float creditsbackgroundX = -1000.0f;
    float creditsbackgroundY = 0.0f; 
    
    float howtoplaybackgroundX = -1000.0f;
    float howtoplaybackgroundY = 0.0f; 

    
    bool jumping = false;

    // Invisible wall
    Obstacle wall = {{-800, 0, 20, screenHeight}, BLANK}; 
    
 
   
         Obstacle obstacles[] = {
        {{ 465, 1220, 120, OBSTACLE_HEIGHT }, BLANK },
        {{ 500, 1000, OBSTACLE_WIDTH, OBSTACLE_HEIGHT }, BLACK }
    };
    int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);
    
    // Circle variables to be animated with easings
    Vector2 position = { (float)screenWidth / 2, -100 };
    float radius = 50.0f;
    float rotation = 0.0f;
    
    // Circle animation variables
    Vector2 circlePosition = { (float)screenWidth / 2, -100 };
    float circleRadius = 50.0f;
    float circleRotation = 0.0f;
    float circleAlpha = 1.0f;
    int circleFramesCounter = 0;
    int animState = 0;
 
    Vector2 ballPosition = { GetScreenWidth()/2.0f, GetScreenHeight()/2.0f };
    Vector2 ballSpeed = { 5.0f, 4.0f };
    int ballRadius = 20;

    bool pause = false;
    
 
    SetTargetFPS(144);

    // Logo animation variables
    int state = 0;
    int framesCounter = 0;
    int topSideRecWidth = 16;
    int leftSideRecHeight = 16;
    int bottomSideRecWidth = 16;
    int rightSideRecHeight = 16;
    int lettersCount = 0;
    float alpha = 1.0f;
    int logoPositionX = (screenWidth - 256) / 2; // Center the logo horizontally
    int logoPositionY = (screenHeight - 256) / 2; // Center the logo vertically
    int blinkDelay = 10; // blinking speed of the black boxes

    //Camera
    Camera2D camera = { 0 };
    camera.target = player.position;
    camera.offset = (Vector2){ screenWidth/2.0f, screenHeight/2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 0.8f;

    // Main game loop
    while (!WindowShouldClose())
    {
        
         switch(currentScreen)
        {   
        case LOGO: {
    // Update animation frames and state transitions
    if (state == 0) {
        // Update animation frames for state 0
        framesCounter++;
        if (framesCounter == 120) {
            state = 1;
            framesCounter = 0; // Reset counter
        }
        
    } else if (state == 1) {
        // Update animation frames for state 1
        topSideRecWidth += 4;
        leftSideRecHeight += 4;
        
        framesCounter++;
        if (framesCounter % blinkDelay == 0 && topSideRecWidth < 256) {
            topSideRecWidth += 4;
            leftSideRecHeight += 4;
        }

        
        if (topSideRecWidth == 256) state = 2;
               
        } else if (state == 2) {
        // Update animation frames for state 2
        bottomSideRecWidth += 4;
        rightSideRecHeight += 4;
        
        framesCounter++;
        
        if (framesCounter % blinkDelay == 0 && bottomSideRecWidth < 256) {
            bottomSideRecWidth += 4;
            rightSideRecHeight += 4;
        }
        
        if (bottomSideRecWidth == 256) state = 3;
                
        } else if (state == 3) {
        // Update animation frames for state 3
        framesCounter++;
            if (framesCounter % 12 == 0) {
            lettersCount++;
            
            }
                if (lettersCount >= 10) {
                alpha -= 0.02f;
                    if (alpha <= 0.0f) {
                    alpha = 0.0f;
                    // Transition directly to ANIM after logo animation ends
                    currentScreen = ANIM;
                    }
                }
        }

    // Draw logo animation
    BeginDrawing();
    ClearBackground(WHITE);

    // Draw logo based on current state
    if (state == 0) {
        // Draw small box blinking
        if ((framesCounter / blinkDelay) % 2 == 0) {
    DrawRectangle(logoPositionX, logoPositionY, 16, 16, Fade(BLACK, alpha+10)); 
    }

    } else if (state == 1) {
        // Draw top and left bars growing
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
    } else if (state == 2) {
        // Draw bottom and right bars growing
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, BLACK);
        DrawRectangle(logoPositionX, logoPositionY, 16, leftSideRecHeight, BLACK);
        DrawRectangle(logoPositionX + 240, logoPositionY, 16, rightSideRecHeight, BLACK);
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, BLACK);
    } else if (state == 3) {
        // Draw letters appearing and fading out
        DrawRectangle(logoPositionX, logoPositionY, topSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 16, 16, leftSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX + 240, logoPositionY + 16, 16, rightSideRecHeight - 32, Fade(BLACK, alpha));
        DrawRectangle(logoPositionX, logoPositionY + 240, bottomSideRecWidth, 16, Fade(BLACK, alpha));
        DrawRectangle(GetScreenWidth() / 2 - 112, GetScreenHeight() / 2 - 112, 224, 224, Fade(WHITE, alpha));
        DrawText(TextSubtext("raylib", 0, lettersCount), GetScreenWidth() / 2 - 44, GetScreenHeight() / 2 + 48, 50, Fade(BLACK, alpha));
    }

        EndDrawing();
    } break;     
    
            case ANIM:
            {     
            
                // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircleV(position, radius, Fade(BLACK, alpha));   

           
        
                 // Update
                switch (animState)
                {
                    case 0: // Move circle down to center of screen
                    {
                        framesCounter++;

                position.y = EaseElasticOut((float)framesCounter, -100, GetScreenHeight() / 2.0f + 100, 120);

                if (framesCounter >= 240)
                {
                    framesCounter = 0;
                    animState = 1;
                }
            } break;
            case 1: // Scale circle to a larger size
            {
                framesCounter++;

                radius = EaseBounceOut((float)framesCounter, 50, 100, 120);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    animState = 2;
                }
            } break;
            case 2: // Rotate circle
            {
                framesCounter++;

                rotation = EaseQuadOut((float)framesCounter, 0.0f, 270.0f, 240);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    animState = 3;
                }
            } break;
            case 3: // Increase circle size to fill all screen
            {
                framesCounter++;

                radius = EaseCircOut((float)framesCounter, 50, (float)GetScreenWidth() * 0.7f, 120);

                if (framesCounter >= 120)
                {
                    framesCounter = 0;
                    animState = 4;
                }
            } break;
            case 4: // Fade out animation
            {
                framesCounter++;

                alpha = EaseSineOut((float)framesCounter, 1.0f, -1.0f, 160);

                if (framesCounter >= 160)
                {
                    framesCounter = 0;
                    animState = 5;
                }
            } break;
            default: break;
        }
        
        EndDrawing(); 
        
        
        if (animState == 5){
        // Transition to MAIN_MENU after animation is complete
        currentScreen = MAIN_MENU;
        }
        
            }
       
            case GAMEPLAY:
            {                             
                // Press enter to change to GAMEPLAY screen
                if (IsKeyPressed(KEY_P))
                {
                    currentScreen = PAUSE;
                }
            } break;
            case PAUSE:
            {               
               // Press enter to change to ENDING screen
                if (IsKeyPressed(KEY_P))
                {
                    currentScreen = GAMEPLAY;
                }
                
                if (IsKeyPressed(KEY_SPACE))
                {
                    currentScreen = MAIN_MENU;
                }   
           
            } break;
            case HOW_TO_PLAY:
            {
            
              if(IsKeyPressed(KEY_SPACE))
               { 
                currentScreen = MAIN_MENU;
               }
             
            }break;
            case CREDITS:
            {
            
                            
              if(IsKeyPressed(KEY_SPACE))
               { 
                currentScreen = MAIN_MENU;
               }
             
            }break;
            case MAIN_MENU:
            {
                
                  mousePoint = GetMousePosition();
                   btnAction = false;

                   mousePoint2 = GetMousePosition();
                   btnAction2 = false;
                   
                   mousePoint3 = GetMousePosition();
                   btnAction3 = false;
                   
                   mousePoint4 = GetMousePosition();
                   btnAction4 = false;

            if (CheckCollisionPointRec(mousePoint, btnBounds))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
                        else btnState = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
                    }
                    else btnState = 0;

                    if (btnAction)
                    {
                        currentScreen = GAMEPLAY;
                    }
                    sourceRec.y = btnState*frameHeight;
            if (CheckCollisionPointRec(mousePoint2, btnBounds2))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState2 = 2;
                        else btnState2 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction2 = true;
                    }
                    else btnState2 = 0;

                    if (btnAction2)
                    {
                        currentScreen = HOW_TO_PLAY;
                   }
    
                    sourceRec2.y = btnState2*frameHeight;
                    
           if (CheckCollisionPointRec(mousePoint3, btnBounds3))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState3 = 2;
                        else btnState3 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction3 = true;
                    }
                    else btnState3 = 0;
                                       
                    if (btnAction3)
                    {
                        currentScreen = CREDITS;
                    }
                                  
                 sourceRec3.y = btnState3*frameHeight;
                 
                 
            if (CheckCollisionPointRec(mousePoint4, btnBounds4))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState4 = 2;
                        else btnState4 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction4 = true;
                    }
                    else btnState4 = 0;

                    if (btnAction4)
                    {
                        CloseWindow();
                    }
                    sourceRec4.y = btnState4*frameHeight;            
            }break;    
            default: break;
        }
        
        // Update
        float deltaTime = GetFrameTime();
        
      // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

           switch(currentScreen)
            {
                case GAMEPLAY:
                {
                           // Draw scrolling background with adjusted Y offset
                          BeginMode2D(camera);
                          
                          StopSound(menuS);
                          
                          if (!IsSoundPlaying(gameS))
                          {
                            PlaySound(gameS);
                          }
                          
                           // Player movement
                if (IsKeyDown(KEY_LEFT) && player.position.x > 50)
                {
                    player.position.x -= PLAYER_HOR_SPD * deltaTime;
                   // backgroundX += SCROLL_SPEED * deltaTime;
                    player.rotationAngle -= PLAYER_HOR_SPD * deltaTime * 3; // Rotate left
                }
                if (IsKeyDown(KEY_RIGHT))
                {
                player.position.x += PLAYER_HOR_SPD * deltaTime;
              //  backgroundX -= SCROLL_SPEED * deltaTime;
                player.rotationAngle += PLAYER_HOR_SPD * deltaTime * 3; // Rotate right
                }

        // Player jumping
        if (IsKeyDown(KEY_SPACE) && player.canJump)
        {
            player.speed = -PLAYER_JUMP_SPD;
            player.canJump = false;
            jumping = true;
        }

           // Update background Y offset based on jumping state
      /*    if (jumping)
           {
            backgroundY -= BACKGROUND_MOVE_SPEED * deltaTime; // Move background upwards
            backgroundY = fmaxf(backgroundY, -background.height + screenHeight); // Clamp background Y offset
           
        }
              else
                  
              
        {
            if (player.position.y < 580)
            {
                backgroundY += BACKGROUND_MOVE_SPEED * deltaTime; // Move background downwards when player lands
                backgroundY = fminf(backgroundY, 0.0f); // Clamp background Y offset
            }
            else
            {
                backgroundY = 0.0f; // Reset background Y offset
            }
        }

*/

        // Player falling
        player.position.y += player.speed * deltaTime;
        player.speed += G * deltaTime;

        // Player collision with ground
        if (player.position.y >= 1370)
        {
            player.position.y = 1370;
            player.speed = 0;
            player.canJump = true;
            jumping = false;
            backgroundY = 0.0f; // Reset background Y offset upon landing
        }

        // Check collision with obstacles
        for (int i = 0; i < numObstacles; i++)
        {
            // Check if the player rectangle collides with the obstacle rectangle
            if (CheckCollisionRecs((Rectangle){ player.position.x - redball.width / 2, player.position.y - redball.height / 2, redball.width, redball.height },
                           obstacles[i].rect))
            {
            // Handle collision based on the player's position and speed
            if (player.position.y < obstacles[i].rect.y && player.speed > 0)
                {
                    player.position.y = obstacles[i].rect.y - redball.height / 2;
                    player.speed = 0;
                    player.canJump = true;
                    jumping = false;
                    backgroundY = easeInOutQuad(player.position.y - 580, -50.0f, 50.0f, PLAYER_JUMP_SPD * 2); // Adjust duration for smoother return
                }
            }
        }
                      // Camera update
                            camera.target = player.position;                             
                        
                        DrawTextureEx(background, (Vector2){ backgroundX, backgroundY }, 0.0f, 1.0f, WHITE);
                                                                          
                          
                         // Draw obstacles
                          for (int i = 0; i < numObstacles; i++)
                           {
                              DrawRectangleRec(obstacles[i].rect, obstacles[i].color);
                           }

                              // Draw invisible wall
                            DrawRectangleRec(wall.rect, wall.color);

                        // Draw player
                        Vector2 origin = { redball.width / 2, redball.height / 2 };
                        DrawTexturePro(redball, (Rectangle){ 0, 0, redball.width, redball.height },
                            (Rectangle){ player.position.x, player.position.y, redball.width, redball.height },
                            origin, player.rotationAngle, WHITE);
               
                        elapsedTime += GetFrameTime();
                        
                        // Convert elapsed time to minutes and seconds
                        int minutes = (int)(elapsedTime / 60);
                        int seconds = (int)(elapsedTime) % 60;

                        // Draw timer text
                        DrawText(TextFormat("Time: %02d:%02d", minutes, seconds), player.position.x + 750, player.position.y - 380, 40, WHITE);
                        
                        if (elapsedTime <= 5){
                            starsEarned = 3;
                            
                        } else if (elapsedTime <= 10){
                        starsEarned = 2;
                        } else if (elapsedTime <= 15) {
                        starsEarned = 1;
                        } else {
                         starsEarned = 0; // No stars earned if time exceeds 3 minutes
                        }           
                        
                        for (int i = 0; i < 3; i++) {
                        if (i < starsEarned) {
                        
                        DrawCircle(player.position.x + i * 55 - 950, player.position.y - 360, 25, PURPLE); // Draw purple ball
                        } else {
                            DrawCircle(player.position.x + i * 55 - 950, player.position.y - 360, 25, GRAY); // Draw grayed-out ball
                            }
                        }
               
               } break;
               case HOW_TO_PLAY:
               {
                   
                    if (!IsSoundPlaying(menuS))
                {
                  PlaySound(menuS);
                }
                  
                    // Draw moving background with zoom out
                DrawTextureEx(howtoplaybackground, (Vector2){ howtoplaybackgroundX + 500, howtoplaybackgroundY }, 0.0f, 0.8f, WHITE);
                DrawTextureEx(howtoplaybackground, (Vector2){ howtoplaybackgroundX + howtoplaybackground.width * 1.0f, howtoplaybackgroundY }, 0.0f, 0.8f, WHITE);
    
                // Update background X offset for movement
                howtoplaybackgroundX -= BACKGROUND_MOVE_SPEED * GetFrameTime();
    
                // Reset background X offset when it goes off-screen
                if (howtoplaybackgroundX <= -howtoplaybackground.width)
                {
                    howtoplaybackgroundX = 0;
                }
                
                DrawText("HOW TO PLAY",700,50,30,GRAY);
                DrawText("Press LEFT to move left",500,120,30,GRAY);
                DrawText("Press RIGHT to move right",500,270,30,GRAY);
                DrawText("Press SPACE to jump",500,420,30,GRAY);
                DrawText("Press space to return to main menu",1200,620,20,GRAY);
             
                   
                DrawTexture(ARROWLEFT,1000,100,GRAY);
                DrawTexture(ARROWRIGHT,1000,250,GRAY);
                DrawTexture(SPACE,1000,400,GRAY);
                    
               }break;
                case PAUSE:
{
    // Draw the gameplay background without any adjustments
    BeginMode2D(camera);
    DrawTextureEx(background, (Vector2){ backgroundX, backgroundY }, 0.0f, 0.8f, WHITE);
    
    // Draw the player at its current position
    Vector2 origin = { redball.width / 2, redball.height / 2 };
    DrawTexturePro(redball, (Rectangle){ 0, 0, redball.width, redball.height },
                   (Rectangle){ player.position.x, player.position.y, redball.width, redball.height },
                   origin, player.rotationAngle, WHITE);

    // Draw the pause screen elements
    DrawText("PAUSE SCREEN", screenWidth/2 - MeasureText("PAUSE SCREEN", 40)/2, screenHeight/2 - 20, 40, DARKBLUE);
    DrawText("PRESS P TO RETURN TO GAMEPLAY SCREEN", 120, 220, 20, DARKBLUE);

    EndMode2D(); // End the 2D mode

    // Set camera target to player position to freeze it
    camera.target = player.position;
} break;





                case CREDITS:
                {
                  
                  if (!IsSoundPlaying(menuS))
                {
                  PlaySound(menuS);
                }
                  
                    // Draw moving background with zoom out
                DrawTextureEx(creditsbackground, (Vector2){ creditsbackgroundX + 500, creditsbackgroundY }, 0.0f, 0.8f, WHITE);
                DrawTextureEx(creditsbackground, (Vector2){ creditsbackgroundX + creditsbackground.width * 1.0f, creditsbackgroundY }, 0.0f, 0.8f, WHITE);
    
                // Update background X offset for movement
                creditsbackgroundX -= BACKGROUND_MOVE_SPEED * GetFrameTime();
    
                // Reset background X offset when it goes off-screen
                if (creditsbackgroundX <= -creditsbackground.width)
                {
                    creditsbackgroundX = 0;
                }
                
                DrawText("CREDITS",700,50,30,GRAY);
                DrawText("Backgrounds and objects;",450,100,30,GRAY);
                DrawText(" https://saurabhkgp.itch.io",500,150,30,GRAY);
                DrawText(" https://edermunizz.itch.io",500,200,30,GRAY);
                DrawText(" https://free-game-assets.itch.io",500,250,30,GRAY);
                DrawText(" https://gleao.itch.io",500,300,30,GRAY);    
                DrawText("Press space to return to main menu",1200,620,20,GRAY);
                DrawText("Resources;",450,350,30,GRAY);
                DrawText("https://www.raylib.com/examples.html",500,400,30,GRAY);
                    
                    
                } break;
                
                case ANIM:
                {
                    DrawCircleV(position, radius, Fade(BLACK, alpha));
                    
                }break;
                
                case MAIN_MENU:
                {
                
                if (!IsSoundPlaying(menuS))
                {
                  PlaySound(menuS);
                }
                
                // Draw moving background with zoom out
                DrawTextureEx(background, (Vector2){ backgroundX + 500, backgroundY }, 0.0f, 0.8f, WHITE);
                DrawTextureEx(background, (Vector2){ backgroundX + background.width * 1.0f, backgroundY }, 0.0f, 0.8f, WHITE);
    
                // Update background X offset for movement
                backgroundX -= BACKGROUND_MOVE_SPEED * GetFrameTime();
    
                // Reset background X offset when it goes off-screen
                if (backgroundX <= -background.width)
                {
                    backgroundX = 0;
                }
                
                DrawTextureRec(startbutton, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
                DrawTextureRec(howtoplaybutton, sourceRec2, (Vector2){ btnBounds2.x, btnBounds2.y }, WHITE);
                DrawTextureRec(creditsbutton, sourceRec3, (Vector2){ btnBounds3.x, btnBounds3.y }, WHITE);
                DrawTextureRec(exitbutton, sourceRec4, (Vector2){ btnBounds4.x, btnBounds4.y }, WHITE);
                
                DrawText("Red Ball Game", 450, 100, 100, MAROON);
              
                }break;
                default: break;
            }

        EndMode2D();

        EndDrawing();
    }

    // De-Initialization
    UnloadSound(gameS);
    UnloadSound(menuS);
    UnloadTexture(background);
    UnloadTexture(redball);
    UnloadTexture(creditsbackground);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}