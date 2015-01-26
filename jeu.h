#ifndef DEF_JEU
#define DEF_JEU

    void loadGame(int campaign);
    int placeMario(int map[]);
    int play(int map[], SDL_Surface* tiles[], SDL_Surface* marioTiles[], int marioPos);
    void movePlayer(int map[], int* marioPos, int direction);
    void displayMario(SDL_Surface* marioTile, int marioPos);
    int haveWon(int map[]);
    void win(SDL_Surface* theEnd);
    void nextLevel(SDL_Surface* great);

#endif
