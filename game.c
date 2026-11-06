#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#ifdef _WIN32
    #include <direct.h>
    #define get_current_dir _getcwd
#else
    #include <unistd.h>
    #define get_current_dir getcwd
#endif
#define inventory_size 10 
#define MAX_NAME_LEN 32
#define MAX_ITEMS 10
#define MAP_MIN_SIZE 5
#define MAP_MAX_SIZE 30
#define CELL_WALL  '#'
#define CELL_FLOOR '.'
#define CELL_TREE  'T'
#define CELL_ROCK  'R'
#define CELL_RUNE  '^'
#define CELL_HERO  '@'
#define OBSTACLE_MIN_PERCENT 15
#define OBSTACLE_MAX_PERCENT 30
#define PERCENT_TOTAL 100
#define RUNE_PERCENT 5
#define INPUT_LINE_LEN 32
#define KEY_UP    'W'
#define KEY_LEFT  'A'
#define KEY_DOWN  'S'
#define KEY_RIGHT 'D'
#define KEY_QUIT  'Q'


// Функция которая чистит буфер от всякой каки
void clear_input_buffer() {
    while (getchar() != '\n');
}

//=============================================================================================
char farmer_name[MAX_NAME_LEN] = "Безымянный";
char item_names[MAX_ITEMS][MAX_NAME_LEN];
int menu_choice;
int hours_to_add=0;
int current_day = 1;
int current_hour = 8;
int item_id;
int index_slot;
int map_blocks_allocated = 0;


// Функция для информации

void info() {
    printf("============================ МЕНЮ ================================\n");
    printf(" Фермер: %s | День: %d | Время: %02d:00\n", farmer_name, current_day, current_hour);
    printf("----------------------------------------------------------------\n");
    printf("[1] Вывести текущий день и час\n");
    printf("[2] Промотать время (поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет\n");
    printf("[6] Поиск тяжестей (ID выше порога)\n");
    printf("[7] Поиск предмета в рюкзаке по названию\n");
    printf("[8] Записать состояние в дневник фермера\n");
    printf("[9] Проверить рецепты крафта \n");
    printf("[10] Отправиться в приключение\n");
    printf("[0] Завершение программы\n");
    printf("================================================================\n");
    printf("Выберите пункт меню: ");
}
//Заполнение каталога по умолчанию======================================================================

void load_default_items() {
const char *defaults[MAX_ITEMS] = {
    "Пусто",
    "Дерево",
    "Камень",
    "Семена_пшеницы",
    "Меч",
    "Каменная_кирка",
    "Яблоко",
    "Факел",
    "Туалетная_бумага",
    "Вода"
};

for (int i = 0; i < MAX_ITEMS; i++) {
    strncpy(item_names[i], defaults[i], MAX_NAME_LEN - 1);
    item_names[i][MAX_NAME_LEN - 1] = '\0';
}
}

//Загрузка каталога из items.txt=======================================================================
void load_items_from_file() {
    FILE *file = fopen("items.txt", "r");
    if (file == NULL) {
        printf("[Внимание] Файл items.txt не найден! Загружены названия по умолчанию.\n\n");
        load_default_items();
        return;
    }
    int id;
    char name[MAX_NAME_LEN];
    int loaded_count = 0;

    // Считываем пары: ID и Название
    while (fscanf(file, "%d %31s", &id, name) == 2) {
        if (id >= 0 && id < MAX_ITEMS) {
            strncpy(item_names[id], name, MAX_NAME_LEN - 1);
            item_names[id][MAX_NAME_LEN - 1] = '\0';
            loaded_count++;
        }
    }
    fclose(file);
    if (loaded_count < MAX_ITEMS) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            if (strlen(item_names[i]) == 0) {
                snprintf(item_names[i], MAX_NAME_LEN, "Предмет_%d", i);
            }
        }
    }
    printf("[Успешно] Каталог предметов загружен из items.txt!\n\n");
}

//Это значит у нас функция которая выводит слоты[3]==============================================
void print_inventory(const int inventory[],int size) {
    printf("================================================================\n");
    printf("=                          ИНВЕНТАРЬ                           =\n");                      
    printf("================================================================\n");

    for (int i=0; i < size; i++) {
    item_id = inventory[i];
    if (item_id == 0) {
        printf("Слот %2d: [-] Пусто\n",i);
    } else if (item_id >=1 && item_id <=9) {
        printf("Слот %2d: [ID: %d] %s\n", i, item_id, item_names[item_id]);
    } else {
        printf("Cлот %2d: [ID:%d] Неизвеcтный предмет\n", i, item_id);
    }
}
printf("================================================================\n\n\n");
}
//Функция которая выводит время[1]===============================================================
void print_time(){
        printf("Результат:\n");
    printf("День %d - %d:00 \n\n\n",current_day,current_hour);
}

//Функция промотки врмени[2]====================================================================
void to_waste_time(){
    printf("На сколько вы бы хотели промотать время: \n");
    if (scanf("%d", &hours_to_add) != 1 || hours_to_add < 0) {
        printf("Ошибка: введите положительное число часов!\n");
        while (getchar() != '\n');
    }
    current_hour += hours_to_add;
    current_day += current_hour / 24;
    current_hour = current_hour % 24;
    printf("День %d - %02d:00\n\n\n", current_day, current_hour);
}
//Функция добавления предмета[4]========================================================================
void add_item(int inventory[], int size) {
    int index_slot;
    int item_id;

    printf("Введите индекс слота (от 0 до %d): \n", size - 1);
    if (scanf("%d", &index_slot) != 1) {
        printf("Ошибка ввода! Введите число.\n\n\n");
        clear_input_buffer();
        return;
    }

    if (index_slot < 0 || index_slot >= size) {
        printf("Ошибка: слот с таким индексом не существует!\nДопустимо от 0 до %d\n\n\n", size - 1);
        return;
    }

    printf("Введите ID предмета: \n");
    if (scanf("%d", &item_id) != 1) {
        printf("Ошибка ввода ID! Введите число.\n\n\n");
        clear_input_buffer();
        return;
    }

    inventory[index_slot] = item_id;
    printf("Предмет успешно добавлен в слот %d\n\n\n", index_slot);
}


//Функция удаления(выбрасывания)предмета[5]======================================================================
void drop_item(int inventory[], int size){
    printf("Введите индекс слота предмета, который хотели бы выбросить (от 0 до %d): \n", inventory_size - 1);
    if (scanf("%d", &index_slot) != 1) {
        printf("Ошибка ввода! Введите число.\n");
        return;
    }
    if (index_slot < 0 || index_slot >= inventory_size) {
        printf("Ошибка: слот с индексом %d не существует!\n", index_slot);
        return;
    }
    if (inventory[index_slot] == 0) {
        printf("Слот %d уже пуст.\n", index_slot);
        return;
    }
    printf("Предмет с ID %d выброшен из слота %d.\n\n\n", inventory[index_slot], index_slot);
    inventory[index_slot]=0;   
}
//Функия поиска тяжестей[6]===============================================================================
void search_heavy_items(const int inv[], int size) {
    int input_id;
    int found_count = 0;
    printf("Введите пороговый ID предмета: \n");
    if (scanf("%d", &input_id) != 1) {
        printf("Ошибка ввода! Введите число.\n\n\n");
        clear_input_buffer();
        return;
    }
    printf("Слоты с предметами, чей ID строго больше %d:\n", input_id);
    for (int i = 0; i < size; i++) {
        if (inv[i] > input_id) {
            printf("- Слот %d: ID %d (%s)\n", i, inv[i],
                   (inv[i] <= 9) ? item_names[inv[i]] : "Неизвестный предмет");
            found_count++;
        }
    }
    if (found_count == 0) {
        printf("Подходящих предметов не найдено.\n");
    }
    printf("\n\n");
}

//Поиск предмета по названию[7]================================================================
void search_item_by_name(const int inventory[], int size) {
    char target_name[MAX_NAME_LEN];
    printf("\nВведите точное название предмета для поиска: ");
    if (scanf("%31s", target_name) != 1) {
        printf("Ошибка чтения названия.\n\n");
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    int target_id = -1;
    for (int i = 0; i < MAX_ITEMS; i++) {
        if (strcmp(item_names[i], target_name) == 0) {
            target_id = i;
            break;
        }
    }
    if (target_id == -1) {
        printf("Предмет с названием \"%s\" не найден в каталоге предметов!\n\n", target_name);
        return;
    }
    int found_count = 0;
    printf("Результаты поиска для \"%s\" (ID: %d):\n", target_name, target_id);
    for (int i = 0; i < size; i++) {
        if (inventory[i] == target_id) {
            printf("- Найден в слоте %d\n", i);
            found_count++;
        }
    }
    if (found_count == 0) {
        printf("В рюкзаке сейчас нет предмета \"%s\".\n", target_name);
    }
    printf("\n");
}

//Запись состояния[8]===================================================================
void write_diary(const int inventory[], int size) {
    FILE *diary = fopen("diary.txt", "a");
    if (diary == NULL) {
        printf("Ошибка: не удалось открыть diary.txt для записи!\n\n");
        return;
    }

    printf("\n--- СВОДКА ДНЕВНИКА ФЕРМЕРА ---\n");
    printf("Фермер: %s\n", farmer_name);
    printf("Текущее время: День %d, %02d:00\n", current_day, current_hour);
    printf("Содержимое рюкзака:\n");

    fprintf(diary, "========================================\n");
    fprintf(diary, "Фермер: %s\n", farmer_name);
    fprintf(diary, "Время: День %d, %02d:00\n", current_day, current_hour);
    fprintf(diary, "Инвентарь:\n");

    for (int i = 0; i < size; i++) {
        int id = inventory[i];
        const char *name = (id >= 0 && id < MAX_ITEMS) ? item_names[id] : "Неизвестный";
        printf("  Слот %2d: %s (ID %d)\n", i, name, id);
        fprintf(diary, "  Слот %2d: %s (ID %d)\n", i, name, id);
    }
    fprintf(diary, "========================================\n\n");
    fclose(diary);

    printf("-------------------------------\n");
    printf("[Успешно] Данные записаны в файл diary.txt!\n\n");
}

//Проверка рецептов крафта[9]==========================================================================
void check_crafting_recipes(const int inventory[], int size) {
    FILE *in = fopen("input.txt", "r");
    if (in == NULL) {
        printf("Ошибка: входной файл input.txt не найден!\n\n");
        return;
    }

    FILE *out = fopen("output.txt", "w");
    if (out == NULL) {
        printf("Ошибка: не удалось открыть output.txt для записи!\n\n");
        fclose(in);
        return;
    }
// Подсчитываем, сколько ресурсов каждого типа есть у игрока
    int inv_counts[MAX_ITEMS] = {0};
    for (int i = 0; i < size; i++) {
        if (inventory[i] > 0 && inventory[i] < MAX_ITEMS) {
            inv_counts[inventory[i]]++;
        }
    }

    char line[256];
    printf("\n--- РЕЗУЛЬТАТ ПРОВЕРКИ КРАФТА ---\n");

    while (fgets(line, sizeof(line), in)) {
        // Удаляем перевод строки
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 0) continue;
        char *colon_pos = strchr(line, ':');
        if (colon_pos == NULL) continue;

        *colon_pos = '\0';
        char recipe_name[MAX_NAME_LEN];
        strncpy(recipe_name, line, MAX_NAME_LEN - 1);
        recipe_name[MAX_NAME_LEN - 1] = '\0';

        char *tokens = colon_pos + 1;
        int req_counts[MAX_ITEMS] = {0};
        int req_id;
        int offset = 0;
        int bytes_read;
        while (sscanf(tokens + offset, "%d%n", &req_id, &bytes_read) == 1) {
            if (req_id >= 0 && req_id < MAX_ITEMS) {
                req_counts[req_id]++;
            }
            offset += bytes_read;
        }
        int can_craft = 1;
        for (int i = 1; i < MAX_ITEMS; i++) {
            if (req_counts[i] > inv_counts[i]) {
                can_craft = 0;
                break;
            }
        }
        if (can_craft) {
            printf("%s: [ДОСТУПНО]\n", recipe_name);
            fprintf(out, "%s: [ДОСТУПНО]\n", recipe_name);
        } else {
            printf("%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Недостает: ", recipe_name);
            fprintf(out, "%s: [НЕ ХВАТАЕТ РЕСУРСОВ] (Недостает: ", recipe_name);

            int first = 1;
            for (int i = 1; i < MAX_ITEMS; i++) {
                if (req_counts[i] > inv_counts[i]) {
                    int diff = req_counts[i] - inv_counts[i];
                    if (!first) {
                        printf(", ");
                        fprintf(out, ", ");
                    }
                    printf("%s x%d", item_names[i], diff);
                    fprintf(out, "%s x%d", item_names[i], diff);
                    first = 0;
                }
            }
            printf(")\n");
            fprintf(out, ")\n");
        }
    }

    fclose(in);
    fclose(out);
    printf("--------------------------------\n");
    printf("[Готово] Результаты крафта сохранены в output.txt\n\n");
}

//Создание карты в динамической памяти==========================================================
char **create_map(int height, int width) {
    char **map = (char**)malloc(height * sizeof(char*));
    if (map == NULL) {
        return NULL;
    }
    map_blocks_allocated++;
    for (int i = 0; i < height; i++) {
        map[i] = (char*)malloc(width * sizeof(char));
        if (map[i] == NULL) {
            for (int j = 0; j < i; j++) {
                free(map[j]);
                map_blocks_allocated--;
            }
            free(map);
            map_blocks_allocated--;
            return NULL;
        }
        map_blocks_allocated++;
    }
    return map;
}

//Освобождение памяти карты=====================================================================
void free_map(char **map, int height) {
    if (map == NULL) {
        return;
    }
    for (int i = 0; i < height; i++) {
        free(map[i]);
        map_blocks_allocated--;
    }
    free(map);
    map_blocks_allocated--;
}

//Случайное число от min до max включительно======================================================
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}

//Генерация карты: стены по краям, внутри пол и препятствия=======================================
void generate_map(char **map, int height, int width) {
    int obstacle_chance = random_range(OBSTACLE_MIN_PERCENT, OBSTACLE_MAX_PERCENT);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (y == 0 || y == height - 1 || x == 0 || x == width - 1) {
                map[y][x] = CELL_WALL;
                continue;
            }
            int roll = rand() % PERCENT_TOTAL;
            if (roll < obstacle_chance) {
                map[y][x] = (rand() % 2 == 0) ? CELL_TREE : CELL_ROCK;
            } else if (roll < obstacle_chance + RUNE_PERCENT) {
                map[y][x] = CELL_RUNE;
            } else {
                map[y][x] = CELL_FLOOR;
            }
        }
    }
}

//Поиск случайной свободной клетки======================================================================
int pick_random_free_cell(char **map, int height, int width, int *out_x, int *out_y) {
    int free_count = 0;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == CELL_FLOOR) {
                free_count++;
            }
        }
    }
    if (free_count == 0) {
        return 0;
    }
    int target = rand() % free_count;
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (map[y][x] == CELL_FLOOR) {
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
//Отрисовка карты с героем==========================================================================
void draw_map(char **map, int height, int width, int hero_x, int hero_y) {
    printf("\n");
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == hero_x && y == hero_y) {
                printf("%c ", CELL_HERO);
            } else {
                printf("%c ", map[y][x]);
            }
        }
        printf("\n");
    }
    printf("%c - вы, %c - стена, %c - дерево, %c - камень, %c - руна-телепорт\n",
           CELL_HERO, CELL_WALL, CELL_TREE, CELL_ROCK, CELL_RUNE);
}
//Ввод направления с защитой от дурака===============================================================
char read_direction(void) {
    char line[INPUT_LINE_LEN];
    while (1) {
        printf("Куда идём? (%c - вверх, %c - влево, %c - вниз, %c - вправо, %c - сбежать домой): ",
               KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT, KEY_QUIT);
        if (fgets(line, sizeof(line), stdin) == NULL) {
            return KEY_QUIT;
        }
        if (strchr(line, '\n') == NULL) {
            clear_input_buffer();
        }
        line[strcspn(line, "\r\n")] = '\0';
        if (strlen(line) == 1) {
            char key = (char)toupper((unsigned char)line[0]);
            if (key == KEY_UP || key == KEY_LEFT || key == KEY_DOWN ||
                key == KEY_RIGHT || key == KEY_QUIT) {
                return key;
            }
        }
        printf("Неверный ввод! Введите одну букву: W, A, S, D или Q.\n");
    }
}

//Можно ли наступить на клетку=======================================================================
int is_passable(char cell) {
    return cell == CELL_FLOOR || cell == CELL_RUNE;
}

//Название препятствия================================================================================
const char *obstacle_name(char cell) {
    switch (cell) {
        case CELL_WALL: return "стена";
        case CELL_TREE: return "дерево";
        case CELL_ROCK: return "камень";
        default:        return "препятствие";
    }
}

//Вариант 7: руна-телепорт=========================================================================
void teleport_hero(char **map, int height, int width, int *hero_x, int *hero_y) {
    int new_x = *hero_x;
    int new_y = *hero_y;
    int found = pick_random_free_cell(map, height, width, &new_x, &new_y);
    map[*hero_y][*hero_x] = CELL_FLOOR;
    if (found) {
        *hero_x = new_x;
        *hero_y = new_y;
        printf("Руна вспыхнула и исчезла! Вас перенесло на клетку (%d, %d).\n", new_x, new_y);
    } else {
        printf("Руна погасла, но свободных клеток для телепорта нет.\n");
    }
}

//Перемещение героя с проверками=====================================================================
void move_hero(char **map, int height, int width, int *hero_x, int *hero_y, char direction) {
    int new_x = *hero_x;
    int new_y = *hero_y;
    switch (direction) {
        case KEY_UP:    new_y--; break;
        case KEY_DOWN:  new_y++; break;
        case KEY_LEFT:  new_x--; break;
        case KEY_RIGHT: new_x++; break;
        default:        return;
    }
    if (new_x < 0 || new_x >= width || new_y < 0 || new_y >= height) {
        printf("Дальше дороги нет — край карты!\n");
        return;
    }
    if (!is_passable(map[new_y][new_x])) {
        printf("Туда нельзя: там %s!\n", obstacle_name(map[new_y][new_x]));
        return;
    }
    *hero_x = new_x;
    *hero_y = new_y;
    if (map[new_y][new_x] == CELL_RUNE) {
        teleport_hero(map, height, width, hero_x, hero_y);
    }
}
//летсгоу в приключение
void adventure(void) {
    clear_input_buffer();
    int height = random_range(MAP_MIN_SIZE, MAP_MAX_SIZE);
    int width = random_range(MAP_MIN_SIZE, MAP_MAX_SIZE);
    char **map = create_map(height, width);
    if (map == NULL) {
        printf("Ошибка: не удалось выделить память под карту!\n\n");
        return;
    }
    generate_map(map, height, width);
    int hero_x = 1;
    int hero_y = 1;
    if (!pick_random_free_cell(map, height, width, &hero_x, &hero_y)) {
        map[hero_y][hero_x] = CELL_FLOOR;
    }
    printf("\nВы отправились в приключение! Размер карты: %d x %d\n", width, height);
    printf("Выделено блоков под карту: %d\n", map_blocks_allocated);
    while (1) {
        draw_map(map, height, width, hero_x, hero_y);
        char direction = read_direction();
        if (direction == KEY_QUIT) {
            break;
        }
        move_hero(map, height, width, &hero_x, &hero_y, direction);
    }
    free_map(map, height);
    map = NULL;
    printf("Вы сбежали домой.\n Карта освобождена, занятых блоков осталось: %d\n\n",map_blocks_allocated);
}



//====================================================================================================================
int main()
{ char current_path[1024];
    srand((unsigned int)time(NULL));
    printf("================================================================\n");
    if (get_current_dir(current_path, sizeof(current_path)) != NULL) {
        printf("ТОЧНЫЙ ПУТЬ К ПАПКЕ ПРОГРАММЫ:\n%s\n", current_path);
    }
    printf("Файлы input.txt и items.txt должны лежать именно в этой папке!\n");
    printf("================================================================\n\n");
    
int inventory[inventory_size] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
load_items_from_file();
printf("Введите имя вашего фермера: ");
    if (fgets(farmer_name, sizeof(farmer_name), stdin) != NULL) {
        farmer_name[strcspn(farmer_name, "\r\n")] = '\0';
    }
    if (strlen(farmer_name) == 0) {
        strcpy(farmer_name, "Безымянный");
    }
    printf("\nДобро пожаловать на ферму, %s!\n\n", farmer_name);

//======================================================================================================================  
// Тело Игры
int menu_choice; 
while(1) { 
    info();
    if (scanf("%d", &menu_choice) != 1) {
            printf("Неверный ввод! Введите номер пункта.\n\n");
            clear_input_buffer();
            continue;
    }
switch (menu_choice) {
//==========
case 1:
    print_time();
break;
//==========
case 2:
    to_waste_time();
break;
//==========
case 3:
    print_inventory(inventory,inventory_size);
break;
//==========
case 4:
    add_item(inventory, inventory_size);
break;
//==========
case 5:
    drop_item(inventory, inventory_size);
break;
//==========
case 6: 
    search_heavy_items(inventory, inventory_size);
break;
case 7:
    search_item_by_name(inventory, inventory_size);
break;
case 8:
     write_diary(inventory, inventory_size);
 break;
 case 9:
    check_crafting_recipes(inventory, inventory_size);
break;
//==========
case 0:
    printf("Игра завершена\nПока - пока:)");
    return 0;
//==========
case 10:
    adventure();
    break;
//=========
default:
    printf("Неверный ввод. Попробуйте снова!\n\n\n");
}
}
 return 0; 
}