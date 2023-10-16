// #include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool P1_UP   = false;
bool P1_DOWN = false;
bool P2_UP   = false;
bool P2_DOWN = false;


// #define P1_UP check_press(SDL_SCANCODE_UP) //check_press(0) && check_press(72)
// #define P1_DOWN check_press(SDL_SCANCODE_DOWN) //check_press(0) && check_press(80)
// // #define P1_RIGHT check_press(0) && check_press(77)
// // #define P1_LEFT check_press(0) && check_press(75)

// #define P2_UP check_press(SDL_SCANCODE_W) //check_press(119) || check_press(87)
// #define P2_DOWN check_press(SDL_SCANCODE_S) //check_press(115) || check_press(83)
// // #define P2_RIGHT check_press(100) || check_press(68)
// #define P2_LEFT check_press(97) || check_press(65)

void check_press(SDL_Event * event){
    switch (event->type)
    {
    case SDL_KEYDOWN:
        switch (event->key.keysym.scancode)
        {
        case SDL_SCANCODE_UP:
            P2_UP = true;
            break;
        case SDL_SCANCODE_DOWN:
            P2_DOWN = true;
            break;
        case SDL_SCANCODE_W:
            P1_UP = true;
            break;
        case SDL_SCANCODE_S:
            P1_DOWN = true;
            break;
        default:
            break;
        }
        break;
    case SDL_KEYUP:
        switch (event->key.keysym.scancode)
        {
        case SDL_SCANCODE_UP:
            P2_UP = false;
            break;
        case SDL_SCANCODE_DOWN:
            P2_DOWN = false;
            break;
        case SDL_SCANCODE_W:
            P1_UP = false;
            break;
        case SDL_SCANCODE_S:
            P1_DOWN = false;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    // return ((event.type == SDL_KEYDOWN) && (event.key.keysym.scancode==press));
    
    // int b = kbhit();
    // if ((bool)b){
    //     int ch = getch();
    //     return (ch == press);
    // }
    // return false;
}

// int main(int argc, char *argv[])
// {
//     // setvbuf(stdout, NULL, _IONBF, 0);
//     // setvbuf(stderr, NULL, _IONBF, 0);
//     SDL_Event event;
//     while (true){

//         check_press(&event);

//         printf("%i\n", P1_UP);
//         // printf("%i\n", P1_RIGHT);
//         printf("%i\n", P1_DOWN);
//         // printf("%i\n", P1_LEFT ) ;      
//         printf("%i\n", P2_UP );
//         // printf("%i\n", P2_RIGHT );
//         printf("%i\n", P2_DOWN);
//         // printf("%i\n", P2_LEFT );
//         // printf("\n");
//         // fflush(stdout);
//         // _sleep(100);
//     }
//     return 0;
// }
