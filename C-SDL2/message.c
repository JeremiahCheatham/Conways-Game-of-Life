#include "message.h"

bool message_new(struct Message **message, SDL_Renderer *renderer, double speed) {

    *message = calloc(1, sizeof(struct Message));
    if (!*message) {
        fprintf(stderr, "Error in calloc of message!\n");
        return true;
    }
    struct Message *m = *message;

    m->renderer = renderer;
    m->color = (SDL_Color){255, 255, 255, 255};

    m->font = TTF_OpenFont("fonts/freesansbold.ttf", FONT_SIZE);
    if (!m->font) {
        fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
        return true;
    }

    m->rect.x = 10;
    m->rect.y = 10;

    if (message_update_speed(m, speed)) {
        return true;
    }

    return false;
}

void message_free(struct Message **message) {
    if (*message) {
        TTF_CloseFont((*message)->font);
        (*message)->font = NULL;
        SDL_FreeSurface((*message)->surface);
        (*message)->surface = NULL;
        SDL_DestroyTexture((*message)->image);
        (*message)->image = NULL;
        (*message)->renderer = NULL;
        free(*message);
        *message = NULL;
    }
}

bool message_update_speed(struct Message *m, double speed) {
    int length = snprintf(NULL, 0, "Speed: %g", speed) + 1;
    char text[length];
    snprintf(text, (size_t)length, "Speed: %g", speed);
    if (message_update(m, text)) {
        return true;
    }
    return false;
}

bool message_update(struct Message *m, const char *text) {
    if (m->image) {
        SDL_DestroyTexture(m->image);
    }
    
    m->surface = TTF_RenderText_Blended(m->font, text, m->color);
    if (!m->surface) {
        fprintf(stderr, "Error creating a surface: %s\n", SDL_GetError());
        return true;
    }

    m->image = SDL_CreateTextureFromSurface(m->renderer, m->surface);
    if (!m->image) {
        fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
        return true;
    }

    SDL_FreeSurface(m->surface);
    m->surface = NULL;

    if (SDL_QueryTexture(m->image, NULL, NULL, &m->rect.w, &m->rect.h)) {
        fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
        return true;
    }
    
    return false;
}

bool message_draw(struct Message *m) {
    if (SDL_RenderCopy(m->renderer, m->image, NULL, &m->rect)) {
        fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
        return true;
    }

    return false;
}