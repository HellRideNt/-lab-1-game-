#include "player.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_HP 100
#define START_STAMINA 50
#define START_GOLD 100
#define FIRST_HOUSE_UPGRADE_COST 100
#define CABIN_UPGRADE_COST 500
#define HOME_UPGRADE_COST 1500

static const char *house_name(HouseLevel level) {
    switch (level) {
        case HOUSE_TENT: return "Палатка";
        case HOUSE_CABIN: return "Хижина";
        case HOUSE_HOME: return "Дом";
        case HOUSE_MANSION: return "Особняк";
        default: return "Неизвестное жильё";
    }
}

static void clear_input_line(void) {
    int symbol;

    while ((symbol = getchar()) != '\n' && symbol != EOF) {
    }
}

void free_player(GameState *game) {
    if (game->player == NULL) {
        return;
    }

    printf("Память структуры по адресу %p освобождена\n",
           (void *)game->player);

    free(game->player);
    game->player = NULL;
}

void create_player(GameState *game) {
    Player *player;
    int answer;
    char name[PLAYER_NAME_SIZE];

    if (game->player != NULL) {
        printf("Персонаж уже создан. Создать нового? (1 - да, 0 - нет): ");

        if (scanf("%d", &answer) != 1) {
            clear_input_line();
            printf("Ошибка ввода.\n");
            return;
        }
        clear_input_line();

        if (answer != 1) {
            printf("Персонаж оставлен без изменений.\n");
            return;
        }

        free_player(game);
    }

    player = malloc(sizeof(Player));
    if (player == NULL) {
        printf("Ошибка: не удалось выделить память под персонажа.\n");
        return;
    }

    printf("Введите имя фермера: ");
    if (fgets(name, sizeof(name), stdin) == NULL) {
        strcpy(name, "Безымянный");
    } else {
        name[strcspn(name, "\r\n")] = '\0';
    }

    if (name[0] == '\0') {
        strcpy(name, "Безымянный");
    }

    strcpy(player->name, name);

    player->hp = START_HP;
    player->max_hp = START_HP;
    player->stamina = START_STAMINA;
    player->max_stamina = START_STAMINA;
    player->gold = START_GOLD;
    player->x = 0;
    player->y = 0;

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        player->inventory[i] = i + 1;
    }
    player->inventory[INVENTORY_SIZE - 1] = 0;

    player->farmhouse.level = HOUSE_TENT;
    player->farmhouse.upgrade_cost = FIRST_HOUSE_UPGRADE_COST;
    player->farmhouse.comfort = 1;

    game->player = player;

    printf("Персонаж %s создан.\n", player->name);
}

void show_player_card(const GameState *game) {
    if (game->player == NULL) {
        printf("Сначала создайте персонажа.\n");
        return;
    }

    printf("\n--- КАРТОЧКА ПЕРСОНАЖА ---\n");
    printf("Фермер: %s\n", game->player->name);
    printf("Здоровье: %d/%d\n",
           game->player->hp, game->player->max_hp);
    printf("Выносливость: %d/%d\n",
           game->player->stamina, game->player->max_stamina);
    printf("Золото: %d\n", game->player->gold);
    printf("Жильё: %s\n", house_name(game->player->farmhouse.level));
    printf("День %d - %02d:00\n", game->day, game->hour);

    printf("Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = game->player->inventory[i];

        if (id >= 0 && id < ITEMS_COUNT) {
            printf("Слот %2d: [ID: %d] %s\n",
                   i, id, game->item_names[id]);
        } else {
            printf("Слот %2d: [ID: %d] Неизвестный предмет\n", i, id);
        }
    }
}

void rest_player(GameState *game) {
    Player *player = game->player;
    int bonus = 0;

    game->day++;
    game->hour = MORNING_HOUR;

    if (player == NULL) {
        printf("Новый день: %d, %02d:00.\n", game->day, game->hour);
        return;
    }

    player->hp = player->max_hp;

    if (player->farmhouse.level == HOUSE_TENT) {
        player->stamina = player->max_stamina * 70 / 100;
    } else if (player->farmhouse.level == HOUSE_CABIN) {
        player->stamina = player->max_stamina;
    } else if (player->farmhouse.level == HOUSE_HOME) {
        bonus = 15;
        player->stamina = player->max_stamina + bonus;
    } else {
        bonus = 30;
        player->stamina = player->max_stamina + bonus;
    }

    printf("Здоровье восстановлено.\n");
    printf("Выносливость: %d/%d", player->stamina, player->max_stamina);

    if (bonus > 0) {
        printf(" (+%d бодрости)", bonus);
    }

    printf(".\nНовый день: %d, %02d:00.\n", game->day, game->hour);
}

static int require_player(const GameState *game) {
    if (game->player == NULL) {
        printf("Сначала создайте персонажа в пункте [1].\n");
        return 0;
    }
    return 1;
}

void print_inventory(const GameState *game) {
    if (!require_player(game)) return;

    printf("================ ИНВЕНТАРЬ ================\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = game->player->inventory[i];
        if (id >= 0 && id < ITEMS_COUNT) {
            printf("Слот %2d: [ID: %d] %s\n", i, id, game->item_names[id]);
        } else {
            printf("Слот %2d: [ID: %d] Неизвестный предмет\n", i, id);
        }
    }
    printf("============================================\n");
}

void add_item(GameState *game) {
    int slot, id;
    if (!require_player(game)) return;

    if (!read_int("Введите индекс слота (от 0 до 9): ", &slot) ||
        slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Ошибка: такого слота нет.\n");
        return;
    }
    if (!read_int("Введите ID предмета (от 0 до 9): ", &id) ||
        id < 0 || id >= ITEMS_COUNT) {
        printf("Ошибка: ID должен быть от 0 до 9.\n");
        return;
    }

    game->player->inventory[slot] = id;
    printf("Предмет успешно добавлен в слот %d.\n", slot);
}

void drop_item(GameState *game) {
    int slot;
    if (!require_player(game)) return;

    if (!read_int("Введите индекс слота, который хотите выбросить: ", &slot) ||
        slot < 0 || slot >= INVENTORY_SIZE) {
        printf("Ошибка: такого слота нет.\n");
        return;
    }
    if (game->player->inventory[slot] == 0) {
        printf("Слот %d уже пуст.\n", slot);
        return;
    }

    printf("Предмет с ID %d выброшен из слота %d.\n",
           game->player->inventory[slot], slot);
    game->player->inventory[slot] = 0;
}

void search_heavy_items(GameState *game) {
    int threshold, found = 0;
    if (!require_player(game)) return;
    if (!read_int("Введите пороговый ID предмета: ", &threshold)) return;

    printf("Слоты с предметами, чей ID строго больше %d:\n", threshold);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = game->player->inventory[i];
        if (id > threshold) {
            const char *name = (id >= 0 && id < ITEMS_COUNT)
                               ? game->item_names[id] : "Неизвестный предмет";
            printf("- Слот %d: ID %d (%s)\n", i, id, name);
            found++;
        }
    }
    if (found == 0) printf("Подходящих предметов не найдено.\n");
}

void search_item_by_name(GameState *game) {
    char target[ITEM_NAME_SIZE];
    int target_id = -1, found = 0;
    if (!require_player(game)) return;

    read_text("Введите точное название предмета: ", target, sizeof(target));
    for (int i = 0; i < ITEMS_COUNT; i++) {
        if (strcmp(game->item_names[i], target) == 0) {
            target_id = i;
            break;
        }
    }
    if (target_id < 0) {
        printf("Предмет с названием \"%s\" не найден в каталоге.\n", target);
        return;
    }

    printf("Результаты поиска для \"%s\" (ID: %d):\n", target, target_id);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (game->player->inventory[i] == target_id) {
            printf("- Найден в слоте %d\n", i);
            found++;
        }
    }
    if (found == 0) printf("В рюкзаке сейчас нет этого предмета.\n");
}

void write_diary(GameState *game) {
    FILE *diary;
    if (!require_player(game)) return;

    diary = fopen("diary.txt", "a");
    if (diary == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи.\n");
        return;
    }

    fprintf(diary, "========================================\n");
    fprintf(diary, "Фермер: %s\n", game->player->name);
    fprintf(diary, "Время: День %d, %02d:00\n", game->day, game->hour);
    fprintf(diary, "Инвентарь:\n");
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = game->player->inventory[i];
        const char *name = (id >= 0 && id < ITEMS_COUNT)
                           ? game->item_names[id] : "Неизвестный предмет";
        fprintf(diary, "  Слот %2d: %s (ID %d)\n", i, name, id);
    }
    fprintf(diary, "========================================\n\n");
    fclose(diary);
    printf("[Успешно] Данные записаны в diary.txt.\n");
}

void check_crafting_recipes(GameState *game) {
    FILE *in, *out;
    char line[256];
    int inventory_counts[ITEMS_COUNT] = {0};
    if (!require_player(game)) return;

    in = fopen("input.txt", "r");
    if (in == NULL) {
        printf("Ошибка: входной файл input.txt не найден.\n");
        return;
    }
    out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Ошибка: не удалось открыть output.txt.\n");
        fclose(in);
        return;
    }

    for (int i = 0; i < INVENTORY_SIZE; i++) {
        int id = game->player->inventory[i];
        if (id > 0 && id < ITEMS_COUNT) inventory_counts[id]++;
    }

    while (fgets(line, sizeof(line), in) != NULL) {
        char *colon;
        int needed[ITEMS_COUNT] = {0};
        int offset = 0, id, bytes_read, can_craft = 1;

        line[strcspn(line, "\r\n")] = '\0';
        colon = strchr(line, ':');
        if (colon == NULL) continue;
        *colon = '\0';
        char *items = colon + 1;

        while (sscanf(items + offset, "%d%n", &id, &bytes_read) == 1) {
            if (id > 0 && id < ITEMS_COUNT) needed[id]++;
            offset += bytes_read;
        }

        for (int i = 1; i < ITEMS_COUNT; i++) {
            if (needed[i] > inventory_counts[i]) can_craft = 0;
        }

        if (can_craft) {
            printf("%s: [ДОСТУПНО]\n", line);
            fprintf(out, "%s: [ДОСТУПНО]\n", line);
        } else {
            int first = 1;
            printf("%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Недостает: ", line);
            fprintf(out, "%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Недостает: ", line);
            for (int i = 1; i < ITEMS_COUNT; i++) {
                if (needed[i] > inventory_counts[i]) {
                    int missing = needed[i] - inventory_counts[i];
                    if (!first) {
                        printf(", ");
                        fprintf(out, ", ");
                    }
                    printf("%s x%d", game->item_names[i], missing);
                    fprintf(out, "%s x%d", game->item_names[i], missing);
                    first = 0;
                }
            }
            printf(")\n");
            fprintf(out, ")\n");
        }
    }

    fclose(in);
    fclose(out);
    printf("Результаты крафта сохранены в output.txt.\n");
}

void upgrade_house(GameState *game) {
    Player *player;
    int answer;
    if (!require_player(game)) return;

    player = game->player;
    if (player->farmhouse.level >= HOUSE_MANSION) {
        printf("У вас уже особняк, улучшать больше некуда.\n");
        return;
    }

    printf("Следующее жильё: %s. Стоимость: %d золота. У вас: %d.\n",
           house_name((HouseLevel)(player->farmhouse.level + 1)),
           player->farmhouse.upgrade_cost, player->gold);
    if (player->gold < player->farmhouse.upgrade_cost) {
        printf("Не хватает золота.\n");
        return;
    }
    if (!read_int("Улучшить жильё? (1 - да, 0 - нет): ", &answer) || answer != 1) {
        printf("Улучшение отменено.\n");
        return;
    }

    player->gold -= player->farmhouse.upgrade_cost;
    player->farmhouse.level++;
    player->farmhouse.comfort++;
    if (player->farmhouse.level == HOUSE_CABIN) {
        player->farmhouse.upgrade_cost = CABIN_UPGRADE_COST;
    } else if (player->farmhouse.level == HOUSE_HOME) {
        player->farmhouse.upgrade_cost = HOME_UPGRADE_COST;
    } else {
        player->farmhouse.upgrade_cost = 0;
    }

    printf("Жильё улучшено: теперь у вас %s.\n",
           house_name(player->farmhouse.level));
}
