#include "table.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


Table* table_Init(int size1, int size2){
//    Инициаллизация таблицы
    Table* table = (Table*)calloc(1, sizeof(Table));
    table->ks1 = ks1_Init(size1);
    table->ks2 = ks2_Init(size2);
    table->msize1 = size1;
    table->msize2 = size2;
    return table;
}

Item* getItem(char* first, char* second){
//    Запись данных в Item
    Item* item = (Item*)calloc(1, sizeof(Item));
    item->info = (InfoType*)calloc(1, sizeof(InfoType));
    item->info->first = (char*)malloc((strlen(first) + 1) * sizeof(char));
    item->info->second = (char*)malloc((strlen(second) + 1) * sizeof(char));
    strcpy(item->info->first, first);
    strcpy(item->info->second, second);
    return item;
}

int table_Add(char* key1, unsigned int key2, Table* t, char* first, char* second){
//    Добавление по (key1; key2)
    if (ks2_Find(key2, t->ks2, t->msize2)) return KEY2_AE;
    if (t->csize1 == t->msize1) return TABLE_FULL;
    Item* item = getItem(first, second);
    ks1_Add(key1, item, t->ks1, &(t->csize1), t->msize1);
    ks2_Add(key2, item, t->ks2, t->msize2);
    return ST_OK;
}

Item* table_Find(char* key1, unsigned int key2, Table* t){
//    Поиск по (Key1; Key2)
    KeySpace2* block = ks2_Find(key2, t->ks2, t->msize2);
    if (!block) return NULL;
    if (str_compare(block->info->ptr1->key, key1) == 0) return block->info;
    return NULL;
}

int table_Delete(char* key1, unsigned int key2, Table* t){
//    Удаление по (Key1; key2)
    Item* block = table_Find(key1, key2, t);
    if (!block) return EL_NOTFOUND;
    ks1_Delete(key1, t->ks1, &(t->csize1), block->index, block->ptr1, t);
    return ST_OK;
}

void table_Print(Table* t){
//    Печать таблицы
    KeySpace2 **ks = t->ks2;
    int max = t->msize2;
    printf("%3s %3s %6s %6s\n", "key1", "key2", "info1", "info2");
    for (int i = 0; i < max; i++, ks++){
        if (*ks){
            KeySpace2 *buf = *ks;
            while (buf){
                printf("%3s %3u %6s %6s\n", buf->info->ptr1->key, buf->key, buf->info->info->first, buf->info->info->second);
                buf = buf->next;
            }
        }
    }
}

void table_Free(Table* t){
//    Очистка памяти под таблицу
    ks1_Free(t->ks1, t->csize1);
    ks2_Free(t->ks2, t->msize2);
    free(t);
}





