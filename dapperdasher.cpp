#include <iostream>
#include "raylib.h"

struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return (data.pos.y >= windowHeight - data.rec.height);
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    // update running time
    data.runningTime += deltaTime;
    if (data.runningTime > data.updateTime)
    {
        data.runningTime = 0;
        data.rec.x = data.frame * data.rec.width;
        data.frame++;
        if (data.frame >= maxFrame)
        {
            data.frame = 0;
        }
    }
    return data;
}

int main()
{

    // window dimensions
    const int windowWidth = 512;
    const int windowHeight = 380;
    InitWindow(windowWidth, windowHeight, "Dapper Dasher");
    // acceleration due to gravity (pixels/s)/s
    const int gravity = 1000;

    // scarfy variables

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    // Texture2D background = LoadTexture("textures/foreground.png");
    // sprite selection

    AnimData scarfyData;

    scarfyData.rec.width = scarfy.width / 6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;
    // position of scarfy
    scarfyData.pos.x = windowWidth / 2 - scarfyData.rec.width / 2;
    scarfyData.pos.y = windowHeight - scarfyData.rec.height;
    // scarfy animation frame
    scarfyData.frame = 0;
    // scarfy animation update time
    scarfyData.updateTime = 1.0 / 6.0;
    // scarfy animation running time
    scarfyData.runningTime = 0;

    AnimData nebulaData;

    // obstacle variables (nebula)
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    nebulaData.rec.width = nebula.width / 8;
    nebulaData.rec.height = nebula.height / 8;
    nebulaData.rec.x = 0;
    nebulaData.rec.y = 0;
    // position of nebula
    nebulaData.pos.x = windowWidth;
    nebulaData.pos.y = windowHeight - nebulaData.rec.height;
    // nebula animation frame
    nebulaData.frame = 0;
    // nebula animation update time
    nebulaData.updateTime = 1.0 / 24.0;
    // nebula animation running time
    nebulaData.runningTime = 0;


    // nebula velocity (pixels/sec)
    int nebVelocity = -200;

    const int numNeb = 15;
    AnimData nebulae[numNeb] = {};

    for (int i = 0; i < numNeb; i++)
    {
        nebulae[i].rec.width = nebula.width / 8;
        nebulae[i].rec.height = nebula.height / 8;
        nebulae[i].rec.x = 0;
        nebulae[i].rec.y = 0;
        // position of nebula
        nebulae[i].pos.y = windowHeight - nebulae[i].rec.height;
        // nebula animation frame
        nebulae[i].frame = 0;
        // nebula animation update time
        nebulae[i].updateTime = 1.0 / 12.0;
        // nebula animation running time
        nebulae[i].runningTime = 0;
        // nebula animation
        nebulae[i].pos.x = windowWidth + (i * 300);
    }

    SetTargetFPS(60);

    int velocity = 0;
    const int jumpVelocity = -600;
    bool inAir;

    // background
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX = 0;

    // midground
    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float midX = 0;

    // foreground
    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX = 0;

    float finishLine = nebulae[numNeb - 1].pos.x;

    bool collision = false;      

    // game loop
    while (!WindowShouldClose())
    {
        // delta time (time since last frame)
        float deltaTime = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        bgX -= 20 * deltaTime;
        if (bgX <= -background.width * 2)
        {
            bgX = 0;
        }
        // draw background
        Vector2 bg1Pos{bgX, 0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        Vector2 bg2Pos{bgX + background.width * 2, 0};
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        midX -= 40 * deltaTime;
        if (midX <= -midground.width * 2)
        {
            midX = 0;
        }
        // draw midground
        Vector2 mid1Pos{midX, 0};
        DrawTextureEx(midground, mid1Pos, 0.0, 2.0, WHITE);
        Vector2 mid2Pos{midX + midground.width * 2, 0};
        DrawTextureEx(midground, mid2Pos, 0.0, 2.0, WHITE);

        fgX -= 80 * deltaTime;
        if (fgX <= -foreground.width * 2)
        {
            fgX = 0;
        }
        // draw foreground
        Vector2 fg1Pos{fgX, 0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        Vector2 fg2Pos{fgX + foreground.width * 2, 0};
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);

        // perform ground check
        if (isOnGround(scarfyData, windowHeight))
        {
            // on the ground
            velocity = 0;
            inAir = false;
        }
        else
        {
            // in the air
            velocity += gravity * deltaTime;
            inAir = true;
        }

        if (IsKeyDown(KEY_A) && scarfyData.pos.x > 0)
        {
            scarfyData.pos.x -= 10;
        }
        if (IsKeyDown(KEY_D) && scarfyData.pos.x < windowWidth)
        {
            scarfyData.pos.x += 10;
        }

        // jump action
        if (IsKeyPressed(KEY_SPACE) && !inAir)
        {

            velocity += jumpVelocity;
        }

        // update scarfy position
        scarfyData.pos.y += velocity * deltaTime;

        // scarfy animation update
        scarfyData.runningTime += deltaTime;
        if (!inAir)
        {
            scarfyData = updateAnimData(scarfyData, deltaTime, 6);
        }

        for (AnimData nebula : nebulae)
        {

            float pad = 50;
            Rectangle nebRec{
                nebula.pos.x + pad,
                nebula.pos.y + pad,
                nebula.rec.width - pad * 2,
                nebula.rec.height - pad * 2};

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height};

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
        }

        // collision checker
        if (collision)
        {
            // lose game
            DrawText("Game Over", windowWidth/4, windowHeight/2, 40, RED);
        }
        else if(scarfyData.pos.x >= finishLine){
            // win game
            DrawText("You Win", windowWidth/4, windowHeight/2, 40, GREEN);
        }
        else 
        {
            // draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

            for (int i = 0; i < numNeb; i++)
            {
                // draw nebula
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
        }
        // nebula update animation
        for (int i = 0; i < numNeb; i++)
        {
            // update nebula position
            nebulae[i].pos.x += nebVelocity * deltaTime;
            nebulae[i] = updateAnimData(nebulae[i], deltaTime, 8);
        }

        // update finish line
        finishLine += nebVelocity * deltaTime;
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}
