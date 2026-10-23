#include <stdio.h>
#include <string.h>
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

//====================================================================================================================
int main()
{ char current_path[1024];
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
default:
    printf("Неверный ввод. Попробуйте снова!\n\n\n");
}
}
 return 0; 
}