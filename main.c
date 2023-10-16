#include <stdbool.h>
#include <stdio.h>
// #include <time.h>
#include "input.c"
#include <SDL2/SDL.h>
#include "settings.c"

// #define DELTA_TIME ((float)(clock() - start))/CLOCKS_PER_SEC
#define DELTA_TIME ((float)(SDL_GetTicks() - start))*0.001


#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define range(a, b) (abs(a) <= b)
#define sign(x) (x > 0) - (x < 0)
#define abs(x) ((signed int)x)*(sign((signed int)x))
// clock_t start;
unsigned int start;
SDL_Event window_event;
SDL_Window *win;
SDL_Renderer * renderer;

typedef struct ball {
    float x;
    float y;
    float x_speed;
    float y_speed;
} ball;

typedef struct player {
    const bool is_right;
    unsigned short int score;
    float location;
    float speed;
    const unsigned short int length;
} player;

typedef struct window
{
    const unsigned int width;
    const unsigned int height;
    const SDL_Rect rect;
    // char * field;
} window;

typedef struct game
{
    bool over;
    ball b;
    player p1, p2;
} game;


window main_window = {
    .width = WIDTH,
    .height = HEIGHT,
    .rect = {
        .h = HEIGHT,
        .w = WIDTH
    }
};

game main_game = {
    .over = false,
    .b = {
            .x = 0,
            .y = 0,
            .x_speed = BALL_SPEED,
            .y_speed = 0
        },
    .p1 = {
        .is_right = true,
        .score = 0,
        .speed = 0,
        .length = LEN,
        .location = 0
    },
    .p2 = {
        .is_right = false,
        .score = 0,
        .speed = 0,
        .length = LEN,
        .location = 0
    }
};
void edge_bounce(ball * b){
    b->y_speed*= -1;
}
bool touching_padel(float location, signed int y, unsigned int length){
    // printf("%i\n", (location) <= (y-(BALL_WIDTH/2)) && (location + length) >= (y-(BALL_WIDTH/2)));
    // printf("%i\n", // ((location) <= (y-(BALL_WIDTH/2)) && (location + length) >= (y-(BALL_WIDTH/2)));// ||
    //             // (
    //             // (location)
    //             //  <= 
    //             //  (y+(BALL_WIDTH/2))
    //             //  &&
    //             (location + length)
    //             >=
    //              (y+(BALL_WIDTH/2))
    //             );
    return ((location) <= (y-(BALL_WIDTH/2)) && (location + length) >= (y-(BALL_WIDTH/2))) ||
            ((location) <= (y+(BALL_WIDTH/2)) && (location + length) >= (y+(BALL_WIDTH/2)));
}

void bounce_or_score(player * recieving_player, player * other_player, ball * b)
{
    if (touching_padel(recieving_player->location, b->y, recieving_player->length)){
        b->x_speed *= -SPEED_BOOST; 
        edge_bounce(b);
        b->y_speed += recieving_player->speed;
    } 
    else{
        b->x = 0;
        b->y = 0;
        b->x_speed = BALL_SPEED;
        b->y_speed = 0;
        other_player->score++;
    }
}
void update_game(game * p_g, float dt){
    // printf("%f\n",(float)(sign(p_g->p1.speed)));
    p_g->p1.speed = ((signed short int)(P1_DOWN) - (signed short int)(P1_UP))*PLAYER_ACCEL;//*dt;  // accel
    // p_g->p1.speed = p_g->p1.speed - (float)(sign(p_g->p1.speed)* (float)(!(P1_DOWN || P1_UP)) * (min(PLAYER_DECEL*dt, dt*abs(p_g->p1.speed)))); // decel
    p_g->p2.speed = ((signed short int)(P2_DOWN) - (signed short int)(P2_UP))*PLAYER_ACCEL;//*dt; // accel
    // p_g->p2.speed = p_g->p2.speed - (float)(sign(p_g->p2.speed)* (float)(!(P2_DOWN || P2_UP)) * (min(PLAYER_DECEL*dt, dt*abs(p_g->p2.speed)))); // decel

    p_g->p1.location += (float) ((p_g->p1.speed)*(dt));
    p_g->p2.location += (float) ((p_g->p2.speed)*(dt));

    p_g-> p1.location = (float)max(-(float)(main_window.height/2), (float)min( p_g-> p1.location, (float)main_window.height/2 - p_g->p1.length));
    p_g-> p2.location = (float)max(-(float)(main_window.height/2), (float)min( p_g-> p2.location, (float)main_window.height/2 - p_g->p2.length));
    p_g->b.x = (float) max( -(float)(main_window.width/2) + BALL_WIDTH, min((p_g->b.x_speed)*dt + p_g->b.x , (float)(main_window.width/2) - BALL_WIDTH));
    p_g->b.y = (float) max( -(float)(main_window.height/2), min((p_g->b.y_speed)*dt + p_g->b.y, (float)(main_window.height/2)));
    if (p_g->b.x == main_window.width/2 - BALL_WIDTH){ // check player 2
        bounce_or_score(&(p_g->p2), &(p_g->p1), &(p_g->b));
    }
    else if (p_g->b.x ==  -((signed int)main_window.width/2) + BALL_WIDTH) // check player 1
    {
        bounce_or_score(&(p_g->p1), &(p_g->p2), &(p_g->b));
    }

    if ((p_g -> b.y == -((signed int)main_window.height/2)) || (p_g -> b.y == (signed int)main_window.height/2))
    {
        edge_bounce(&(p_g->b));
    } 
    
    p_g->over = (p_g->p1.score >= MAX_SCORE || p_g->p2.score >= MAX_SCORE);
    
}



void draw()
{
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
    SDL_RenderFillRect(renderer, &(main_window.rect));
    // for (int i = 0; i < main_window.width; i++){
    //     for (int j = 0; j < main_window.height; j++){
    //         SDL_RenderDrawPoint(renderer, i, j);
    //     }
    // }
    // SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_Rect b = {
        .h = BALL_WIDTH,
        .w = BALL_WIDTH,
        .x = (int)main_game.b.x + (int)((main_window.width - BALL_WIDTH)/2),
        .y = (int)main_game.b.y + (int)((main_window.height - BALL_WIDTH)/2)
    };

    SDL_RenderFillRect(renderer, &b);
    
    // for (int i = main_game.b.y + ((main_window.height - BALL_WIDTH)/2) ;
    //              i < main_game.b.y + ((main_window.height + BALL_WIDTH)/2); i++){
    //     for (int j = main_game.b.x + ((main_window.width - BALL_WIDTH)/2);
    //              j < main_game.b.x + ((main_window.width + BALL_WIDTH)/2); j++){
    //         // if (range(i - (main_game.b.y + (main_window.height / 2)), BALL_WIDTH) && range(j -(main_game.b.x + (main_window.width / 2)), BALL_WIDTH)){
    //         //     ch = FULL;
    //         SDL_RenderDrawPoint(renderer, j, i);
    //         // }
    //         // main_window.field[i * main_window.width + j] = ch; // render
    //     }
    // }
    for (int i = (int)main_game.p1.location + (main_window.height/2);
         i < main_game.p1.location + (main_window.height/2) + main_game.p1.length; i++){
        for (int j = 0; j < PADDLE_WIDTH; j++){
            // main_window.field[(i + main_game.p1.location + (main_window.height / 2)) * main_window.width + main_window.width - 1 ] = FULL;
            SDL_RenderDrawPoint(renderer, j, i);
        }
    }
    for (int i = (int)main_game.p2.location + (main_window.height/2);
         i < main_game.p2.location + (main_window.height/2) + main_game.p2.length; i++){
        for (int j = 0; j < PADDLE_WIDTH; j++){
            // main_window.field[(i + main_game.p2.location + (main_window.height / 2)) * main_window.width] = FULL;
            SDL_RenderDrawPoint(renderer, main_window.width - j, i);
        }
    }
    SDL_RenderPresent(renderer);
}

void init() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &win, &renderer);

    SDL_RenderSetScale(renderer, 1, 1);
    
    start = SDL_GetTicks();
    // main_window.field = (char *)malloc(sizeof(char) * main_window.width * main_window.height);
    // for (int i = 0; i < main_window.height; i++){
    //     for (int j = 0; j < main_window.width; j++){
    //         main_window.field[i * main_window.width + j] = FULL; // render
    //     }
    // }
    
}
void update(float dt)
{
    if (SDL_PollEvent(&window_event))
    {
        if (window_event.type == SDL_QUIT){
            main_game.over = true;
        }
    }
    // start = clock();
    start = SDL_GetTicks();
    update_game(&main_game, dt);
    draw();
}

// int SDLMAIN_DECLSPEC SDL_main(int argc, char *argv[])
int main(int argc, char *argv[])
{
    char title[] = "pong";
    init();
    SDL_SetWindowTitle(win, &title);
    while (!main_game.over)
    {
        
        check_press(&window_event);
        float dtime = DELTA_TIME;
        if (dtime >= (1.0/FRAME_RATE)){
            update(dtime);
            // printf("%i\n",main_game.p2.speed);

            sprintf(&title, "pong    fps: %f\n", 1.0/dtime);
            SDL_SetWindowTitle(win, &title);
            // for (int i = 0; i < main_window.height; i++){
            //     for (int j = 0; j < main_window.width; j++){
            //         // printf("%c",(unsigned char)main_window.field[i*main_window.width + j]);
            //     }
            //     // printf("\n");
            // }
            // printf("\n");
            // printf("%i\n", P1_UP);
            // printf("%i\n", P1_DOWN); 
            // printf("%i\n", P2_UP );
            // printf("%i\n", P2_DOWN);
        }

        //printf("%i\n",DELTA_TIME);
        
    }
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}
