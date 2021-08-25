#include "keySpace2.h"
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


//KeySpace2** ks2_Pull(char* fileName){
//    FILE *f = fopen(fileName, "r+");
//    if (!f) {
//        return NULL;
//    }
//    int n;
//    fread(&n, sizeof(int), 1, f);
//    KeySpace2* space = (KeySpace2*)calloc(n, sizeof(KeySpace2));
//    fread(space, sizeof(KeySpace2), n, f);
//
//}


KeySpace2** ks2_Init(int size){
//    Выделение памяти под таблицу
     return (KeySpace2**)calloc(size, sizeof(KeySpace2*));
}

int hash_f(unsigned int key, int max){
//    Хэш-функция
    return ((int)key % max);
}

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2** ks, int max){
//    Поиск по ks2
//    Входные данные: искомый ключ, ks, размер таблицы
//    Выходные данные: указатель на эл-т
    int pos = hash_f(requiredKey, max);
    if (!ks[pos]) return NULL;
    KeySpace2* buf = ks[pos];
    while (buf){
        if (buf->key == requiredKey){
            return buf;
        }
        buf = buf->next;
    }
    return NULL;
}

KeySpace2* getKS(Item* item, unsigned int key){
//    Создание элемента ks и запись в него данных
//    Входные данные: данные, ключ
//    Выходные данные: указатель на эл-т ks2
    KeySpace2 * bufKS = (KeySpace2*)calloc(1, sizeof(KeySpace2));
    bufKS->info = item;
    bufKS->info->key2 = key;
    bufKS->key = key;
    return bufKS;
}


int ks2_Add(unsigned int key, Item* item, KeySpace2** ks, int max){
//    Добавление элемента
//    Входные данные: ключ, данные, ks, размер таблицы
//    Выходные данные: код ошибки
    if (ks2_Find(key, ks, max)) return KEY_AE;
    int pos = hash_f(key, max);
    KeySpace2* bufKS = getKS(item, key);
    bufKS->next = ks[pos];
    ks[pos] = bufKS;
    return ST_OK;
}

void freeKS(KeySpace2* el, int flag){
//    Очистка памяти элемента KeySpace2
//    Входные данные: элемент, флаг(0 - очищать данные, 1 - нет)
//    Выходные данные:
    if (el){
        if (el->info && !flag){
            if (el->info->info){
                if (el->info->info->second) free(el->info->info->second);
                if (el->info->info->first) free(el->info->info->first);
                free(el->info->info);
            }
            free(el->info);
        }
        free(el);
    }
}

int ks2_Delete(unsigned int deletedKey, KeySpace2** ks, int max, int flag){
//    Удаление эл-та ks2
//    Входные данные: ключ, ks, размер таблицы, флаг(0 - очищать данные, 1 - нет)
//    Выходные данные: код ошибки
    KeySpace2* element = ks2_Find(deletedKey, ks, max);
    if (!element) return EL_NOTFOUND;
    int pos = hash_f(deletedKey, max);
    KeySpace2* par = NULL;
    KeySpace2* buf = ks[pos];
    while (buf) {
        if (buf->key == deletedKey){
            if (par){
                par->next = buf->next;
            }
            else {
                ks[pos] = buf->next;
            }
            break;
        }
        par = buf;
        buf = buf->next;
    }
    freeKS(buf, flag);
    return ST_OK;
}

void ks2_Free(KeySpace2** ks, int max){
//    Очистка памяти
    KeySpace2** bufKS = ks;
    for (int i = 0; i < max; i++, ks++){
        if (*ks){
            KeySpace2* buf = *ks;
            KeySpace2* del = NULL;
            while (buf){
                del = buf;
                buf = buf->next;
                freeKS(del, 1);
            }
        }
    }
    free(bufKS);
}

void ks2_Print(KeySpace2** ks, int max){

    for (int i = 0; i < max; i++, ks++){
        if (*ks){
            KeySpace2 *buf = *ks;
            while (buf){
                printf("%3u ", buf->key);
                buf = buf->next;
            }
            printf("\n");
        }
        else {
            printf("NULL\n");
        }
    }
}

