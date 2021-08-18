#include "keySpace1.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100 // Максимальный размер ключа

int str_compare(char* s1, char* s2){
// Сравнение строк
//    Возвращает 1, s1 > s2
//               0, s1 = s2
//              -1, s1 < s2
    if ((s1 == NULL) && (s2 == NULL)) return 0;
    if ((s1 == NULL) && (s2 != NULL)) return -1;
    if ((s1 != NULL) && (s2 == NULL)) return 1;
    while ((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2)){
        s1++;
        s2++;
    }
    if ((*s1 == '\0') && (*s2 == '\0')) return 0;
    if (*s1 > *s2) return 1;
    return -1;
}



KeySpace1* ks1_Init(int size){
//    Инициаллизация таблицы
//    Входные данные: размер
//    Выходные данные: указатель на ks1 или NULL в случае ошибки
    if (size > 0){
        return (KeySpace1*)calloc(size, sizeof(KeySpace1));
    }
    return NULL;
}


int ks1_FindIndex(char* requiredKey, KeySpace1* ks, int lvl){
//    Поиск в таблице по ключу (бинарный поиск)
//    Входные данные: Искомый ключ, ks1, количество эл-в
//    Выходные данные: Позиция эл-та или -1
    int up = lvl - 1, low = 0, i;
    while (up >= low){
        i = (up + low) / 2;
        int compare = str_compare(requiredKey, ks[i].key);
        if (compare  == 1) {
            low = i + 1;
        }
        else if (compare == -1){
            up = i - 1;
        }
        else {
            return i;
        }
    }
    return KEY_NF;
}

Node1* getNode(Item* item){
//    Создание объекта Node и запись в него данных
//    Входные данные: указатель на эл-т Item
//    Выходные данные: указатель на эл-т Node1
    Node1 *bufNode = (Node1*)calloc(1, sizeof(Node1));
    bufNode->info = item;
    return bufNode;
}

int ks1_Add(char* key, Item* item, KeySpace1* ks, int* lvl, int max){
//    Добавление элемента в таблицу
//    Входные данные: Ключ, информация, ks, количество эл-в и размер таблицы
//    Выходные данные: 0 - ок, 1 - таблица заполнена

    if (*lvl == max) return TABLE_FULL;                         // Проверка на заполненость таблицы
    Node1* bufNode = getNode(item);
    int pos = ks1_FindIndex(key, ks, *lvl);
    if (pos != -1) {                                            // Если ключ уже есть в таблице
        bufNode->next = ks[pos].node;
        bufNode->release = ks[pos].node->release + 1;
        bufNode->info->index = bufNode->release;
        bufNode->info->ptr1 = ks + pos;
        ks[pos].node = bufNode;
        return ST_OK;
    }
    KeySpace1 *buf = ks;
    int i = *lvl - 1;
    while (i >= 0 && str_compare(buf[i].key, key) > 0){        // Поиск места для вставки
        buf[i + 1] = buf[i];
        i--;
    }
    buf[i + 1].key = (char*)calloc(N, sizeof(char));
    strcpy(buf[i + 1].key, key);                                      // Вставка
    bufNode->info->index = 0;
    bufNode->info->ptr1 = buf + i + 1;
    buf[i + 1].node = bufNode;
    (*lvl)++;
    return ST_OK;
}

KeySpace1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version){
//    Поиск элемента/тов с использованием версии
//    Входные данные: Искомый ключ, ks, кол-во эл-в, версия
//    Выходные данные: указатель на таблицу KS2 с результатами
    int pos = ks1_FindIndex(requiredKey, ks, lvl);
    if (pos == -1) return NULL;
    if (version == -1) {
        KeySpace1 *newKS = (KeySpace1*)calloc(1, sizeof(KeySpace1));
        *newKS = ks[pos];
        return newKS;
    }
    Node1* buf = ks[pos].node;
    while (buf && buf->release > version){
        buf = buf->next;
    }
    if (buf && buf->release == version){
        KeySpace1* bufKS = ks1_Init(1);
        bufKS->key = (char*)calloc(N, sizeof(char));
        strcpy(bufKS->key, requiredKey);
        bufKS->node = buf;
        return bufKS;
    }
    else return NULL;
}

void nodeFree(Node1* el){
    // Очистка памяти ноды
    if (el){
        if (el->info){
            if (el->info->info){
                if (el->info->info->first) free(el->info->info->first);
                if (el->info->info->second) free(el->info->info->second);
                free(el->info->info);
            }
            free(el->info);
        }
        free(el);
    }
}

int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version, KeySpace1* base, Table* t){
//  Удаление элемента по ключу и версии(или всех эл-в с конкретным ключем)
//  Входные данные: ключ удаляемого эл-та, ks1, кол-во эл-в, версия, указатель на эл-т или NULL, nf,kbwf
//  Выходные данные: 0 - ок, 2 - эл-т с заданным ключем не найден, 3 - эл-т с заданной версией не найден
    int pos;
    if (base){
        pos =(int)(base - ks);
    }
    else {
        pos = ks1_FindIndex(deletedKey, ks, *lvl);
        if (pos == -1) return EL_NOTFOUND;
    }
    if (version != -1){                                     // Удаление по версии
        Node1 *buf = ks[pos].node;
        if (buf->release == version){                       // Если первый элемент
            ks[pos].node = ks[pos].node->next;
            ks2_Delete(buf->info->key2, t->ks2, t->msize2, 1);
            nodeFree(buf);
            if (ks[pos].node) return ST_OK;                 // Если элемент был единственным
            else {
                for (int i = pos; i < *lvl - 1; i++){
                    ks[i] = ks[i + 1];
                }
                free(ks[*lvl - 1].key);
                (*lvl)--;
                return ST_OK;
            }
        }
        Node1 *par = NULL;
        while (buf && buf->release > version){              // Ищем нужный
            par = buf;
            buf = buf->next;
        }
        if (buf && buf->release == version){                // Проверка
            if (par){
                par->next = buf->next;
            }
            ks2_Delete(buf->info->key2, t->ks2, t->msize2, 1);
            nodeFree(buf);
            return ST_OK;
        }
        else return VERS_NOTFOUND;
    }

    Node1 *delEl = (ks + pos)->node;                // Удаление без версии
    Node1 *bufEl = delEl;
    for (int i = pos; i < *lvl - 1; i++){
        ks[i] = ks[i + 1];
    }
    while (bufEl){
        delEl = bufEl;
        bufEl = bufEl->next;
        ks2_Delete(delEl->info->key2, t->ks2, t->msize2, 1);
        nodeFree(delEl);
    }
    free(ks[*lvl - 1].key);
    (*lvl)--;
    return ST_OK;
}

void ks1_Print(KeySpace1* ks, int lvl){
//    Вывод таблицы

    for (int i = 0; i < lvl; i++, ks++){
        printf("%s %d ", ks->key, ks->node->release);
        Node1 *buf = ks->node;
        while (buf->next){
            buf = buf->next;
            printf("%d ", buf->release);
        }
        printf("\n");
    }
}

void ks1_Free(KeySpace1* ks, int lvl){
    // Очистка памяти
    KeySpace1 *bufKS = ks;
    for (int i = 0; i < lvl; i++, ks++){
        Node1 *buf = ks->node;
        Node1 *del = NULL;
        while (buf){
            del = buf;
            buf = buf->next;
            nodeFree(del);
        }
        free(ks->key);
    }
    free(bufKS);
}
