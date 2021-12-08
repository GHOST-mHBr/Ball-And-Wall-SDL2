#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>

using namespace std;

#define WINDOW_TITLE "HW4_mHBr"
#define WIDTH 1600
#define HEIGHT 900

#define WALL_DX 5
#define BALL_R  80
#define WALL_WIDTH 20
#define BALL_DX0 5
#define BALL_DY 5
#define BALL_CLR_R 0xFF
#define BALL_CLR_G 0xA6
#define BALL_CLR_B 0x9E

#define BACK_FILE_ADDR  "back.jpg"

int init_window_and_renderer(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("Error in initialization: %s", SDL_GetError());
        return -1;
    }
    if(IMG_Init(IMG_INIT_JPG) < 0){
        printf("Error with image init: %s",SDL_GetError());
        return -1;
    }
    *window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_HIDDEN);
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);

    if (!*window)
    {
        printf("Error with SDL_Window: %s", SDL_GetError());
        return -1;
    }

    if (!*renderer)
    {
        printf("Error with renderer: %s", SDL_GetError());
        return -1;
    }
    return 1;
}

int main()
{
    SDL_Window *win_main = NULL;
    SDL_Renderer *renderer_main = NULL;
    
    SDL_Surface * back_surf = IMG_Load(BACK_FILE_ADDR);

    bool should_loop = true;

    if (init_window_and_renderer(&win_main, &renderer_main) < 0)
    {
        return -1;
    }


    SDL_SetRenderDrawColor(renderer_main , 30 , 40 , 50 , 255);
    SDL_RenderClear(renderer_main);
    
    SDL_ShowWindow(win_main);

    SDL_Texture *wall_txt = SDL_CreateTexture(renderer_main, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WALL_WIDTH, HEIGHT);
    SDL_Texture *back_txt = SDL_CreateTextureFromSurface(renderer_main , back_surf);
    back_surf = NULL;
    // ---------------- declaring game elements and events container -----------------
    SDL_Event event;

    SDL_Point ball_center;
    ball_center.x = BALL_R;
    ball_center.y = HEIGHT - BALL_R;

    SDL_Rect window_bounds;
    window_bounds.x = 0;
    window_bounds.y = 0;
    window_bounds.w = WIDTH;
    window_bounds.h = HEIGHT;

    SDL_Rect ball_bounds;
    ball_bounds.x = ball_center.x - BALL_R;
    ball_bounds.y = ball_center.y - BALL_R;
    ball_bounds.w = 2 * BALL_R;
    ball_bounds.h = 2 * BALL_R;

    SDL_Rect wall;
    wall.x = WIDTH - WALL_WIDTH;
    wall.y = 0;
    wall.h = HEIGHT;
    wall.w = WALL_WIDTH;

    // ---------- declare movement variables -----------
    int ball_dx = BALL_DX0;
    int ball_dy = BALL_DY;
    int wall_dx = -WALL_DX;

    // ----------------- main loop ---------------------
    while (should_loop)
    {
        // ----------- moving elements -------------

        if (wall.x + WALL_DX > WIDTH - WALL_WIDTH)
        {
            wall_dx = -WALL_DX;
        }
        if (wall.x - WALL_DX < WIDTH / 2)
        {
            wall_dx = WALL_DX;
        }
        wall.x += wall_dx;

        if(ball_center.x + BALL_R > wall.x){
            ball_dx = wall_dx - ball_dx;
            ball_center.x = wall.x - BALL_R;
        }

        if(ball_center.x - BALL_R < 0){
            ball_dx *= -1;
        }
        if(ball_center.y + BALL_R > HEIGHT){
            ball_dy = -BALL_DY;
        }
        if(ball_center.y - BALL_R < 0){
            ball_dy = BALL_DY;
        }

        ball_center.x += ball_dx;
        ball_center.y += ball_dy;

        ball_bounds.x = ball_center.x - BALL_R;
        ball_bounds.y = ball_center.y - BALL_R;
        // ----------- drawing textures -----------

        SDL_SetRenderTarget(renderer_main ,NULL);
        SDL_SetRenderDrawColor(renderer_main , 30 , 40 , 50 , 255);
        SDL_RenderClear(renderer_main);

        SDL_SetRenderTarget(renderer_main , wall_txt);
        SDL_SetRenderDrawColor(renderer_main , 135 , 180 , 225 , 255);
        SDL_RenderClear(renderer_main);

        SDL_SetRenderTarget(renderer_main , NULL);
        SDL_RenderCopy(renderer_main , back_txt , NULL , &window_bounds);
        filledCircleRGBA(renderer_main , ball_center.x , ball_center.y , BALL_R ,BALL_CLR_R , BALL_CLR_G ,BALL_CLR_B, 255);
        SDL_RenderCopy(renderer_main,wall_txt , NULL , &wall);
        SDL_RenderPresent(renderer_main);

        // ----------- check for Events -----------
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                should_loop = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == 'q')
                {
                    should_loop = false;
                }
                if(event.key.keysym.sym == 'd'){
                	ball_dx +=1;
                }
                if(event.key.keysym.sym == 'a'){
                	ball_dx -=1;
                }
                if(event.key.keysym.sym == 'w'){
                	ball_dy-=1;
                }
                if(event.key.keysym.sym == 's'){
                	ball_dy+=1;
                }
                break;
            }
        }
        SDL_Delay(17);
    }

    SDL_DestroyTexture(wall_txt);
    SDL_DestroyTexture(back_txt);
    SDL_DestroyRenderer(renderer_main);
    SDL_DestroyWindow(win_main);

    wall_txt = NULL;
    back_txt = NULL;
    renderer_main = NULL;
    win_main = NULL;

    IMG_Quit();
    SDL_Quit();

    return 0;
}
