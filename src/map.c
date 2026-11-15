#include "map.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_MIN_SIZE 5
#define MAP_MAX_SIZE 30
#define CELL_WALL '#'
#define CELL_FLOOR '.'
#define CELL_RUNE '^'
#define CELL_STAIRS_UP 'U'
#define CELL_STAIRS_DOWN 'D'
#define CELL_HERO '@'
#define OBSTACLE_MIN_PERCENT 15
#define OBSTACLE_MAX_PERCENT 30
#define RUNE_PERCENT 5
#define PERCENT_TOTAL 100
#define INPUT_LINE_LEN 32
#define NIGHT_HOUR 2
#define GOLD_MULTIPLIER 10

static int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

static void release_map(Map *map, int show_message) {
    if (map == NULL) {
        return;
    }

    if (map->cells != NULL) {
        for (int y = 0; y < map->height; y++) {
            free(map->cells[y]);
        }
        free(map->cells);
    }

    if (show_message) {
        printf("Память структуры по адресу %p освобождена\n", (void *)map);
    }
    free(map);
}

void free_map(GameState *game) {
    if (game->map == NULL) {
        return;
    }

    release_map(game->map, 1);
    game->map = NULL;
}

static int allocate_cells(Map *map) {
    map->cells = calloc((size_t)map->height, sizeof(char *));
    if (map->cells == NULL) {
        return 0;
    }

    for (int y = 0; y < map->height; y++) {
        map->cells[y] = malloc((size_t)map->width * sizeof(char));
        if (map->cells[y] == NULL) {
            return 0;
        }
    }

    return 1;
}

static void generate_cells(Map *map) {
    int obstacle_chance = random_range(OBSTACLE_MIN_PERCENT, OBSTACLE_MAX_PERCENT);

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (y == 0 || y == map->height - 1 ||
                x == 0 || x == map->width - 1) {
                map->cells[y][x] = CELL_WALL;
                continue;
            }

            int roll = rand() % PERCENT_TOTAL;
            if (roll < obstacle_chance) {
                map->cells[y][x] = CELL_WALL;
            } else if (roll < obstacle_chance + RUNE_PERCENT) {
                map->cells[y][x] = CELL_RUNE;
            } else {
                map->cells[y][x] = CELL_FLOOR;
            }
        }
    }
}

static int pick_free_cell(Map *map, int *out_x, int *out_y) {
    int free_count = 0;

    for (int y = 1; y < map->height - 1; y++) {
        for (int x = 1; x < map->width - 1; x++) {
            if (map->cells[y][x] == CELL_FLOOR) {
                free_count++;
            }
        }
    }

    if (free_count == 0) {
        return 0;
    }

    int target = rand() % free_count;
    for (int y = 1; y < map->height - 1; y++) {
        for (int x = 1; x < map->width - 1; x++) {
            if (map->cells[y][x] == CELL_FLOOR) {
                if (target == 0) {
                    *out_x = x;
                    *out_y = y;
                    return 1;
                }
                target--;
            }
        }
    }

    return 0;
}

static Map *create_floor(int floor_number, int *start_x, int *start_y) {
    Map *map = calloc(1, sizeof(Map));
    if (map == NULL) {
        return NULL;
    }

    map->height = random_range(MAP_MIN_SIZE, MAP_MAX_SIZE);
    map->width = random_range(MAP_MIN_SIZE, MAP_MAX_SIZE);
    map->floor = floor_number;

    if (!allocate_cells(map)) {
        release_map(map, 1);
        return NULL;
    }

    generate_cells(map);

    if (!pick_free_cell(map, start_x, start_y)) {
        *start_x = 1;
        *start_y = 1;
        map->cells[*start_y][*start_x] = CELL_FLOOR;
    }
    map->cells[*start_y][*start_x] = CELL_STAIRS_DOWN;

    int up_x, up_y;
    if (!pick_free_cell(map, &up_x, &up_y)) {
        up_x = map->width - 2;
        up_y = map->height - 2;
        map->cells[up_y][up_x] = CELL_FLOOR;
    }
    map->cells[up_y][up_x] = CELL_STAIRS_UP;

    return map;
}

static int is_passable(char cell) {
    return cell == CELL_FLOOR || cell == CELL_RUNE ||
           cell == CELL_STAIRS_UP || cell == CELL_STAIRS_DOWN;
}

static void teleport_player(GameState *game) {
    int x, y;

    game->map->cells[game->player->y][game->player->x] = CELL_FLOOR;
    if (pick_free_cell(game->map, &x, &y)) {
        game->player->x = x;
        game->player->y = y;
        printf("Руна вспыхнула и исчезла! Вас перенесло на клетку (%d, %d).\n", x, y);
    } else {
        printf("Руна погасла, но свободных клеток для телепорта нет.\n");
    }
}

static Direction read_direction(void) {
    char line[INPUT_LINE_LEN];

    printf("Куда идём? (W - вверх, A - влево, S - вниз, D - вправо, Q - домой): ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
        return DIR_QUIT;
    }

    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int symbol;
        while ((symbol = getchar()) != '\n' && symbol != EOF) {
        }
        printf("Неверный ввод! Введите одну букву: W, A, S, D или Q.\n");
        return DIR_NONE;
    }
    line[strcspn(line, "\r\n")] = '\0';
    if (strlen(line) != 1) {
        printf("Неверный ввод! Введите одну букву: W, A, S, D или Q.\n");
        return DIR_NONE;
    }

    switch (toupper((unsigned char)line[0])) {
        case 'W': return DIR_UP;
        case 'S': return DIR_DOWN;
        case 'A': return DIR_LEFT;
        case 'D': return DIR_RIGHT;
        case 'Q': return DIR_QUIT;
        default:
            printf("Неверный ввод! Введите W, A, S, D или Q.\n");
            return DIR_NONE;
    }
}

static int move_player(GameState *game, Direction direction) {
    int new_x = game->player->x;
    int new_y = game->player->y;

    switch (direction) {
        case DIR_UP: new_y--; break;
        case DIR_DOWN: new_y++; break;
        case DIR_LEFT: new_x--; break;
        case DIR_RIGHT: new_x++; break;
        default: return 0;
    }

    if (new_x < 0 || new_x >= game->map->width ||
        new_y < 0 || new_y >= game->map->height) {
        printf("Дальше дороги нет — край карты!\n");
        return 0;
    }

    char destination = game->map->cells[new_y][new_x];
    if (!is_passable(destination)) {
        printf("Туда нельзя: там стена!\n");
        return 0;
    }

    game->player->x = new_x;
    game->player->y = new_y;

    if (destination == CELL_RUNE) {
        teleport_player(game);
        return 1;
    }

    return destination == CELL_STAIRS_UP ? 2 : 1;
}

static void draw_map(const GameState *game) {
    const Map *map = game->map;
    const Player *player = game->player;

    printf("\nHP: %d/%d | Выносливость: %d/%d | Золото: %d | Этаж: %d | Время: %02d:00\n",
           player->hp, player->max_hp, player->stamina, player->max_stamina,
           player->gold, map->floor, game->hour);

    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (x == player->x && y == player->y) {
                printf("%c ", CELL_HERO);
            } else {
                printf("%c ", map->cells[y][x]);
            }
        }
        printf("\n");
    }

    printf("%c - вы, %c - стена, %c - руна, %c - лестница вверх, %c - лестница вниз\n",
           CELL_HERO, CELL_WALL, CELL_RUNE, CELL_STAIRS_UP, CELL_STAIRS_DOWN);
}

static void emergency_return(GameState *game) {
    free_map(game);
    game->player->gold /= 2;
    game->player->x = 0;
    game->player->y = 0;
    printf("Вы обессилели и потеряли сознание. Спасатели доставили вас домой.\n");
    printf("За помощь забрали половину золота. Осталось: %d.\n", game->player->gold);
}

void adventure(GameState *game) {
    int action;

    if (game->player == NULL) {
        printf("Сначала создайте персонажа.\n");
        return;
    }
    if (game->player->stamina <= 0) {
        printf("Нельзя отправиться в поход без выносливости.\n");
        return;
    }
    if (game->hour >= NIGHT_HOUR) {
        printf("Уже 02:00 или позже. Сначала нужно поспать.\n");
        return;
    }

    game->map = create_floor(1, &game->player->x, &game->player->y);
    if (game->map == NULL) {
        printf("Ошибка: не удалось выделить память под карту.\n");
        return;
    }

    printf("Вы отправились исследовать башню.\n");

    while (game->map != NULL) {
        draw_map(game);
        Direction direction = read_direction();

        if (direction == DIR_QUIT) {
            if (game->map->cells[game->player->y][game->player->x] == CELL_STAIRS_DOWN) {
                free_map(game);
                game->player->x = 0;
                game->player->y = 0;
                printf("Вы вернулись домой. Всё золото осталось при вас.\n");
                return;
            }
            printf("Вернуться домой можно только с лестницы вниз.\n");
            continue;
        }

        int move_result = move_player(game, direction);
        if (move_result == 0) {
            continue;
        }

        game->player->stamina--;
        if (game->player->stamina <= 0) {
            emergency_return(game);
            return;
        }

        if (move_result == 2) {
            game->player->stamina -= 5;
            game->hour++;
            if (game->hour >= 24) {
                game->hour -= 24;
                game->day++;
            }

            int next_floor = game->map->floor + 1;
            game->player->gold += GOLD_MULTIPLIER * next_floor;

            if (game->player->stamina <= 0 || game->hour >= NIGHT_HOUR) {
                emergency_return(game);
                return;
            }

            free_map(game);
            game->map = create_floor(next_floor, &game->player->x, &game->player->y);
            if (game->map == NULL) {
                game->player->x = 0;
                game->player->y = 0;
                printf("Не удалось создать следующий этаж. Поход завершён.\n");
                return;
            }

            printf("Вы перешли на этаж %d и получили %d золота.\n",
                   next_floor, GOLD_MULTIPLIER * next_floor);
        }
    }
}
