#ifndef GAME_H
#define GAME_H

#define PLAYER_NAME_SIZE 32
#define ITEM_NAME_SIZE 32
#define ITEMS_COUNT 10
#define INVENTORY_SIZE 10
#define NIGHT_HOUR 2
#define MORNING_HOUR 8

typedef enum {
    HOUSE_TENT = 0,
    HOUSE_CABIN,
    HOUSE_HOME,
    HOUSE_MANSION
} HouseLevel;

typedef enum {
    DIR_NONE = 0,
    DIR_UP,
    DIR_DOWN,
    DIR_LEFT,
    DIR_RIGHT,
    DIR_QUIT
} Direction;

typedef struct {
    HouseLevel level;
    int upgrade_cost;
    int comfort;
} Farmhouse;

typedef struct Player {
    char name[PLAYER_NAME_SIZE];

    int hp;
    int max_hp;
    int stamina;
    int max_stamina;
    int gold;

    int x;
    int y;

    int inventory[INVENTORY_SIZE];
    Farmhouse farmhouse;
} Player;

typedef struct Map {
    char **cells;
    int width;
    int height;
    int floor;
} Map;

typedef struct {
    Player *player;
    Map *map;

    int day;
    int hour;

    char item_names[ITEMS_COUNT][ITEM_NAME_SIZE];
} GameState;

#endif
