#include <stdio.h>
#include <stdlib.h>

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
//====================================================================================================================
int main()
{ 
    
    int numbering1;
    int added_time=0;
    int current_day = 1;
    int current_hour = 8;
    int inventory = 10;

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
        break;
    }
case 0:
    printf("Игра завершена.\nПока - пока!\n");
    return 0;

default:
printf("Неверный ввод. Попробуйте снова!\n");

}
}






 return 0; 
}








