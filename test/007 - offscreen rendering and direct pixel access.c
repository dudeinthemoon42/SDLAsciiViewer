#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define NUM_THREADS 4

SDL_Event event;
SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *screen_texture = NULL;
int i, j;
int threads_ret[NUM_THREADS];
SDL_Thread *threads[NUM_THREADS];
//Uint32 *pixels;
volatile Uint32 pixels[sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT];

typedef struct {
    int x;
    int y;
    int x_to;
    int y_to;
} cord;

void render(cord *ptr) {
    // iterate through each pixel on the screen
    for(i = ptr->x; i < ptr->x_to; i++) {
        for(j = ptr->y; j < ptr->y_to; j++) {
            // create a random pixel
            Uint32 pixel = rand();
            // set the pixel
            pixels[(i * SCREEN_WIDTH) + j] = pixel;
        }
    }
    //free(ptr);
}

void exit_msg(char *msg) {
    printf(msg);
    exit(-1);
}

void cleanup() {
    // free image
    SDL_FreeSurface(screen);
    // free image texture
    SDL_DestroyTexture(screen_texture);
    // free renderer
    SDL_DestroyRenderer(renderer);
    // free window
    SDL_DestroyWindow(window);
    // quite SDL
    SDL_Quit();
}

int main(int argc, char* args[]) {
    // seed random number
    srand(time(NULL));

    // setup SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
        exit_msg("Couldn't init SDL");

    // setup SDL window
    window = SDL_CreateWindow("SDL Examples", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == NULL)
        exit_msg("Couldn't init SDL Window");

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // create screen surface
    screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    // create screen texture
    screen_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Thread *thread;
    int threadRet;

    while(1) {
        // check for pending events
        while(SDL_PollEvent(&event)) {
            // quit was requested
            if(event.type == SDL_QUIT) {
                cleanup();
                return 0;
            // keyboard button was hit
            } else if (event.type == SDL_KEYDOWN) {
                // check which key was hit
                switch(event.key.keysym.sym) {
                    // quit
                    case 'q':
                        cleanup();
                        return 0;
                }
            }
        }

        // fill background with black
        SDL_FillRect(screen, NULL, 0x00000000);
        /*
        // get the SDL surface pixels
        Uint32 *pixels = (Uint32 *)screen->pixels;
        // iterate through each pixel on the screen
        for(i = 0; i < SCREEN_HEIGHT; i++) {
            for(j = 0; j < SCREEN_WIDTH; j++) {
                // create a random pixel
                Uint32 pixel = rand();
                // set the pixel
                pixels[(i * SCREEN_WIDTH) + j] = pixel;
            }
        }*/

        // get the SDL surface pixels
        memcpy(pixels, screen->pixels, sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);

        for(i = 0; i < NUM_THREADS; i++) {
            char s[2];
            cord *cords = malloc(sizeof(cords));
            cords->x = (SCREEN_HEIGHT / NUM_THREADS) * i;
            cords->y = 0;
            cords->x_to = (SCREEN_HEIGHT / NUM_THREADS) * (i + 1);
            cords->y_to = SCREEN_WIDTH;
            SDL_itoa(i, &s, 10);
            threads[i] = SDL_CreateThread(render, s, cords);
        }

        for(i = 0; i < NUM_THREADS; i++) {
            SDL_WaitThread(threads[i], &threads_ret[i]);
        }

        for(i = 0; i < NUM_THREADS; i++) {
            printf("%d\n", threads_ret[i]);
        }

        printf("four done\n");

        // update screen texture
        SDL_UpdateTexture(screen_texture, NULL, &pixels, screen->pitch);
        // clear renderer
        SDL_RenderClear(renderer);
        // copy image into renderer to be rendered
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        // render image
        SDL_RenderPresent(renderer);
     }

    return 0;
}
