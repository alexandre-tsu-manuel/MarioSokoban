#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include "constantes.h"
#include "fichiers.h"
#include "jeu.h"
#include "exit.h"
#include "editeur.h"

SDL_Surface* screen;

int main(int argc, char* argv[])
{
    SDL_Surface* menu;
    SDL_Event input;
    SDL_Rect menuPos;

    SDL_Init(SDL_INIT_VIDEO);
    screen = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN);
    SDL_WM_SetCaption("TP Mario Sokoban", NULL);
    SDL_EnableKeyRepeat(100, 100);

    menu = IMG_Load("images/menu.jpg");
    menuPos.x = (LARGEUR_FENETRE - menu->w) / 2;
    menuPos.y = (HAUTEUR_FENETRE - menu->h) / 2;
    while (SDL_WaitEvent(&input))
    {
        switch (input.type)
        {
            case SDL_QUIT:
                quit();
            case SDL_KEYDOWN:
                switch (input.key.keysym.sym)
                {
                    case SDLK_1:
                    case SDLK_KP1:
                        loadGame(1);
                        break;
                    case SDLK_2:
                    case SDLK_KP2:
                        LoadEditor();
                        break;
                    case SDLK_3:
                    case SDLK_KP3:
                        loadGame(0);
                        break;
                    case SDLK_ESCAPE:
                        quit();
                    default: ;
                }
        }
        SDL_FillRect(screen, NULL, 0);
        SDL_BlitSurface(menu, NULL, screen, &menuPos);
        SDL_Flip(screen);
    }

    printf("error while acquiring event in function main line %d in %s", __LINE__, __FILE__);
    SDL_FreeSurface(menu);
    SDL_Quit();
    return EXIT_FAILURE;
}
