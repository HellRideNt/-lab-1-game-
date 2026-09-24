#include <stdio.h>
#include <string.h>
#define inventory_size 10
// Функция для информации

void info() {
    printf("============================МЕНЮ================================\n");
    printf("[1] Выводит на экран текущий день и час\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет \n");
    printf("[6] Инверсия предметов\n");
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

//Это значит у нас функция которая выводит слоты===============================================
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
    printf("Результат:\n");
    printf("День %d - %d:00 \n\n\n",current_day,current_hour);
break;
//==========
case 2:
    printf("На сколько вы бы хотели промотать время: \n");
    if (scanf("%d", &hours_to_add) != 1 || hours_to_add < 0) {
        printf("Ошибка: введите положительное число часов!\n");
        while (getchar() != '\n');
        break;
    }
    current_hour += hours_to_add;
    current_day += current_hour / 24;
    current_hour = current_hour % 24;
    printf("День %d - %02d:00\n\n\n", current_day, current_hour);
break;
//==========
case 3:
    print_inventory(inventory,inventory_size);
break;
//==========
case 4:
    printf("Введите индекс слота (от 0 до %d): \n", inventory_size-1);
    scanf("%d", &index_slot);
    if (index_slot < 0 || index_slot >= inventory_size) {
        printf("Error: слот с таким индексом не сущетсвует!\nДопустимо от 0 до %d\n", inventory_size - 1);
    } else {
        printf("Введите ID предмета: \n");
        scanf("%d", &item_id);
        inventory[index_slot] = item_id;
        printf("Предмет успешно добавлен в слот %d\n\n\n",index_slot);
    }
break;
//==========
case 5:
    printf("Введите индекс слота предмета, который хотели бы выбросить (от 0 до %d): \n", inventory_size - 1);
    if (scanf("%d", &index_slot) != 1) {
        printf("Ошибка ввода! Введите число.\n");
        while (getchar() != '\n'); 
        break;
    }
    if (index_slot < 0 || index_slot >= inventory_size) {
        printf("Ошибка: слот с индексом %d не существует!\n", index_slot);
break;
    }
    if (inventory[index_slot] == 0) {
        printf("Слот %d уже пуст.\n", index_slot);
break;
    }
    printf("Предмет с ID %d выброшен из слота %d.\n\n\n", inventory[index_slot], index_slot);
    inventory[index_slot]=0;   
break;
//==========
case 6:
    printf("===================Инвентарь до переворота======================\n\n");
    print_inventory(inventory, inventory_size);
    for (int i = 0; i < inventory_size / 2; i++) {
        int opposite_index = inventory_size - 1 - i;
        int temp = inventory[i];
        inventory[i] = inventory[opposite_index];
        inventory[opposite_index] = temp;
    }
    printf("===================Инвентарь после переворота===================\n\n");
    print_inventory(inventory, inventory_size);
    printf("Инвентарь успешно перевернут!\n\n\n");
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