#include "player.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define START_HP 100
#define START_STAMINA 50
#define START_GOLD 100
#define FIRST_HOUSE_UPGRADE_COST 100

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
