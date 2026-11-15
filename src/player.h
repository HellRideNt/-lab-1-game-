#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

void create_player(GameState *game);
void free_player(GameState *game);
void show_player_card(const GameState *game);
void rest_player(GameState *game);
void print_inventory(const GameState *game);
void add_item(GameState *game);
void drop_item(GameState *game);
void search_heavy_items(GameState *game);
void search_item_by_name(GameState *game);
void write_diary(GameState *game);
void check_crafting_recipes(GameState *game);
void upgrade_house(GameState *game);

#endif
