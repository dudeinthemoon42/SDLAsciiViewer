#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define NUM_THREADS 2

SDL_Event event;
SDL_Window *window = NULL;
SDL_Surface *screen = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *screen_texture = NULL;
int i, j;
Uint32 *threads_ret[NUM_THREADS];
SDL_Thread *threads[NUM_THREADS];
Uint32 *pixels;
//Uint32 pixels[sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT];

typedef struct {
    int x;
    int y;
    int x_to;
    int y_to;
    //int *data;
} cord;

void render(cord *ptr) {
    int x = 0, y = 0;
    //int s = sizeof(Uint32) * SCREEN_WIDTH * (SCREEN_HEIGHT / NUM_THREADS);
    //int *data = malloc(s);
    // iterate through each pixel on the screen
    for(x = 0; x < ptr->x_to; x++) {
        for(y = 0; y < ptr->y_to; y++) {
            // create a random pixel
            Uint32 pixel = rand();
            // set the pixel
            pixels[(x * SCREEN_WIDTH) + y] = pixel;
        }
    }
    //free(ptr);
    //ptr->data = data;
    //return data;
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
        pixels = (Uint32 *)screen->pixels;
        //memcpy(pixels, screen->pixels, sizeof(Uint32) * SCREEN_WIDTH * SCREEN_HEIGHT);

        //Uint32

        cord *cords[NUM_THREADS];
        for(i = 0; i < NUM_THREADS; i++)
            cords[i] = malloc(sizeof(cord));

        for(i = 0; i < NUM_THREADS; i++) {
            char s[2];

            cords[i]->x = (SCREEN_HEIGHT / NUM_THREADS) * i;
            cords[i]->y = 0;
            //cords->x_to = (SCREEN_HEIGHT / NUM_THREADS) * (i + 1);
            cords[i]->x_to = (SCREEN_HEIGHT / NUM_THREADS);
            cords[i]->y_to = SCREEN_WIDTH;
            //cords[i]->data = malloc(sizeof(int*));
            SDL_itoa(i, &s, 10);
            threads[i] = SDL_CreateThread(render, s, cords);
        }

        for(i = 0; i < NUM_THREADS; i++) {
            int ret;
            SDL_WaitThread(threads[i], &ret);
            //int mem = sizeof(Uint32) * SCREEN_WIDTH * (SCREEN_HEIGHT / NUM_THREADS);
            //memcpy(screen->pixels + (i * mem), cords[i]->data, mem);
            //free((void**)&threads_ret[i]);
        }

        //for(i = 0; i < NUM_THREADS; i++)
          //  free(cords[i]);

        //free(threads);
        //free(threads_ret);
        //free(*threads_ret);
        //free(&threads[0]);
        //free(&threads[1]);
        //free(threads_ret);

        for(i = 0; i < NUM_THREADS; i++) {
            printf("%d\n", threads_ret[i]);
        }

        printf("four done\n");

        // update screen texture
        SDL_UpdateTexture(screen_texture, NULL, screen->pixels, screen->pitch);
        // clear renderer
        SDL_RenderClear(renderer);
        // copy image into renderer to be rendered
        SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
        // render image
        SDL_RenderPresent(renderer);
     }

    return 0;
}
