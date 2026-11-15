#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

void create_player(GameState *game);
void free_player(GameState *game);
void show_player_card(const GameState *game);

#endif
