#ifndef GAME_H
#define GAME_H

#include "main.h"
#include "board.h"
#include "message.h"
#include "fps.h"

struct Game {
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    struct Board *board;
    struct Message *message;
    struct Fps *fps;
    bool playing;
    double delta_time;
};

bool game_new(struct Game **game);
void game_free(struct Game **game);
bool game_run(struct Game *g);

#endif