#include "game.h"
#include "initialize.h"

bool game_new(struct Game **game) {
    *game = calloc(1, sizeof(struct Game));
    if (*game == NULL) {
        fprintf(stderr, "Error in calloc of new game.\n");
        return true;
    }
    struct Game *g = *game;

    if (game_initilize(g)) {
        return true;
    }

    if (board_new(&g->board, g->renderer)) {
        return true;
    }

    if (message_new(&g->message, g->renderer, board_speed(g->board))) {
        return true;
    }

    if (fps_new(&g->fps)) {
        return true;
    }

    g->playing = true;

    return false;
}

void game_free(struct Game **game) {
    fps_free(&(*game)->fps);
    board_free(&(*game)->board);
    message_free(&(*game)->message);

    SDL_DestroyRenderer((*game)->renderer);
    (*game)->renderer = NULL;
    SDL_DestroyWindow((*game)->window);
    (*game)->window = NULL;
    TTF_Quit();
    SDL_Quit();
    free(*game);
    *game = NULL;
}

bool game_run(struct Game *g) {

    while (true) {
        while (SDL_PollEvent(&g->event)) {
            switch (g->event.type) {
                case SDL_QUIT:
                    return false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(g->event.button.button == SDL_BUTTON_LEFT){
                        board_click(g->board, g->event.button.x, g->event.button.y);
                    }
                    break;
                case SDL_KEYDOWN:
                    switch (g->event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            return false;
                            break;
                        case SDL_SCANCODE_SPACE:
                            if (g->playing) {
                                if (message_update(g->message, "Paused")) {
                                    return true;
                                }
                                g->playing = false;
                            } else {
                                if (message_update_speed(g->message, board_speed(g->board))) {
                                    return true;
                                }
                                g->playing = true;
                            }
                            break;
                        case SDL_SCANCODE_UP:
                            if (board_increase_speed(g->board)) {
                                if (message_update_speed(g->message, board_speed(g->board))) {
                                    return true;
                                }
                            }
                            break;
                        case SDL_SCANCODE_DOWN:
                            if (board_decrease_speed(g->board)) {
                                if (message_update_speed(g->message, board_speed(g->board))) {
                                    return true;
                                }
                            }
                            break;
                        case SDL_SCANCODE_R:
                            board_reset(g->board);
                            break;
                        case SDL_SCANCODE_C:
                            board_clear(g->board);
                            break;
                        case SDL_SCANCODE_F:
                            fps_toggle_display(g->fps);
                            break;
                        default:
                            break;
                    }
                default:
                    break;
            }
        }

        if (g->playing) {
            board_update(g->board, g->delta_time);
        }

        if (SDL_RenderClear(g->renderer)) {
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            return true;
        }

        if (board_draw(g->board)) {
            return true;
        }

        if (message_draw(g->message)) {
            return true;
        }
        
        SDL_RenderPresent(g->renderer);

        g->delta_time = fps_update(g->fps);
    }
    
    return false;
}