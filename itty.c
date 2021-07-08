/*
=========== CONTROLS ==============
o  SCREENSHOT(ENTER)
o  sprite-width=(LEFT/RIGHT)
o  sprite-height=(UP/DOWN)
o  num-colors=(PLUS/MINUS)
o  black=(O/P)
===================================
*/

#include <raylib.h>

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

#define SCR_W 320
#define SCR_H 240
#define DISP_W SCR_W * 4
#define DISP_H SCR_H * 4

void
gen_sprite(int pos_x, int pos_y, int width, int height, int colour_count, int percentage_black)
{
    int w = (width / 2) + (width % 2 != 0);  // ceil division
    int h = height;
    
    // Select random colors
    Color colors[32] = { 0 };
    for (int i = 0; i < colour_count; ++i)
    {
        colors[i] = ColorFromHSV((Vector3){ (float)GetRandomValue(0, 255), (float)GetRandomValue(10, 200), (float)GetRandomValue(100, 220) });
    }

    // Randomly draw pixels
    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            Color pixel_colour = BLACK;
            if (GetRandomValue(0, 100) > percentage_black)
            {
                pixel_colour = colors[GetRandomValue(0, colour_count - 1)];
            }

            DrawPixel(pos_x + x, pos_y + y, pixel_colour);
            DrawPixel((pos_x + width - x - 1), pos_y + y, pixel_colour);  // Mirror across vertical
        }
    }
}

int
main()
{
    InitWindow(DISP_W, DISP_H, "itty-gen");
    SetTargetFPS(60);

    RenderTexture2D screen = LoadRenderTexture(SCR_W, SCR_H);
    SetTextureFilter(screen.texture, FILTER_POINT);

    int sprite_w = 8;
    int sprite_h = 8;
    int sprite_color_count = 2;
    int percentage_black = 50;

    while (!WindowShouldClose())
    {
        // NOTE: Screenshot
        if (IsKeyPressed(KEY_ENTER))
        {
            int i = 0;
            while (FileExists(FormatText("./itty-sprites/itty_img%d.png", i)))
                ++i;
            
            TakeScreenshot(TextFormat("./itty-sprites/itty_img%d.png", i));
        }

        // NOTE: Control values
        if (IsKeyPressed(KEY_UP))
            sprite_h += 1;
        else if (IsKeyPressed(KEY_DOWN))
            sprite_h -= 1;
        sprite_h = max(sprite_h, 1);

        if (IsKeyPressed(KEY_RIGHT))
            sprite_w += 1;
        else if (IsKeyPressed(KEY_LEFT))
            sprite_w -= 1;
        sprite_w = max(sprite_w, 1);

        if (IsKeyPressed(KEY_EQUAL))
            sprite_color_count += 1;
        else if (IsKeyPressed(KEY_MINUS))
            sprite_color_count -= 1;
        sprite_color_count = min(max(sprite_color_count, 1), 32);

        if (IsKeyPressed(KEY_O))
            percentage_black -= 1;
        else if (IsKeyPressed(KEY_P))
            percentage_black += 1;
        percentage_black = min(max(percentage_black, 0), 100);

        SetWindowTitle(TextFormat("itty-gen  w=%d h=%d col=%d \%black=%d", sprite_w, sprite_h, sprite_color_count, percentage_black));

        BeginDrawing();
            if (IsKeyReleased(KEY_SPACE))
            {
                BeginTextureMode(screen);
                    ClearBackground(BLACK);

                    // NOTE: Render lots of sprites
                    for (int x = 1; x < SCR_W; x += 2 + sprite_w)
                        for (int y = 1; y < SCR_H; y += 2 + sprite_h)
                            gen_sprite(x, y, sprite_w, sprite_h, sprite_color_count, percentage_black);
                
                EndTextureMode();
            }

            ClearBackground(BLACK);

            DrawTexturePro(screen.texture, (Rectangle){ 0.0f, 0.0f, screen.texture.width, -screen.texture.height }, (Rectangle){ 0.0f, 0.0f, DISP_W, DISP_H }, (Vector2){ 0.0f, 0.0f }, 0.0f, WHITE);
            
        EndDrawing();
    }
}
