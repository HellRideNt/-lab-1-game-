#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define inventory_size 10
// Функция для информации

void info() {
    printf("============================================================\n");
    printf("[1] Выводит на экран текущий день и час\n");
    printf("[2] Промотать время (Поработать)\n");
    printf("[3] Посмотреть инвентарь\n");
    printf("[4] Положить предмет в слот\n");
    printf("[5] Выбросить предмет \n");
    printf("[6] \n");
    printf("[0] Завершение программы\n");
    printf("============================================================\n");
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

int numbering1;
int added_time=0;
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
        printf("Cлот %2d: [ID:%d] Неизвсетный предмет\n", i, item_id);
    }
}
printf("=====================================================================\n");
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
    
scanf("%d", &numbering1);
switch (numbering1) {
case 1:
    printf("Результат:\n");
    printf("День %d - %d:00 \n",current_day,current_hour);
break;
case 2:
    printf("На сколько вы бы хотели промотать время\n");
    scanf("%d", &added_time);
    current_hour=current_hour+added_time;
    if (current_hour <= 23){
        printf("День %d - %d:00\n",current_day,current_hour);
     }else {
        current_day+=1;
        current_hour=current_hour-24;
        printf("День %d - %d:00\n",current_day,current_hour);
    }
break;
case 3:
    print_inventory(inventory,inventory_size);
break;

case 4:
    printf("Введите индекс слота (от 0 до %d): \n", inventory_size-1);
    scanf("%d", &index_slot);
    if (index_slot < 0 || index_slot >= inventory_size) {
        printf("Error: слот с таким индексом не сущетсвует!\nДопустимо от 0 до %d\n", inventory_size - 1);
    } else {
        printf("Введите ID предмета: \n");
        scanf("%d", &item_id);
        inventory[index_slot] = item_id;
        printf("Предмет успешно добавлен в слот %d\n",index_slot);
    }
break;

case 0:
    printf("Игра завершена.\nПока - пока!\n");
    return 0;

default:
    printf("Неверный ввод. Попробуйте снова!\n");
}
}
 return 0; 
}





