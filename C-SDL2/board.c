#include "board.h"

bool board_new(struct Board **board, SDL_Renderer *renderer) {
    *board = calloc(1, sizeof(struct Board));
    if (*board == NULL) {
        fprintf(stderr, "Error in calloc of new Board.\n");
        return true;
    }
    struct Board *b = *board;

    b->renderer = renderer;
    b->rows = WINDOW_HEIGHT / SIZE;
    b->columns = WINDOW_WIDTH / SIZE;
    b->delay_target = 16;

    b->board = calloc(1, sizeof(bool) * (Uint64)(b->rows * b->columns));
    if (b->board == NULL) {
        fprintf(stderr, "Error in calloc of new board.\n");
        return true;
    }

    b->new_board = calloc(1, sizeof(bool) * (Uint64)(b->rows * b->columns));
    if (b->new_board == NULL) {
        fprintf(stderr, "Error in calloc of new board.\n");
        return true;
    }

    b->surface = SDL_CreateRGBSurface(0,SIZE, SIZE,32,0,0,0,0);
    if (!b->surface) {
        fprintf(stderr, "Error creating a target text surface: %s\n", SDL_GetError());
        return true;
    }

    Uint32 color = SDL_MapRGB(b->surface->format, 127, 127, 127);

    if (SDL_FillRect(b->surface, &(SDL_Rect){1, 1, SIZE - 2, SIZE - 2}, color)) {
        fprintf(stderr, "Error creating a filled rectangle: %s\n", SDL_GetError());
        return true;
    }

    b->image = SDL_CreateTextureFromSurface(b->renderer, b->surface);
    if (!b->image) {
        fprintf(stderr, "Error creating a text texture: %s\n", SDL_GetError());
        return true;
    }

    if (SDL_QueryTexture(b->image, NULL, NULL, &b->rect.w, &b->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(b->surface);
    b->surface = NULL;

    board_reset(b);

    return false;
}

void board_free(struct Board **board) {
    if (*board) {
        free((*board)->board);
        (*board)->board = NULL;
        free((*board)->new_board);
        (*board)->new_board = NULL;
        SDL_FreeSurface((*board)->surface);
        (*board)->surface = NULL;
        SDL_DestroyTexture((*board)->image);
        (*board)->image = NULL;
        (*board)->renderer = NULL;
        free(*board);
        *board = NULL;
    }
}

void board_reset(struct Board *b) {
    for (int row = 0; row < b->rows * b->columns; row += b->columns) {
        for (int column = 0; column < b->columns; column++) {
            if (rand() % 5 == 0) {
                b->board[row + column] = true;
            } else {
                b->board[row + column] = false;
            }
        }
    }
}

void board_clear(struct Board *b) {
    for (int row = 0; row < b->rows * b->columns; row += b->columns) {
        for (int column = 0; column < b->columns; column++) {
            b->board[row + column] = false;
        }
    }
}

void board_click(struct Board *b, int x, int y) {
    int column = x / SIZE;
    int row = y / SIZE;
    if (b->board[row * b->columns + column]) {
        b->board[row * b->columns + column] = false;
    } else {
        b->board[row * b->columns + column] = true;
    }

}

double board_speed(struct Board *b) {
   return 8 / b->delay_target; 
}

bool board_increase_speed(struct Board *b) {
    if (b->delay_target > 1) {
        b->delay_target /= 2;
        return true;
    }
    return false;
}

bool board_decrease_speed(struct Board *b) {
    if (b->delay_target < 64) {
        b->delay_target *= 2;
        return true;
    }
    return false;
}

void board_update(struct Board *b, double dt) {
    if (b->delay_count < b->delay_target) {
        b->delay_count += 60 * dt;
    } else {
        for (int row = 0; row < b->rows; row++) {
            for (int column = 0; column < b->columns; column++) {
                b->new_board[row * b->columns + column] = board_update_life(b, row, column);
            }
        }
        bool *old = b->board;
        b->board = b->new_board;
        b->new_board = old;
        b->delay_count = 1;
    }
}

bool board_update_life(struct Board *b, int row, int column) {
    int is_alive = b->board[row * b->columns + column];
    int count = -is_alive;
    for (int r = -1; r < 2; r++) {
        int check_row = (row + r + b->rows) % b->rows;
        for (int c = -1; c < 2; c++) {
            int check_column = (column + c + b->columns) % b->columns;
            count += b->board[check_row * b->columns + check_column];
        }
    }
    if (count == 3) {
        return true;
    } else if (is_alive && count == 2) {
        return true;
    }
    return false;
}

bool board_draw(struct Board *b) {
    for (int row = 0; row < b->rows; row++) {
        b->rect.y = SIZE * row;
        for (int column = 0; column < b->columns; column++) {
            b->rect.x = SIZE * column;
            if (b->board[row * b->columns + column]) {
                if (SDL_RenderCopy(b->renderer, b->image, NULL, &b->rect)) {
                    fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
                    return true;
                }
            }
        }
    }
    return false;
}