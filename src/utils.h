#ifndef UTILS_H
#define UTILS_H

#include "game.h"

int read_int(const char *prompt, int *value);
void read_text(const char *prompt, char *text, int size);
void load_item_names(GameState *game);
void show_time(const GameState *game);
void time_menu(GameState *game);

#endif
