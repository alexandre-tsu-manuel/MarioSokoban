#include <stdio.h>
#include <SDL/SDL.h>

void quit()
{
    SDL_Quit();
    exit(EXIT_SUCCESS);
}

void error(char* err)
{
    printf(err);
    SDL_Quit();
    exit(EXIT_FAILURE);
}
