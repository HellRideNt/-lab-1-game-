#include <stdio.h>
#include <string.h>
#define inventory_size 10 


// Функция которая чистит буфер от всякой каки
void clear_input_buffer() {
    while (getchar() != '\n');
}
// Функция для информации

void info() {
    printf("============================МЕНЮ================================\n");
    printf("[1] Выводит на экран текущий день и час\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет \n");
    printf("[6] Поиск тяжестей\n");
    printf("[0] Завершение программы\n");
    printf("================================================================\n");
    printf("Выберите цифру которая соответсвует вашему запросу:\n");
}

//Список нашего инвентаря======================================================================

    const char* const item_names[10]={
    "Пусто",
    "Дерево",
    "Камень",
    "Семена",
    "Меч",
    "Каменная кирка",
    "Яблоко",
    "Факел",
    "Туалетная бумага",
    "Вода"
};
//=============================================================================================

int menu_choice;
int hours_to_add=0;
int current_day = 1;
int current_hour = 8;
int item_id;
int index_slot;

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

//====================================================================================================================
int main()
{ 
int inventory[inventory_size] = {0};
inventory[0] = 1;
inventory[1] = 2;
inventory[2] = 3;
inventory[3] = 4;
inventory[4] = 5;
inventory[5] = 6;
inventory[6] = 7;
inventory[7] = 8;
inventory[8] = 9;

//======================================================================================================================  
// Тело Игры 
while(1) { 
    info();
    
scanf("%d", &menu_choice);
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