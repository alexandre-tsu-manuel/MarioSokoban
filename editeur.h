#ifndef DEF_EDITOR
#define DEF_EDITOR

    void LoadEditor();
    void edit(int map[], SDL_Surface* tiles[], SDL_Surface* confirm);
    int confirmQuit(int map[], SDL_Surface* confirm);

#endif
