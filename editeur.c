#include <SDL/SDL.h>
#include "constantes.h"
#include "fichiers.h"
#include "exit.h"
#include "editeur.h"

extern SDL_Surface* screen;

void LoadEditor()
{
    int map[NB_BLOCS];
    SDL_Surface* tiles[5];
    SDL_Surface* confirm;

    loadEditTiles(tiles);
    loadInfosBox(&confirm);
    loadControls(0);
    getPersoMap(map);
    edit(map, tiles, confirm);
    freeEditTiles(tiles);
    freeInfosBox(confirm);
}

void edit(int map[], SDL_Surface* tiles[], SDL_Surface* confirm)
{
    int unsavedModifs = 0;
    int currentBlock = VIDE;
    int target = -1;
    int i;
    int currentClick;
    SDL_Event event;
    SDL_Rect pos;

    while (SDL_WaitEvent(&event))
    {
        displayMap(map, tiles);
        switch (event.type)
        {
            case SDL_QUIT:
                quit();
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        if (!unsavedModifs || confirmQuit(map, confirm))
                            return;
                    case SDLK_r:
                        getPersoMap(map);
                        unsavedModifs = 0;
                        break;
                    case SDLK_s:
                        setPersoMap(map);
                        unsavedModifs = 0;
                        break;
                    case SDLK_1:
                    case SDLK_KP1:
                        currentBlock = MARIO;
                        break;
                    case SDLK_2:
                    case SDLK_KP2:
                        currentBlock = OBJECTIF;
                        break;
                    case SDLK_3:
                    case SDLK_KP3:
                        currentBlock = CAISSE;
                        break;
                    case SDLK_4:
                    case SDLK_KP4:
                        currentBlock = MUR;
                        break;
                    default: ;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.x == LARGEUR_FENETRE)
                    event.button.x--;
                target = event.button.y / TAILLE_BLOC * NB_BLOCS_LARGEUR + event.button.x / TAILLE_BLOC;
                switch(event.button.button)
                {
                    case SDL_BUTTON_MIDDLE:
                        if (target >= 0 && target < NB_BLOCS)
                            currentBlock = map[target] == CAISSE_OK ? CAISSE : map[target];
                        break;
                    case SDL_BUTTON_WHEELUP:
                        if (currentBlock == MUR)
                            currentBlock = CAISSE;
                        else if (currentBlock == CAISSE)
                            currentBlock = OBJECTIF;
                        else if (currentBlock == OBJECTIF)
                            currentBlock = MARIO;
                        else
                            currentBlock = MUR;
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        if (currentBlock == MARIO)
                            currentBlock = OBJECTIF;
                        else if (currentBlock == OBJECTIF)
                            currentBlock = CAISSE;
                        else if (currentBlock == CAISSE)
                            currentBlock = MUR;
                        else
                            currentBlock = MARIO;
                        break;
                }
                break;
            case SDL_MOUSEMOTION:
                pos.x = event.motion.x - TAILLE_BLOC / 2;
                pos.y = event.motion.y - TAILLE_BLOC / 2;
                if (pos.y < 0)
                    pos.y = 0;
                else if (pos.y > HAUTEUR_FENETRE - HAUTEUR_AIDE - TAILLE_BLOC)
                    pos.y = HAUTEUR_FENETRE - HAUTEUR_AIDE - TAILLE_BLOC;
                if (pos.x < 0)
                    pos.x = 0;
                else if (pos.x > LARGEUR_FENETRE - TAILLE_BLOC)
                    pos.x = LARGEUR_FENETRE - TAILLE_BLOC;
                if (event.motion.x == LARGEUR_FENETRE)
                    event.motion.x--;
                target = event.motion.y / TAILLE_BLOC * NB_BLOCS_LARGEUR + event.motion.x / TAILLE_BLOC;
        }
        currentClick = SDL_GetMouseState(NULL, NULL);
        if (currentClick & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if (target >= 0 && target < NB_BLOCS)
            {
                if (currentBlock == MARIO)
                    for (i = 0 ; i < NB_BLOCS ; i++)
                        if (map[i] == MARIO)
                            map[i] = VIDE;
                map[target] = (map[target] == OBJECTIF || map[target] == CAISSE_OK) && currentBlock == CAISSE ? CAISSE_OK : currentBlock;
                unsavedModifs = 1;
            }
        }
        if (currentClick & SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            if (target >= 0 && target < NB_BLOCS)
            {
                map[target] = map[target] == CAISSE_OK ? CAISSE : VIDE;
                unsavedModifs = 1;
            }
        }
        if (currentBlock != VIDE)
            SDL_BlitSurface(tiles[currentBlock], NULL, screen, &pos);
        SDL_Flip(screen);
    }
}

int confirmQuit(int map[], SDL_Surface* confirm)
{
    SDL_Rect pos;
    SDL_Event event;

    pos.x = (LARGEUR_FENETRE - confirm->w) / 2;
    pos.y = (NB_BLOCS_HAUTEUR * TAILLE_BLOC - confirm->h) / 2;
    SDL_BlitSurface(confirm, NULL, screen, &pos);
    SDL_Flip(screen);
    while(SDL_WaitEvent(&event))
    {
        switch(event.type)
        {
            case SDL_QUIT:
                quit();
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (event.button.y > pos.y + 82 && event.button.y < pos.y + 105)
                    {
                        if (event.button.x > pos.x + 10 && event.button.x < pos.x + 67)
                        {
                            setPersoMap(map);
                            return 1;
                        }
                        if (event.button.x > pos.x + 74 && event.button.x < pos.x + 131)
                            return 1;
                        if (event.button.x > pos.x + 138 && event.button.x < pos.x + 195)
                            return 0;
                    }
                }
        }
    }
    return 0;
}
