#ifndef DEF_FICHIERS
#define DEF_FICHIERS

    void getMap(int map[], int mapID);
    void getPersoMap(int map[]);
    void setPersoMap(int map[]);
    int getNumberOfMaps();
    void loadEditTiles(SDL_Surface* tiles[]);
    void freeEditTiles(SDL_Surface* tiles[]);
    void loadTiles(SDL_Surface* tiles[], SDL_Surface* marioTiles[], SDL_Surface** theEnd, SDL_Surface** great);
    void freeTiles(SDL_Surface* tiles[], SDL_Surface* marioTiles[], SDL_Surface* theEnd, SDL_Surface* great);
    void loadControls(int playing);
    void loadInfosBox(SDL_Surface** confirm);
    void freeInfosBox(SDL_Surface* confirm);
    void displayMap(int map[], SDL_Surface* tiles[]);

#endif
