#include "keySpace1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100 // Максимальный размер ключа

/* (1) Инициаллизация
 * (2) Поиск
 * (3) Добавление
 * (4) Удаление
 * (5) Очистка памяти
 *
 *  Упорядоченая таблица, вектор, ключи могут повторяться (версии)
 *  () - поиск в таблице всех версий элемента, заданного ключом, или конкретной (заданной) версии элемента,
 * также заданного своим ключом; результатом поиска должна быть новая таблица, содержащая найденные элементы;
 *  () - удаление из таблицы всех элементов с заданным ключом или элемента определенной версии, также заданного своим ключом.
 */

typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
}ans;


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
//    Поиск в таблице по ключу
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

Node1* getNode(char* first, char* second){
//    Создание объекта Node и запись в него данных
    Node1 *bufNode = (Node1*)calloc(1, sizeof(Node1));
    bufNode->info = (Item*)calloc(1, sizeof(Item));
    bufNode->info->info = (InfoType*)calloc(1, sizeof(InfoType));
    bufNode->info->info->first = (char*)malloc((strlen(first) + 1) * sizeof(char));
    bufNode->info->info->second = (char*)malloc((strlen(second) + 1) * sizeof(char));
    strcpy(bufNode->info->info->first, first);
    strcpy(bufNode->info->info->second, second);
    return bufNode;
}

int ks1_Add(char* key, char* first, char* second, KeySpace1* ks, int* lvl, int max){
//    Добавление элемента в таблицу
//    Входные данные: Ключ, информация, ks, количество эл-в и размер таблицы
//    Выходные данные: 0 - ок, 1 - таблица заполнена

    if (*lvl == max) return TABLE_FULL;                         // Проверка на заполненость таблицы
    Node1* bufNode = getNode(first, second);
    int pos = ks1_FindIndex(key, ks, *lvl);
    if (pos != -1) {                                            // Если ключ уже есть в таблице
        bufNode->next = ks[pos].node;
        bufNode->release = ks[pos].node->release + 1;
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
    buf[i + 1].node = bufNode;
    (*lvl)++;
    return ST_OK;
}

KeySpace1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version){
    // Поиск элемента/тов с использованием версии
    // Входные данные: Искомый ключ, ks, кол-во эл-в, версия

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

int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version){
//  Удаление элемента по ключу и версии(или всех эл-в с конкретным ключем)
//  Входные данные: ключ удаляемого эл-та, ks, кол-во эл-в, версия
//  Выходные данные: 0 - ок, 2 - эл-т с заданным ключем не найден, 3 - эл-т с заданной версией не найден

    int pos = ks1_FindIndex(deletedKey, ks, *lvl);
    if (pos == -1) return EL_NOTFOUND;
    if (version != -1){                                     // Удаление по версии
        Node1 *buf = ks[pos].node;
        if (buf->release == version){                       // Если первый элемент
            ks[pos].node = ks[pos].node->next;
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
            par->next = buf->next;
            nodeFree(buf);
            return ST_OK;
        }
        else return VERS_NOTFOUND;
    }

    Node1 *delEl = (ks + pos)->node;
    Node1 *bufEl = delEl;
    for (int i = pos; i < *lvl - 1; i++){
        ks[i] = ks[i + 1];
    }
    while (bufEl){
        delEl = bufEl;
        bufEl = bufEl->next;
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


//int main(){
//    KeySpace1* table = ks1_Init(6);
//    int lvl = 0;
//    char* s = "1";
//    ks1_Add("7", s, s, table, &lvl, 6);
//    ks1_Add("6", s, s, table, &lvl, 6);
//    ks1_Add("5", s, s, table, &lvl, 6);
//    ks1_Add("4", s, s, table, &lvl, 6);
//    ks1_Add("3", s, s, table, &lvl, 6);
//    ks1_Add("2", s, s, table, &lvl, 6);
//    ks1_Add("1", s, s, table, &lvl, 6);
////    ks1_Print(table, lvl);
////    printf("\n");
////    ks1_Delete("7", table, &lvl, -1);
////    ks1_Add("7", s, s, table, &lvl, 6);
////    ks1_Delete("5", table, &lvl, 0);
////    ks1_Print(table, lvl);
//    ks1_Free(table, lvl);
//    return 0;
//}


