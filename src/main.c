#include "game.h"
#include "map.h"
#include "player.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void show_menu(const GameState *game) {
    const char *name = game->player == NULL
                       ? "Персонаж не создан" : game->player->name;

    printf("\n============================ МЕНЮ ============================\n");
    printf(" Фермер: %s | День: %d | Время: %02d:00\n",
           name, game->day, game->hour);
    printf("---------------------------------------------------------------\n");
    printf("[1] Создать / пересоздать персонажа\n");
    printf("[2] Карточка персонажа\n");
    printf("[3] Режим дня\n");
    printf("[4] Посмотреть инвентарь\n");
    printf("[5] Положить предмет в слот\n");
    printf("[6] Выбросить предмет\n");
    printf("[7] Поиск предметов по ID\n");
    printf("[8] Поиск предмета по названию\n");
    printf("[9] Записать состояние в дневник\n");
    printf("[10] Отправиться в приключение\n");
    printf("[11] Проверить рецепты крафта\n");
    printf("[12] Улучшить жильё\n");
    printf("[0] Завершение программы\n");
    printf("===============================================================\n");
}

int main(void) {
    GameState game = {0};
    int menu_choice;

    srand((unsigned int)time(NULL));
    game.day = 1;
    game.hour = 8;
    load_item_names(&game);

    while (1) {
        show_menu(&game);
        if (!read_int("Выберите пункт меню: ", &menu_choice)) {
            continue;
        }

        switch (menu_choice) {
            case 1:
                create_player(&game);
                break;
            case 2:
                show_player_card(&game);
                break;
            case 3:
                time_menu(&game);
                break;
            case 4:
                print_inventory(&game);
                break;
            case 5:
                add_item(&game);
                break;
            case 6:
                drop_item(&game);
                break;
            case 7:
                search_heavy_items(&game);
                break;
            case 8:
                search_item_by_name(&game);
                break;
            case 9:
                write_diary(&game);
                break;
            case 10:
                adventure(&game);
                break;
            case 11:
                check_crafting_recipes(&game);
                break;
            case 12:
                upgrade_house(&game);
                break;
            case 0:
                free_map(&game);
                free_player(&game);
                printf("Игра завершена.\n");
                return 0;
            default:
                printf("Неверный ввод. Попробуйте снова.\n");
        }
    }
}
