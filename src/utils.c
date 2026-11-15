#include "utils.h"
#include "player.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NIGHT_HOUR 2

static int read_int(const char *prompt, int *value) {
    char line[64];
    char *end;
    long number;

    printf("%s", prompt);

    if (fgets(line, sizeof(line), stdin) == NULL) {
        return 0;
    }

    if (strchr(line, '\n') == NULL && !feof(stdin)) {
        int symbol;
        while ((symbol = getchar()) != '\n' && symbol != EOF) {
        }
        printf("Ошибка ввода. Слишком длинная строка.\n");
        return 0;
    }

    errno = 0;
    number = strtol(line, &end, 10);

    if (end == line || errno == ERANGE ||
        number < INT_MIN || number > INT_MAX) {
        printf("Ошибка ввода. Введите целое число.\n");
        return 0;
    }

    while (*end == ' ' || *end == '\t' ||
           *end == '\r' || *end == '\n') {
        end++;
    }

    if (*end != '\0') {
        printf("Ошибка ввода. Введите только целое число.\n");
        return 0;
    }

    *value = (int)number;
    return 1;
}

void show_time(const GameState *game) {
    printf("День %d - %02d:00\n", game->day, game->hour);
}

void time_menu(GameState *game) {
    int choice;

    while (1) {
        printf("\n--- РЕЖИМ ДНЯ ---\n");
        printf("[1] Посмотреть на часы\n");
        printf("[2] Пропустить время\n");
        printf("[3] Лечь спать\n");
        printf("[0] Назад\n");

        if (!read_int("Выберите пункт: ", &choice)) {
            continue;
        }

        if (choice == 0) {
            return;
        }

        if (choice == 1) {
            show_time(game);
        } else if (choice == 2) {
            int hours;
            int until_night = (NIGHT_HOUR - game->hour + 24) % 24;

            if (until_night == 0) {
                printf("Уже 02:00. Чтобы продолжить, нужно лечь спать.\n");
                continue;
            }

            if (!read_int("На сколько часов пропустить время: ", &hours)) {
                continue;
            }

            if (hours < 1 || hours > until_night) {
                printf("Можно пропустить от 1 до %d ч., не переходя за 02:00.\n",
                       until_night);
                continue;
            }

            game->hour += hours;

            if (game->hour >= 24) {
                game->hour -= 24;
                game->day++;
            }

            show_time(game);
        } else if (choice == 3) {
            rest_player(game);
        } else {
            printf("Такого пункта нет.\n");
        }
    }
}
