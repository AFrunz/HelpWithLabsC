//#include "keySpace2.h"
#include <stdio.h>
#include <stdlib.h>


typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4
}ans;



typedef struct InfoType {
    char *first, *second;
}InfoType;

typedef struct Item{
    InfoType *info;
//    KeySpace1* ptr1;     // указатель на эл-т 1го пр-ва
    int index;      // версия этого эл-та
    unsigned int key2; // ключ из 2-го пр-ва
}Item;

typedef struct KeySpace2{
    int status;         // заполненость эл-та (0 - эл-т пустой, 1 - эл-т заполнен)
    unsigned int key; /* ключ элемента */
    fpos_t info;       /* указатель на информацию  Item*  */
    fpos_t next; /* указатель на следующий элемент struct KeySpace2*  */
}KeySpace2;

KeySpace2* ks2_Pull(char* ks2FileName){
//    Загрузка таблицы из файла
    FILE *f = fopen(ks2FileName, "r+");
    if (!f) {
        return NULL;
    }
    int n;
    fread(&n, sizeof(int), 1, f);
    KeySpace2* space = (KeySpace2*)calloc(n, sizeof(KeySpace2));
    fread(space, sizeof(KeySpace2), n, f);
    return space;
}


void ks2_Push(char* Ks2FileName, KeySpace2* ks, int n){
//    Загрузка таблицы в файл
    FILE *f = fopen(Ks2FileName, "r+");
    fwrite(&n, 1, sizeof(int), f);
    fwrite(ks, n, sizeof(KeySpace2), f);
    fclose(f);
}


int hash_f(unsigned int key, int max){
//    Хэш-функция
    return ((int)key % max);
}

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2* ks, int max, char* Ks2FileName){
//    Поиск по ks2
//    Входные данные: искомый ключ, ks, размер таблицы
//    Выходные данные: указатель на эл-т
    int pos = hash_f(requiredKey, max);
    if (!(ks + pos)->status) return NULL;
    KeySpace2* buf = ks + pos;
    FILE *f = fopen(Ks2FileName, "r+");
    while (buf){
        if (buf->key == requiredKey){
            fclose(f);
            return buf;
        }
        fseek(f, (long)buf->next, SEEK_SET);
        fread(&buf, 1, sizeof(KeySpace2), f);
    }
    fclose(f);
    return NULL;
}

//KeySpace2* getKS(Item* item, unsigned int key){
////    Создание элемента ks и запись в него данных
////    Входные данные: данные, ключ
////    Выходные данные: указатель на эл-т ks2
//    KeySpace2* bufKS = (KeySpace2*)calloc(1, sizeof(KeySpace2));
//    bufKS->info = item;
//    bufKS->info->key2 = key;
//    bufKS->key = key;
//    return bufKS;
//}



int ks2_Add(unsigned int key, Item* item, KeySpace2* ks, int max, char* Ks2FileName){
//    Добавление элемента
//    Входные данные: ключ, данные, ks, размер таблицы
//    Выходные данные: код ошибки
    if (ks2_Find(key, ks, max, Ks2FileName)) return KEY_AE;
    int pos = hash_f(key, max);
//    KeySpace2* bufKS = getKS(item, key);
    bufKS->next = ks[pos];
    ks[pos] = bufKS;
    return ST_OK;
}
