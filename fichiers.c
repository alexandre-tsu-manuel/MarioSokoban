#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include "constantes.h"
#include "fichiers.h"
#include "exit.h"

extern SDL_Surface* screen;

void getMap(int map[], int mapID)
{
    int i;
    char empty[NB_BLOCS + 3];
    FILE* input = fopen("maps/levels.msf", "r");
    if (!input)
        error("can't open levels.msf\n");
    for (i = 0 ; i < mapID ; i++)
        fgets(empty, NB_BLOCS + 2, input);
    for (i = 0 ; i < NB_BLOCS ; i++)
        map[i] = fgetc(input) - '0';

    fclose(input);
}

void getPersoMap(int map[])
{
    int i;
    FILE* input = fopen("maps/PersoLevel.msf", "r");
    if (!input)
        error("can't open levels.msf\n");
    for (i = 0 ; i < NB_BLOCS ; i++)
        map[i] = fgetc(input) - '0';

    fclose(input);
}

void setPersoMap(int map[])
{
    int i;
    FILE* output = fopen("maps/PersoLevel.msf", "w+");

    if (!output)
        error("can't open persoLevel.msf\n");
    for (i = 0 ; i < NB_BLOCS ; i++)
        fputc(map[i] + '0', output);

    fclose(output);
}

int getNumberOfMaps()
{
    int total = -1;
    FILE* input = fopen("maps/levels.msf", "r");

    if (!input)
        error("can't open levels.msf\n");
    fscanf(input, "%d", &total);

    return total;
}

void loadEditTiles(SDL_Surface* tiles[])
{
    tiles[MUR] = IMG_Load("images/mur.jpg");
    tiles[CAISSE] = IMG_Load("images/caisse.jpg");
    tiles[OBJECTIF] = IMG_Load("images/objectif.png");
    tiles[CAISSE_OK] = IMG_Load("images/caisse_ok.jpg");
    tiles[MARIO] = IMG_Load("images/mario_bas.gif");
}

void freeEditTiles(SDL_Surface* tiles[])
{
    SDL_FreeSurface(tiles[MUR]);
    SDL_FreeSurface(tiles[CAISSE]);
    SDL_FreeSurface(tiles[OBJECTIF]);
    SDL_FreeSurface(tiles[CAISSE_OK]);
    SDL_FreeSurface(tiles[MARIO]);
}

void loadTiles(SDL_Surface* tiles[], SDL_Surface* marioTiles[], SDL_Surface** theEnd, SDL_Surface** great)
{
    loadEditTiles(tiles);
    marioTiles[HAUT] = IMG_Load("images/mario_haut.gif");
    marioTiles[BAS] = tiles[MARIO];
    marioTiles[GAUCHE] = IMG_Load("images/mario_gauche.gif");
    marioTiles[DROITE] = IMG_Load("images/mario_droite.gif");
    *theEnd = IMG_Load("images/bravo_fin.jpg");
    *great = IMG_Load("images/bravo.jpg");
}

void freeTiles(SDL_Surface* tiles[], SDL_Surface* marioTiles[], SDL_Surface* theEnd, SDL_Surface* great)
{
    freeEditTiles(tiles);
    SDL_FreeSurface(marioTiles[HAUT]);
    SDL_FreeSurface(marioTiles[GAUCHE]);
    SDL_FreeSurface(marioTiles[DROITE]);
    SDL_FreeSurface(theEnd);
    SDL_FreeSurface(great);
}

void loadControls(int playing)
{
    SDL_Surface* controls;
    SDL_Rect pos;

    if (playing)
        controls = IMG_Load("images/explications_jeu.png");
    else
        controls = IMG_Load("images/explications_editeur.png");
    pos.x = 0;
    pos.y = screen->h - controls->h;
    SDL_BlitSurface(controls, NULL, screen, &pos);
    SDL_FreeSurface(controls);
}

void loadInfosBox(SDL_Surface** confirm)
{
    *confirm = IMG_Load("images/confirmer.jpg");
}

void freeInfosBox(SDL_Surface* confirm)
{
    SDL_FreeSurface(confirm);
}

void displayMap(int map[], SDL_Surface* tiles[])
{
    int i;
    SDL_Rect current = {0};
    SDL_Surface* empty;

    empty = SDL_CreateRGBSurface(SDL_HWSURFACE, NB_BLOCS_LARGEUR * TAILLE_BLOC, NB_BLOCS_HAUTEUR * TAILLE_BLOC, 32, 0, 0, 0, 0);
    SDL_FillRect(empty, NULL, 16777215);
    SDL_BlitSurface(empty, NULL, screen, NULL);
    SDL_FreeSurface(empty);
    for (i = 0 ; i < NB_BLOCS ; i++)
    {
        if (map[i] != VIDE)
        {
            current.x = (i % NB_BLOCS_LARGEUR) * TAILLE_BLOC;
            current.y = i / NB_BLOCS_LARGEUR * TAILLE_BLOC;
            SDL_BlitSurface(tiles[map[i]], NULL, screen, &current);
        }
    }
}
