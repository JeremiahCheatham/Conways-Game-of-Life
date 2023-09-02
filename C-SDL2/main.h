#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#define SDL_FLAGS (SDL_INIT_VIDEO|SDL_INIT_AUDIO|SDL_INIT_TIMER)
#define WINDOW_TITLE "Conway's Game of Life"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define TARGET_FPS 60
#define SIZE 10
#define FONT_SIZE 24

#endif