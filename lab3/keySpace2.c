#include "keySpace2.h"
#include <stdio.h>
#include <stdlib.h>



/*   * (2) Перемешанная таблица, сцеплением, ключи уникальны

 *    (1) Инициаллизация
 *    (2) Поиск
 *    (3) Добавление эл-та
 *    (4) Удаление эл-та
 *    (5) Очистка памяти

 */
typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4
}ans;





KeySpace2** ks2_Init(int size){
//    Выделение памяти под таблицу
     return (KeySpace2**)calloc(size, sizeof(KeySpace2*));
}

int hash_f(unsigned int key, int max){
    return ((int)key % max);
}

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2** ks, int max){
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
    KeySpace2 * bufKS = (KeySpace2*)calloc(1, sizeof(KeySpace2));
    bufKS->info = item;
//    bufKS->info = (Item*)calloc(1, sizeof(Item));
//    bufKS->info->info = (InfoType*)calloc(1, sizeof(InfoType));
//    bufKS->info->info->second = (char*)calloc(strlen(second) + 1, sizeof(char));
//    bufKS->info->info->first = (char*)calloc(strlen(first) + 1, sizeof(char));
//    strcpy(bufKS->info->info->first, first);
//    strcpy(bufKS->info->info->second, second);
    bufKS->key = key;
    return bufKS;
}


int ks2_Add(unsigned int key, Item* item, KeySpace2** ks, int max){
    if (ks2_Find(key, ks, max)) return KEY_AE;
    int pos = hash_f(key, max);
    KeySpace2* bufKS = getKS(item, key);
    bufKS->next = ks[pos];
    ks[pos] = bufKS;
    return ST_OK;
}

void freeKS(KeySpace2* el, int flag){
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
    KeySpace2** bufKS = ks;
    for (int i = 0; i < max; i++, ks++){
        if (*ks){
            KeySpace2* buf = *ks;
            KeySpace2* del = NULL;
            while (buf){
                del = buf;
                buf = buf->next;
                freeKS(del, 0);
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


//int main() {
//    KeySpace2 **table = ks2_Init(10);
//    int lvl = 0;
//    char *s = "1";
//    ks2_Add(7, s, s, table, 10);
//    ks2_Add(6, s, s, table, 10);
//    ks2_Add(5, s, s, table, 10);
//    ks2_Add(4, s, s, table, 10);
//    ks2_Add(3, s, s, table, 10);
//    ks2_Add(2, s, s, table, 10);
//    ks2_Add(6, s, s, table, 10);
////    ks2_Delete(1, table, 10);
////    ks2_Delete(7, table, 10);
//    ks2_Print(table, 10);
//    ks2_Free(table, 10);
//    return 0;
//}


