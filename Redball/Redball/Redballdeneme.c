#include "raylib.h"
#include"stdlib.h"
#include <math.h>

#define G 400
#define PLAYER_JUMP_SPD 300.0f
#define PLAYER_HOR_SPD 150.0f
#define SCROLL_SPEED 40.0f
#define BACKGROUND_MOVE_SPEED 3.0f 

#define obstaclecolor BLANK
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
typedef enum GameScreen {GAMEPLAY, PAUSE, MAIN_MENU, LOGO, HOW_TO_PLAY, CREDITS,ENDING,CLEAR,LEVELS} GameScreen;



typedef struct {
    Vector2 position;
    float speed;
    bool canJump;
    float rotationAngle;
} Player;

typedef struct {
    Rectangle rect;
    Color color;
} Obstacle;

int main(void)
{
    // Initialization
    const int screenWidth = 1550;
    const int screenHeight = 650;
    
   //Box A: Moving box (level 1)
    Rectangle boxA = { 2250,820, 60, 20};
    int boxASpeedX = 1.0;   
    
    //Box B: Moving box (level 2)
    Rectangle boxB = { 5500,920, 60, 20};
    int boxBSpeedX = 1.0;
    
    //Box C: Moving box (level 2)
    Rectangle boxC = { 6650,875, 60, 20};
    int boxCSpeedX = 1.0;
    
    //Box D: Moving box (level 2)
    Rectangle boxD = { 7050,855, 60, 20};
    int boxDSpeedX = 1.0;
    
    //Box E: Moving box (level 2)
    Rectangle boxE = { 7890,855, 60, 20};
    int boxESpeedX = 1.0;
    
    //Box F: Moving box (level 3)
    Rectangle boxF = { 11900,940, 60, 20};
    int boxFSpeedX = 1.0;
    
    //Box H: Moving box (level 3)
    Rectangle boxH = { 13800,790, 60, 20};
    int boxHSpeedX = 1.0;
    
    
    InitWindow(screenWidth, screenHeight, "Redball");

    InitAudioDevice();//function calls the audio//

    float elapsedTime = 0.0f; //Cronometer//
    int starsEarned = 0;
    
    GameScreen currentScreen = LOGO;
    int Level;
    
    Texture2D level1 = LoadTexture("level1.png");
    Texture2D level2 = LoadTexture("level2.png");
    Texture2D level3 = LoadTexture("level3.png"); 
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
    Texture2D backgroundoriginal = LoadTexture("backgroundoriginal.png");
    
    Sound gameS = LoadSound("gameplay.mp3");
    Sound menuS = LoadSound("menu.mp3");
    Sound gameoverS = LoadSound("gameoverS.mp3");
    Sound gamel2S = LoadSound("gameplayl2S.mp3");
    Sound gamel3S = LoadSound("gameplayl3S.mp3");  
    Sound winS = LoadSound("winS.mp3");

    
      
    float frameHeight = (float)startbutton.height/NUM_FRAMES;
    Rectangle sourceRec = { 0, 0, (float)startbutton.width, frameHeight };
    
    float frameHeight2 = (float)howtoplaybutton.height/NUM_FRAMES;
    Rectangle sourceRec2 = { 0, 0, (float)howtoplaybutton.width, frameHeight2 };
    
    float frameHeight3 = (float)creditsbutton.height/NUM_FRAMES;
    Rectangle sourceRec3 = { 0, 0, (float)creditsbutton.width, frameHeight3 };
    
    float frameHeight4 = (float)exitbutton.height/NUM_FRAMES;
    Rectangle sourceRec4 = { 0, 0, (float)exitbutton.width, frameHeight4 };
    
    
    float frameHeightl1 = (float)level1.height/NUM_FRAMES;
    Rectangle sourceRecl1 = { 0, 0, (float)level1.width, frameHeightl1 };
    
    float frameHeightl2 = (float)level2.height/NUM_FRAMES;
    Rectangle sourceRecl2 = { 0, 0, (float)level2.width, frameHeightl2 };
    
    float frameHeightl3 = (float)level3.height/NUM_FRAMES;
    Rectangle sourceRecl3 = { 0, 0, (float)level3.width, frameHeightl3 };
    
    
    Rectangle btnBounds = { screenWidth/2.0f - startbutton.width/2.0f, screenHeight/2.0f - startbutton.height/2, (float)startbutton.width, frameHeight };
    Rectangle btnBounds2 = { screenWidth/2.0f - howtoplaybutton.width/2.0f, screenHeight/2.0f - howtoplaybutton.height/6, (float)howtoplaybutton.width, frameHeight2 };
    Rectangle btnBounds3 = { screenWidth/2.0f - creditsbutton.width/2.0f, screenHeight/2.0f + creditsbutton.height/6 , (float)creditsbutton.width, frameHeight3 };
    Rectangle btnBounds4 = { screenWidth/2.0f - exitbutton.width/2.0f, screenHeight/2.0f + exitbutton.height/2, (float)exitbutton.width, frameHeight4 };
    
    Rectangle btnBoundsl1 = { screenWidth/2.0f - level1.width/2.0f, screenHeight/2.0f - level1.height/2, (float)level1.width, frameHeightl1 };
    Rectangle btnBoundsl2 = { screenWidth/2.0f - level2.width/2.0f, screenHeight/2.0f - level2.height/6, (float)level2.width, frameHeightl2 };
    Rectangle btnBoundsl3 = { screenWidth/2.0f - level3.width/2.0f, screenHeight/2.0f + level3.height/6 , (float)level3.width, frameHeightl3 };

    int btnState = 0;               
    bool btnAction = false;  
    
     int btnState2 = 0;               
    bool btnAction2 = false;  
    
     int btnState3 = 0;               
    bool btnAction3 = false;  
    
     int btnState4 = 0;               
    bool btnAction4 = false;  
    
    
    int btnStatel1 = 0;               
    bool btnActionl1 = false;  
    
     int btnStatel2 = 0;               
    bool btnActionl2 = false;  
    
     int btnStatel3 = 0;               
    bool btnActionl3 = false; 
    
    
    Vector2 mousePoint = { 0.0f, 0.0f };
    Vector2 mousePoint2 = { 0.0f, 0.0f }; 
    Vector2 mousePoint3 = { 0.0f, 0.0f };
    Vector2 mousePoint4 = { 0.0f, 0.0f };
    
    Vector2 mousePointl1 = { 0.0f, 0.0f };
    Vector2 mousePointl2 = { 0.0f, 0.0f }; 
    Vector2 mousePointl3 = { 0.0f, 0.0f };
    
    
    
    
    // Adjust the volume of the loaded sounds
    SetSoundVolume(gameS, 0.1f); 
    SetSoundVolume(menuS, 0.1f); 
    SetSoundVolume(gameoverS,0.1f);
    SetSoundVolume(gamel2S, 0.1f); 
    SetSoundVolume(gamel3S, 0.1f); 
    SetSoundVolume(winS, 0.1f); 
    SetSoundVolume(gameoverS, 0.1f); 
    
    Player player = { 0 };
    player.position = (Vector2){ 250, 1370 }; //Player starting position <z
    player.speed = 0;
    player.canJump = false;
    player.rotationAngle = 0;

    float backgroundX = -955.0f;
    float backgroundY = 0.0f; 
    
    float backgroundoriginalX = -950.0f;
    float backgroundoriginalY = 0.0f; 
   
    float creditsbackgroundX = -500.0f;
    float creditsbackgroundY = 0.0f; 
    
    float howtoplaybackgroundX = -500.0f;
    float howtoplaybackgroundY = 0.0f; 
    
    bool jumping = false;

    // Invisible wall
   // Obstacle wall = {{0, 1100, 20, screenHeight},BLANK}; 
       
      Obstacle obstacles[] = {
    // level 1 begins
    {{ 200, 878, 145, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 100, 995, 55, OBSTACLE_HEIGHT }, obstaclecolor}, 
    {{ 468, 920, 338, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 100, 995, 55, OBSTACLE_HEIGHT }, obstaclecolor}, 
    {{ 975, 995, 43, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1186, 952, 266, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1181, 851, 45, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1553, 1013, 37, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1524, 863, 133, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1308, 732, 232, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1620, 810, 40, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1620, 658, 85, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1778, 643, 44, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1778, 788, 200, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 1998, 888, 152, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 2198, 987, 53, OBSTACLE_HEIGHT }, obstaclecolor}, 
    {{ 2460, 787, 165, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 2626, 1008, 40, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 2626, 902, 40, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 2626, 824, 40, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 2804, 793, 165, OBSTACLE_HEIGHT }, obstaclecolor},
    // level 2 begins
    {{ 4161, 1013, 59, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 4490, 980, 256, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 4328, 974, 48, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 4910, 924, 160, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 5270, 848, 157, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 6149, 1011, 38, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 6330, 946, 250, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 7688, 1013, 38, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 7529, 929, 118, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 7359, 832, 118, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 7624, 784, 158, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 8321, 797, 158, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 8624, 771, 178, OBSTACLE_HEIGHT }, obstaclecolor},
    // Level 3 begins
    {{ 10104, 794, 38, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 10440, 1000, 60, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 10335, 910, 60, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 10100, 838, 180, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 10390, 775, 150, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 10620, 760, 200, OBSTACLE_HEIGHT },obstaclecolor},
    {{ 11060, 990, 20, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11085, 900, 100, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11480, 955, 70, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11480, 835, 70, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11480, 715, 70, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11480, 605, 70, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11480, 535, 70, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11550, 510, 500, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 12155, 510, 60, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 12320, 510, 360, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 12500, 620, 75, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 11835, 740, 570, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 12495, 930, 150, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 12970, 1000, 55, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 13095, 950, 55, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 13210, 910, 180, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 13528, 840, 200, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 14480, 770, 80, OBSTACLE_HEIGHT }, obstaclecolor},   
    {{ 14480, 660, 80, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 14480, 560, 80, OBSTACLE_HEIGHT }, obstaclecolor},
    {{ 14540, 530, 150, OBSTACLE_HEIGHT }, obstaclecolor},
      
};

    
    int numObstacles = sizeof(obstacles) / sizeof(obstacles[0]);
    
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
    camera.zoom = 1.4f;

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
                // Transition directly to MAIN_MENU after logo animation ends
                currentScreen = MAIN_MENU;
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

            case LEVELS:
            {       
                   StopSound(gameS);
                   StopSound(gamel2S);
                   StopSound(gamel3S);
                   
                   if (!IsSoundPlaying(menuS))
                   {
                       PlaySound(menuS);
                   }

            
                   mousePointl1 = GetMousePosition();
                   btnActionl1 = false;

                   mousePointl2 = GetMousePosition();
                   btnActionl2 = false;
                   
                   mousePointl3 = GetMousePosition();
                   btnActionl3 = false;
                    
                    if (CheckCollisionPointRec(mousePointl1, btnBoundsl1))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnStatel1 = 2;
                        else btnStatel1 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnActionl1 = true;
                    }
                    else btnStatel1 = 0;

                    if (btnActionl1)
                    {   
                        Level = 1;
                        player.position = (Vector2){ 280, 1370 };
                        currentScreen = GAMEPLAY;
                    }
                    
                    sourceRecl1.y = btnStatel1*frameHeightl1;  
         

                    if (CheckCollisionPointRec(mousePointl2, btnBoundsl2))
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnStatel2 = 2;
                        else btnStatel2 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnActionl2 = true;
                    }
                    else btnStatel2 = 0;

                    if (btnActionl2)
                    {
                        player.position = (Vector2){4023,1370 };
                        Level = 2;
                        currentScreen = LOGO;
                   }
    
                    sourceRecl2.y = btnStatel2*frameHeightl2;
                    
                    if (CheckCollisionPointRec(mousePointl3, btnBoundsl3))  
                    {
                        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnStatel3 = 2;
                        else btnStatel3 = 1;

                        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnActionl3 = true;
                    }
                    else btnStatel3 = 0;
                                       
                    if (btnActionl3)
                    {
                        Level = 3;
                        player.position = (Vector2){10100, 1370};
                        currentScreen = GAMEPLAY;
                    }
                                  
                 sourceRecl3.y = btnStatel3*frameHeightl3;
                    
     
            }break;
            
            case GAMEPLAY:
            {                             
                StopSound(menuS);
                  
                if ((player.position.x >= 100)&&(player.position.x <= 3200))
                {
                  if (!IsSoundPlaying(gameS))
                  {
                      PlaySound(gameS);
                  }
                }
                
                 if ((player.position.x >= 3800)&&(player.position.x <= 9500))
                {
                  if (!IsSoundPlaying(gamel2S))
                  {
                      StopSound(gameS);
                      PlaySound(gamel2S);
                  }
                }
                 
                 if ((player.position.x >= 9550))
                {
                  if (!IsSoundPlaying(gamel3S))
                  {
                      PlaySound(gamel3S);
                  }
                }
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
            
                StopSound(gameS);
                StopSound(gamel2S);
                StopSound(gamel3S);
            if (!IsSoundPlaying(menuS))
            {
                PlaySound(menuS);
            }


             if(IsKeyPressed(KEY_SPACE))
               { 
                currentScreen = MAIN_MENU;
               }
             
            }break;
            
            case CLEAR:
            {   
                StopSound(gameS);
                StopSound(gamel2S);
                StopSound(gamel3S);
                StopSound(menuS);

                
                if(Level == 2)
                {
                 player.position = (Vector2){ 4050, 1370 };
                 elapsedTime = 0;
                 
                }
                
              if(Level == 3)
                {
                 player.position = (Vector2){ 10000, 1370 };
                 elapsedTime = 0;
                }
              
              if(IsKeyPressed(KEY_ENTER))
                { 
                    currentScreen = GAMEPLAY;
                }
               
            }break;
            
            case CREDITS:
            {
            
                StopSound(gameS);
                StopSound(gamel2S);
                StopSound(gamel3S);
                  
                if (!IsSoundPlaying(menuS))
                {
                  PlaySound(menuS);                      
                }      
                            
              if(IsKeyPressed(KEY_SPACE))
               { 
                currentScreen = MAIN_MENU;
               }
             
            }break;
            
            case ENDING:
            {
                              
                StopSound(gameS);
                StopSound(gamel2S);
                StopSound(gamel3S);
                StopSound(menuS);                               
              
              if(IsKeyPressed(KEY_R))
               { 
                currentScreen = MAIN_MENU;
               }
             
            }break;
            
            case MAIN_MENU:
            {
                StopSound(gameS);
                StopSound(gamel2S);
                StopSound(gamel3S);
                  
                if (!IsSoundPlaying(menuS))
                {
                  PlaySound(menuS);                      
                }
                 

                 elapsedTime = 0;
                    
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
                        currentScreen = LEVELS;
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

           switch(currentScreen)
            {
                case GAMEPLAY:
                {                                                               
                      BeginMode2D(camera);
                  
                    // Player movement
                if (IsKeyDown(KEY_LEFT) && player.position.x > 50)
                {
                    player.position.x -= PLAYER_HOR_SPD * deltaTime;
                   // backgroundX += SCROLL_SPEED * deltaTime;
                   player.rotationAngle -= PLAYER_HOR_SPD * deltaTime * 3; // Rotate left (shows scrolling)
                }
                if (IsKeyDown(KEY_RIGHT))
                 {
                  player.position.x += PLAYER_HOR_SPD * deltaTime;
                  //  backgroundX -= SCROLL_SPEED * deltaTime;
                  player.rotationAngle += PLAYER_HOR_SPD * deltaTime * 3; // Rotate right (shows scrolling) 
                 }
                
        // Player jumping
        if ((IsKeyDown(KEY_SPACE) && player.canJump))
        {
            player.speed = -PLAYER_JUMP_SPD;
            player.canJump = false;
            jumping = true;
        }  

        // Player falling
        player.position.y += player.speed * deltaTime;
        player.speed += G * deltaTime;

        // Player collision with ground
        if ((player.position.y >= 1050) && (player.position.y <= 1500))     
        {
            player.position.y = 1050;
            player.speed = 0;
            player.canJump = true;
            jumping = false;
            backgroundY = 0.0f; // Reset background Y offset upon landing
        }
        
         
         // moving obstacle for level 1
          if((player.position.x - boxA.x > -15)&&(player.position.x - boxA.x < 60))
                     {
                         if((boxA.y - player.position.y < 20)&&(boxA.y - player.position.y > -10))
                         {
                             player.position.y = 810;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
         // moving obstacle for level 2
          if((player.position.x - boxB.x > -15)&&(player.position.x - boxB.x < 60))
                     {
                         if((boxB.y - player.position.y < 20)&&(boxB.y - player.position.y > -10))
                         {
                             player.position.y = 910;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
         // moving obstacle for level 2
          if((player.position.x - boxC.x > -15)&&(player.position.x - boxC.x < 60))
                     {
                         if((boxC.y - player.position.y < 20)&&(boxC.y - player.position.y > -10))
                         {
                             player.position.y = 865;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
                     
          // moving obstacle for level 2
          if((player.position.x - boxD.x > -15)&&(player.position.x - boxD.x < 60))
                     {
                         if((boxD.y - player.position.y < 20)&&(boxD.y - player.position.y > -10))
                         {
                             player.position.y = 845;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
          // moving obstacle for level 2
          if((player.position.x - boxE.x > -15)&&(player.position.x - boxE.x < 60))
                     {
                         if((boxE.y - player.position.y < 20)&&(boxE.y - player.position.y > -10))
                         {
                             player.position.y = 845;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
          // moving obstacle for level 3
          if((player.position.x - boxF.x > -15)&&(player.position.x - boxF.x < 60))
                     {
                         if((boxF.y - player.position.y < 20)&&(boxF.y - player.position.y > -10))
                         {
                             player.position.y = 930;
                             player.canJump = true;
                             jumping = false;
                         }
                     }
                     
          // moving obstacle for level 3
          if((player.position.x - boxH.x > -15)&&(player.position.x - boxH.x < 60))
                     {
                         if((boxH.y - player.position.y < 20)&&(boxH.y - player.position.y > -10))
                         {
                             player.position.y = 780;
                             player.canJump = true;
                             jumping = false;
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
                          
                         // Check collision with obstacles
                       for (int i = 0; i < numObstacles; i++)
                        {
                         // Check if the player rectangle collides with the obstacle rectangle
                           if (CheckCollisionRecs((Rectangle){player.position.x - 20, player.position.y - redball.height / 2, redball.width, redball.height}, obstacles[i].rect))
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

                       // Check collision with invisible walls
                       for (int i = 0; i < numObstacles; i++)
                        {
                         // Check if the player rectangle collides with the obstacle rectangle
                           if (CheckCollisionRecs((Rectangle){player.position.x - 20, player.position.y - redball.height / 2, redball.width, redball.height}, obstacles[i].rect))
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
                             
                            if(player.position.x < 116)
                            {
                                player.position.x = 116;
                            }

                            if((player.position.x > 463)&&(player.position.x < 795))
                            {
                               if(player.position.y > 920)
                               {
                                   player.position.x = 463;
                               }                                   
                            }
                            
                             if((player.position.x > 808)&&(player.position.x < 813))
                            {
                               if(player.position.y > 920)
                               {
                                   player.position.x = 813;
                               }                                   
                            }
                            
                            // invisible wall for the box in level one
                             if((player.position.x > 1173)&&(player.position.x < 1200))
                            {
                               if((player.position.y > 851)&&(player.position.y < 957))
                               {
                                   player.position.x = 1173;
                               }                                   
                            }                                                    
 
                            // invisible wall for the box in level one(one box)
                               if((player.position.x > 1236)&&(player.position.x < 1242))
                            {
                               if((player.position.y > 851)&&(player.position.y < 957))
                               {
                                   player.position.x = 1242;
                               }                                   
                            }
                            
                            // invisible wal from left for the box
                                if((player.position.x > 1603)&&(player.position.x < 1620))
                            {
                               if((player.position.y > 810)&&(player.position.y < 855))
                               {
                                   player.position.x = 1603;
                               }                                   
                            }
                            
                             // invisible wal from righ for the box
                                if((player.position.x > 1675)&&(player.position.x < 1678))
                            {
                               if((player.position.y > 810)&&(player.position.y < 855))
                               {
                                   player.position.x = 1678;
                               }                                   
                            }
                            
                                if((player.position.x > 1605)&&(player.position.x < 1662))
                            {
                               if((player.position.y > 860)&&(player.position.y < 865))
                               {
                                   player.position.y = 865;
                               }                                   
                            }
                            
                              if((player.position.x > 1764)&&(player.position.x < 1767))
                            {
                               if(player.position.y > 643)
                               {
                                   player.position.x = 1764;
                               }                                   
                            }
                            
                              if((player.position.x > 1970)&&(player.position.x < 1973))
                            {
                               if(player.position.y > 788)
                               {
                                   player.position.x = 1973;
                               }                                   
                            }
                               
                               if((player.position.x > 1835)&&(player.position.x < 1838))
                            {
                               if(player.position.y > 643)
                               {
                                   player.position.x = 1838;
                               }                                   
                            }
                            
                                 if((player.position.x > 2450)&&(player.position.x < 2455))
                            {
                               if(player.position.y > 787)
                               {
                                   player.position.x = 2450;
                               }                                   
                            }
                            
                                   if((player.position.x > 2635)&&(player.position.x < 2638))
                            {
                               if(player.position.y > 787)
                               {
                                   player.position.x = 2638;
                               }                                   
                            }
                            
                            // the first invisible wall for level 2
                               if((player.position.x > 4480)&&(player.position.x < 4495))
                            {
                               if(player.position.y > 980)
                               {
                                   player.position.x = 4480;
                               }                                   
                            }
                            
                                if((player.position.x > 4760)&&(player.position.x < 4763))
                            {
                               if(player.position.y > 980)
                               {
                                   player.position.x = 4763;
                               }                                   
                            }
                            
                                 if((player.position.x > 5260)&&(player.position.x < 5270))
                            {
                               if(player.position.y > 848)
                               {
                                   player.position.x = 5260;
                               }                                   
                            }
                            
                                if((player.position.x > 5440)&&(player.position.x < 5443))
                            {
                               if(player.position.y > 848)
                               {
                                   player.position.x = 5443;
                               }                                   
                            }
                            
                                if((player.position.x > 6315)&&(player.position.x < 6325))
                            {
                               if(player.position.y > 946)
                               {
                                   player.position.x = 6315;
                               }                                   
                            }
                            
                                if((player.position.x > 6594)&&(player.position.x < 6597))
                            {
                               if(player.position.y > 946)
                               {
                                   player.position.x = 6597;
                               }                                   
                            }
                              if((player.position.x > 7349)&&(player.position.x < 7359))
                            {
                               if(player.position.y > 832)
                               {
                                   player.position.x = 7349;
                               }                                   
                            }
                            
                              if((player.position.x > 7484)&&(player.position.x < 7487))
                            {
                               if(player.position.y > 832)
                               {
                                   player.position.x = 7487;
                               }                                   
                            }
                            
                              if((player.position.x > 7484)&&(player.position.x < 7487))
                            {
                               if(player.position.y > 832)
                               {
                                   player.position.x = 7487;
                               }                                   
                            }
                            
                              if((player.position.x > 8614)&&(player.position.x < 8624))
                            {
                               if(player.position.y > 771)
                               {
                                   player.position.x = 8614;
                               }                                   
                            }
                            
                                if((player.position.x > 8820 )&&(player.position.x < 8823 ))
                            {
                               if(player.position.y > 771)
                               {
                                   player.position.x = 8823;
                               }                                   
                            }
                            
                               if((player.position.x > 8848 )&&(player.position.x < 8850))
                            {                                                          
                                   player.position.x = 8848;                                                               
                            }
                            
                               if((player.position.x > 4022 )&&(player.position.x < 4024))
                            {                                                          
                                   player.position.x = 4024;                                                               
                            }
                            
                               if((player.position.x > 2909 )&&(player.position.x < 2911))
                            {                                                          
                                   player.position.x = 2909;                                                               
                            }
                            
                            //First invisible wall for Level 3
                            
                            if((player.position.x > 9970 )&&(player.position.x < 9980))
                            {                                                          
                                   player.position.x = 9980;                                                               
                            }
                            
                            if((player.position.x > 10620 )&&(player.position.x < 10625 ))
                            {
                               if(player.position.y > 760)
                               {
                                   player.position.x = 10620;
                               }                                   
                            }
                            
                            if((player.position.x > 10820 )&&(player.position.x < 10825 ))
                            {
                               if(player.position.y > 760)
                               {
                                   player.position.x = 10825;
                               }                                   
                            }
                            
                            if((player.position.x > 11545 )&&(player.position.x < 11555 ))
                            {
                               if(player.position.y > 500)
                               {
                                   player.position.x = 11545;
                               }                                   
                            }
                            
                            if((player.position.x > 11710 )&&(player.position.x < 11715 ))
                            {
                               if(player.position.y > 500)
                               {
                                   player.position.x = 11715;
                               }                                   
                            }
                            
                            // arada eksik duvar var buraya ekle son ikisini aşağı yazıyorum
                            if((player.position.x > 14540 )&&(player.position.x < 14545))
                            {
                               if(player.position.y > 530)
                               {
                                   player.position.x = 14540;
                               }                                   
                            }
                            
                            // Spikes
                              if((player.position.x > 4200)&&(player.position.x < 4490))
                            {
                               if(player.position.y > 1048)
                               {
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }                                   
                            }
                            
                             if((player.position.x > 4670)&&(player.position.x < 5270))
                            {
                               if(player.position.y > 1048)
                               {
                                   player.position.y = 1043;
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }                                   
                            }
                            
                             if((player.position.x > 5430)&&(player.position.x < 5950))
                            {
                               if(player.position.y > 1038)
                               {
                                   player.position.y = 1038;
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }                                   
                            }
                            
                            if((player.position.x > 6580)&&(player.position.x < 7360))
                            {
                               if(player.position.y > 1033)
                               {
                                   player.position.y = 1033;
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }                                   
                            }
                             // first box in level 3
                               if((player.position.x > 10154 )&&(player.position.x < 10157))
                            {
                               if((player.position.y > 794)&&(player.position.y < 872))
                               {
                                   player.position.x = 10157;
                               }                                   
                            }    

                               if((player.position.x > 10094 )&&(player.position.x < 10104))
                            {
                               if((player.position.y > 794)&&(player.position.y < 862))
                               {
                                   player.position.x = 10094;
                               }                                   
                            }
                            
                            if((player.position.x > 12050)&&(player.position.x < 12320))
                            {
                               if((player.position.y > 670)&&(player.position.y < 690))
                               {                                  
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }
                            }
                            
                            if((player.position.x > 12051 )&&(player.position.x < 12053))
                            {
                               if((player.position.y > 510)&&(player.position.y < 690))
                               {
                                   player.position.x = 12053;
                               }
                            }   
                            
                            if((player.position.x > 12050)&&(player.position.x < 12322))
                            {
                               if((player.position.y > 693)&&(player.position.y < 698))
                               {                                  
                                   player.position.y = 698;                               
                               }
                            }
                            
                            if((player.position.x > 12046 )&&(player.position.x < 12050))
                            {
                               if((player.position.y > 510)&&(player.position.y < 690))
                               {
                                   player.position.x = 12046;
                               }
                            }
                            
                            if((player.position.x > 12314 )&&(player.position.x < 12316))
                            {
                               if((player.position.y > 510)&&(player.position.y < 690))
                               {
                                   player.position.x = 12314;
                               }
                            }
                            
                             if((player.position.x > 12322 )&&(player.position.x < 12325))
                            {
                               if((player.position.y > 510)&&(player.position.y < 690))
                               {
                                   player.position.x = 12325;
                               }
                            }
                            
                            if((player.position.x > 11550)&&(player.position.x < 12772))
                            {
                               if(player.position.y > 1035)
                               {                                  
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }
                            }
                            
                            if((player.position.x > 12350)&&(player.position.x < 12700))
                            {
                               if((player.position.y > 722)&&(player.position.y < 728))
                               {                                  
                                   currentScreen = ENDING;
                                   PlaySound(gameoverS);
                               }
                            }
                            
                            if((player.position.x > 12730 )&&(player.position.x < 13500 ))
                            {
                               if((player.position.y > 300)&&(player.position.y < 750))
                               {
                                   player.position.x = 12730;
                               }                                   
                            }
                            
                            if((player.position.x > 12320)&&(player.position.x < 12680))
                            {
                               if((player.position.y > 513)&&(player.position.y < 519))
                               {                                  
                                   player.position.y = 519;                               
                               }
                            }
                            
                          }
                                  
                        // Draw player
                        Vector2 origin = { redball.width / 2, redball.height / 2 };
                        DrawTexturePro(redball, (Rectangle){ 0, 0, redball.width, redball.height },
                            (Rectangle){ player.position.x, player.position.y, redball.width, redball.height },
                            origin, player.rotationAngle, WHITE);
                    
                        boxA.x += boxASpeedX; 
                        boxB.x += boxBSpeedX;
                        boxC.x += boxCSpeedX;
                        boxD.x += boxDSpeedX;
                        boxE.x += boxESpeedX;
                        boxF.x += boxFSpeedX;
                        boxH.x += boxHSpeedX;
                        
                       // moving obstacles 
                     
                     // first level   
                     if (((boxA.x + boxA.width) >= GetScreenWidth()+870) || (boxA.x <= 2200)) boxASpeedX *= -1;    
                     // second level
                     if (((boxB.x + boxB.width) >= GetScreenWidth()+4270) || (boxB.x <= 5500)) boxBSpeedX *= -1;
                     // second level
                     if (((boxC.x + boxC.width) >= GetScreenWidth()+5450) || (boxC.x <= 6650)) boxCSpeedX *= -1; 
                     // second level  
                     if (((boxD.x + boxD.width) >= GetScreenWidth()+5700) || (boxD.x <= 7050)) boxDSpeedX *= -1;
                     // second level  
                     if (((boxE.x + boxE.width) >= GetScreenWidth()+6640) || (boxE.x <= 7890)) boxESpeedX *= -1;
                     // third level  
                     if (((boxF.x + boxF.width) >= GetScreenWidth()+10770) || (boxF.x <= 11820)) boxFSpeedX *= -1;    
                     // third level  
                     if (((boxH.x + boxH.width) >= GetScreenWidth()+12800) || (boxH.x <= 13800)) boxHSpeedX *= -1;
                     
                                                              
                        DrawRectangleRec(boxA, BROWN);                   
                        DrawRectangleRec(boxB, BROWN); 
                        DrawRectangleRec(boxC, BROWN);
                        DrawRectangleRec(boxD, BROWN);
                        DrawRectangleRec(boxE, BROWN);
                        DrawRectangleRec(boxF, BROWN);
                        DrawRectangleRec(boxH, BROWN);
                                                
                        if ((player.position.x >= 2860) && (player.position.x <= 2880) && (player.position.y <=800) && (player.position.y >=770))
                        {
                            PlaySound(winS);
                            Level = 2;
                            currentScreen = CLEAR;
                        }
                        
                        if ((player.position.x >= 8680) && (player.position.x <= 8700) && (player.position.y <=771) && (player.position.y >=650))
                        {
                            PlaySound(winS);                              
                            Level = 3;
                            currentScreen = CLEAR;
                        }
                        
                        elapsedTime += GetFrameTime();
                        
                        // Convert elapsed time to minutes and seconds
                        int minutes = (int)(elapsedTime / 60);
                        int seconds = (int)(elapsedTime) % 60;

                        // Draw timer text
                        DrawText(TextFormat("Time: %02d:%02d", minutes, seconds), player.position.x + 370, player.position.y - 220, 30, WHITE);
                        
                         
                        if (elapsedTime <= 10)
                        {
                            starsEarned = 3;
                            
                        } else if (elapsedTime <= 20){
                        starsEarned = 2;
                        } else if (elapsedTime <= 30) {
                        starsEarned = 1;
                        } else {
                         starsEarned = 0; 
                        }           
                        
                        for (int i = 0; i < 3; i++)
                        {
                            if (i < starsEarned)
                            {
                                DrawCircle(player.position.x + i * 55 - 520, player.position.y - 200, 22, PURPLE); // Draw purple ball
                            } else {
                                DrawCircle(player.position.x + i * 55 - 520, player.position.y - 200, 22, GRAY); // Draw grayed-out ball
                                }
                        }                                                  
              
               } break;
               
               case HOW_TO_PLAY:
               {            
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
                DrawText("Press SPACE to return to Main Menu",1200,620,20,GRAY);
             
                   
                DrawTexture(ARROWLEFT,1000,100,GRAY);
                DrawTexture(ARROWRIGHT,1000,250,GRAY);
                DrawTexture(SPACE,1000,400,GRAY);
                    
               }break;
               
                case PAUSE:
                {                                   
                    DrawText("PAUSE SCREEN", 435, 170, 100, RED);
                    DrawText("Press P to Resume the Game", 500, 400, 30, RED);
                    DrawText("Press SPACE to Return to Main Menu",500,450,30,RED);
                } break;
                
                case CLEAR:
                {                                        
                    DrawText("Level is Cleared", 500, 150, 80, WHITE);
                    DrawText("Press Enter to Continue", 550, 250, 30, RED);
                    elapsedTime = 0;
                    
                }break;
                
                case LEVELS:
                {              
                         elapsedTime = 0;
                
                DrawTextureEx(backgroundoriginal, (Vector2){ backgroundoriginalX + 500, backgroundoriginalY }, 0.0f, 0.4f, WHITE);
                DrawTextureEx(backgroundoriginal, (Vector2){ backgroundoriginalX + backgroundoriginal.width * 1.0f, backgroundoriginalY }, 0.0f, 0.8f, WHITE);
    
                // Update background X offset for movement
                backgroundoriginalX -= BACKGROUND_MOVE_SPEED * GetFrameTime();
    
                // Reset background X offset when it goes off-screen
                if (backgroundoriginalX <= -backgroundoriginal.width)
                {
                    backgroundoriginalX = 0;
                }

                
                DrawTextureRec(level1, sourceRecl1, (Vector2){ btnBoundsl1.x, btnBoundsl1.y }, WHITE);
                DrawTextureRec(level2, sourceRecl2, (Vector2){ btnBoundsl2.x, btnBoundsl2.y }, WHITE);
                DrawTextureRec(level3, sourceRecl3, (Vector2){ btnBoundsl3.x, btnBoundsl3.y }, WHITE);
                            
                 DrawText("RED BALL GAME", 400, 60, 100, MAROON);
                }break;
                
                case ENDING:
                {
                                         
                    DrawText("GAME OVER!",500,285,100,RED);
                    DrawText("PRESS ENTER TO RESTART",625,450,20,RED);
                    
                    if(IsKeyPressed(KEY_ENTER))
                    {
                        if(Level == 2)
                        {
                        player.position = (Vector2){ 4050, 1370 };
                        elapsedTime = 0;
                        }else if(Level == 3)
                              {
                                player.position = (Vector2){ 10000, 1370 };
                                elapsedTime = 0;
                              }
                        currentScreen = GAMEPLAY;
                    }
                }
                 break;
                 
                case CREDITS:
                {
                
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
                
                case MAIN_MENU:
                {
                             
                  elapsedTime = 0;
                
                // Draw moving background with zoom out
                DrawTextureEx(backgroundoriginal, (Vector2){ backgroundoriginalX + 500, backgroundoriginalY }, 0.0f, 0.4f, WHITE);
                DrawTextureEx(backgroundoriginal, (Vector2){ backgroundoriginalX + backgroundoriginal.width * 1.0f, backgroundoriginalY }, 0.0f, 0.8f, WHITE);
    
                // Update background X offset for movement
                backgroundoriginalX -= BACKGROUND_MOVE_SPEED * GetFrameTime();
    
                // Reset background X offset when it goes off-screen
                if (backgroundoriginalX <= -backgroundoriginal.width)
                {
                    backgroundoriginalX = 0;
                }

                
                DrawTextureRec(startbutton, sourceRec, (Vector2){ btnBounds.x, btnBounds.y }, WHITE);
                DrawTextureRec(howtoplaybutton, sourceRec2, (Vector2){ btnBounds2.x, btnBounds2.y }, WHITE);
                DrawTextureRec(creditsbutton, sourceRec3, (Vector2){ btnBounds3.x, btnBounds3.y }, WHITE);
                DrawTextureRec(exitbutton, sourceRec4, (Vector2){ btnBounds4.x, btnBounds4.y }, WHITE);
                
                 DrawText("RED BALL GAME", 400, 60, 100, MAROON);
              
                }break;
                default: break;
            }

        EndMode2D();

        EndDrawing();
    }

    // De-Initialization
    UnloadSound(gameS);
    UnloadSound(menuS);
    UnloadSound(gamel2S);
    UnloadSound(gamel3S);
    UnloadSound(winS);
    UnloadSound(gameoverS);
    UnloadTexture(background);
    UnloadTexture(redball);
    UnloadTexture(creditsbackground);
    UnloadTexture(howtoplaybackground);
    UnloadTexture(howtoplaybutton);
    UnloadTexture(startbutton);
    UnloadTexture(exitbutton);
    UnloadTexture(creditsbutton);
    CloseAudioDevice();
    CloseWindow();


    return 0;
} 