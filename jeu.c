#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_Image.h>
#include <windows.h>
#include "constantes.h"
#include "fichiers.h"
#include "jeu.h"
#include "exit.h"

extern SDL_Surface* screen;

void loadGame(int campaign)
{
    int map[NB_BLOCS], lvl;
    int numberOfMaps;
    int marioPos;
    SDL_Surface* tiles[5];
    SDL_Surface* marioTiles[4];
    SDL_Surface* theEnd = NULL;
    SDL_Surface* great = NULL;

    loadTiles(tiles, marioTiles, &theEnd, &great);
    loadControls(1);
    if (campaign)
    {
        numberOfMaps = getNumberOfMaps();
        if (numberOfMaps <= 0)
            error("no levels detected in levels.msf\n");
        for (lvl = 1 ; lvl <= numberOfMaps ; lvl++)
        {
            getMap(map, lvl);
            marioPos = placeMario(map);
            if (play(map, tiles, marioTiles, marioPos))
                lvl--;
            else if (!haveWon(map))
                break;
            else
                nextLevel(great);
        }
        if (haveWon(map))
            win(theEnd);
    }
    else
    {
        do
        {
            getPersoMap(map);
            marioPos = placeMario(map);
        } while(play(map, tiles, marioTiles, marioPos));
        if (haveWon(map))
            nextLevel(great);
    }
    freeTiles(tiles, marioTiles, theEnd, great);
}

int placeMario(int map[])
{
    int marioPos = 0;
    int i;

    for (i = 0 ; i < NB_BLOCS ; i++)
    {
        if (map[i] == MARIO)
        {
            marioPos = i;
            map[i] = VIDE;
        }
    }

    return marioPos;
}

int play(int map[], SDL_Surface* tiles[], SDL_Surface* marioTiles[], int marioPos)
{
    int marioDirection = BAS;
    SDL_Event mov;

    while (!haveWon(map) && SDL_WaitEvent(&mov))
    {
        switch (mov.type)
        {
            case SDL_QUIT:
                quit();
            case SDL_KEYDOWN:
                switch (mov.key.keysym.sym)
                {
                    case SDLK_UP:
                        marioDirection = HAUT;
                        movePlayer(map, &marioPos, marioDirection);
                        break;
                    case SDLK_DOWN:
                        marioDirection = BAS;
                        movePlayer(map, &marioPos, marioDirection);
                        break;
                    case SDLK_LEFT:
                        marioDirection = GAUCHE;
                        movePlayer(map, &marioPos, marioDirection);
                        break;
                    case SDLK_RIGHT:
                        marioDirection = DROITE;
                        movePlayer(map, &marioPos, marioDirection);
                        break;
                    case SDLK_r:
                        return 1;
                    case SDLK_ESCAPE:
                        return 0;
                    default: ;
                }
                break;
        }
        displayMap(map, tiles);
        displayMario(marioTiles[marioDirection], marioPos);
        SDL_Flip(screen);
    }
    return 0;
}

void movePlayer(int map[], int* marioPos, int direction)
{
    switch (direction)
    {
        case HAUT:
            if (*marioPos < NB_BLOCS_LARGEUR)
                return;
            if (map[*marioPos - NB_BLOCS_LARGEUR] == MUR)
                return;
            if (map[*marioPos - NB_BLOCS_LARGEUR] == CAISSE || map[*marioPos - NB_BLOCS_LARGEUR] == CAISSE_OK)
            {
                if (*marioPos < 2 * NB_BLOCS_LARGEUR)
                    return;
                if (map[*marioPos - 2 * NB_BLOCS_LARGEUR] == VIDE)
                    map[*marioPos - 2 * NB_BLOCS_LARGEUR] = CAISSE;
                else if (map[*marioPos - 2 * NB_BLOCS_LARGEUR] == OBJECTIF)
                    map[*marioPos - 2 * NB_BLOCS_LARGEUR] = CAISSE_OK;
                else
                    return;
                map[*marioPos - NB_BLOCS_LARGEUR] = map[*marioPos - NB_BLOCS_LARGEUR] == CAISSE ? VIDE : OBJECTIF;
            }
            *marioPos -= NB_BLOCS_LARGEUR;
            return;
        case BAS:
            if (*marioPos + NB_BLOCS_LARGEUR >= NB_BLOCS)
                return;
            if (map[*marioPos + NB_BLOCS_LARGEUR] == MUR)
                return;
            if (map[*marioPos + NB_BLOCS_LARGEUR] == CAISSE || map[*marioPos + NB_BLOCS_LARGEUR] == CAISSE_OK)
            {
                if (*marioPos + 2 * NB_BLOCS_LARGEUR > NB_BLOCS)
                    return;
                if (map[*marioPos + 2 * NB_BLOCS_LARGEUR] == VIDE)
                    map[*marioPos + 2 * NB_BLOCS_LARGEUR] = CAISSE;
                else if (map[*marioPos + 2 * NB_BLOCS_LARGEUR] == OBJECTIF)
                    map[*marioPos + 2 * NB_BLOCS_LARGEUR] = CAISSE_OK;
                else
                    return;
                map[*marioPos + NB_BLOCS_LARGEUR] = map[*marioPos + NB_BLOCS_LARGEUR] == CAISSE ? VIDE : OBJECTIF;
            }
            *marioPos += NB_BLOCS_LARGEUR;
            return;
        case GAUCHE:
            if (*marioPos % NB_BLOCS_LARGEUR == 0)
                return;
            if (map[*marioPos - 1] == MUR)
                return;
            if (map[*marioPos - 1] == CAISSE || map[*marioPos - 1] == CAISSE_OK)
            {
                if (*marioPos % NB_BLOCS_LARGEUR < 2)
                    return;
                if (map[*marioPos - 2] == VIDE)
                    map[*marioPos - 2] = CAISSE;
                else if (map[*marioPos - 2] == OBJECTIF)
                    map[*marioPos - 2] = CAISSE_OK;
                else
                    return;
                map[*marioPos - 1] = map[*marioPos - 1] == CAISSE ? VIDE : OBJECTIF;
            }
            *marioPos -= 1;
            return;
        case DROITE:
            if (*marioPos % NB_BLOCS_LARGEUR == NB_BLOCS_LARGEUR - 1)
                return;
            if (map[*marioPos + 1] == MUR)
                return;
            if (map[*marioPos + 1] == CAISSE || map[*marioPos + 1] == CAISSE_OK)
            {
                if (*marioPos % NB_BLOCS_LARGEUR + 2 > NB_BLOCS_LARGEUR - 1)
                    return;
                if (map[*marioPos + 2] == VIDE)
                    map[*marioPos + 2] = CAISSE;
                else if (map[*marioPos + 2] == OBJECTIF)
                    map[*marioPos + 2] = CAISSE_OK;
                else
                    return;
                map[*marioPos + 1] = map[*marioPos + 1] == CAISSE ? VIDE : OBJECTIF;
            }
            *marioPos += 1;
            return;
    }
}

void displayMario(SDL_Surface* marioTile, int marioPos)
{
    SDL_Rect marioRectPos;

    marioRectPos.x = (marioPos % NB_BLOCS_LARGEUR) * TAILLE_BLOC;
    marioRectPos.y = marioPos / NB_BLOCS_LARGEUR * TAILLE_BLOC;
    SDL_BlitSurface(marioTile, NULL, screen, &marioRectPos);
}

int haveWon(int map[])
{
    int i;

    for (i = 0 ; i < NB_BLOCS ; i++)
        if (map[i] == OBJECTIF)
            return 0;

    return 1;
}

void win(SDL_Surface* theEnd)
{
    SDL_Rect theEndPos;
    SDL_Event event;

    theEndPos.x = (NB_BLOCS_LARGEUR * TAILLE_BLOC - theEnd->w) / 2;
    theEndPos.y = (NB_BLOCS_HAUTEUR * TAILLE_BLOC - theEnd->h) / 2;
    SDL_BlitSurface(theEnd, NULL, screen, &theEndPos);
    SDL_Flip(screen);
    while (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
            quit();
        if (event.type == SDL_KEYDOWN)
            break;
    }
}

void nextLevel(SDL_Surface* great)
{
    SDL_Rect greatPos;
    SDL_Event event;

    greatPos.x = (NB_BLOCS_LARGEUR * TAILLE_BLOC - great->w) / 2;
    greatPos.y = (NB_BLOCS_HAUTEUR * TAILLE_BLOC - great->h) / 2;
    SDL_BlitSurface(great, NULL, screen, &greatPos);
    SDL_Flip(screen);
    while (SDL_WaitEvent(&event))
    {
        if (event.type == SDL_QUIT)
            quit();
        if (event.type == SDL_KEYDOWN)
            break;
    }
}
