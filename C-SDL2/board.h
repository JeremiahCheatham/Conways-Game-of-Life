#ifndef BOARD_H
#define BOARD_H

#include "main.h"

struct Board {
    SDL_Renderer *renderer;
    SDL_Surface *surface;
    SDL_Texture *image;
    bool *board;
    bool *new_board;
    int rows;
    int columns;
    double delay_target;
    double delay_count;
    SDL_Rect rect;
};

bool board_new(struct Board **board, SDL_Renderer *renderer);
void board_free(struct Board **board);
void board_reset(struct Board *b);
void board_clear(struct Board *b);
void board_click(struct Board *b, int x, int y);
double board_speed(struct Board *b);
bool board_increase_speed(struct Board *b);
bool board_decrease_speed(struct Board *b);
void board_update(struct Board *b, double dt);
bool board_update_life(struct Board *b, int row, int column);
bool board_draw(struct Board *b);

#endif