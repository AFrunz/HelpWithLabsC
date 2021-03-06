#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keySpace1.h"
//#define ks1file "/student/b20503/b2050326/help/lab3b/ks1.bin"
//#define ks2file "/student/b20503/b2050326/help/lab3b/ks2.bin"
//#define infofile "/student/b20503/b2050326/help/lab3b/info.bin"
//#define ks1file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks1.bin"
//#define ks2file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks2.bin"
//#define infofile "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\info.bin"
#define N 100 // Максимальный размер ключа

//
//typedef struct InfoType {
//    char *first, *second;
//}InfoType;



//typedef struct Item{
//    InfoType *info;
//    KeySpace1* ptr1;     // указатель на эл-т 1го пр-ва
//    int index;      // версия этого эл-та
//    unsigned int key2; // ключ из 2-го пр-ва
//}Item;


typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4
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

char* freadStr(long int pos, FILE *f){
    if (pos == 0) return NULL;
    char* string = (char*)calloc(N, sizeof(char));
    fseek(f, pos, SEEK_SET);
    fread(string, sizeof(char), N, f);
    return string;
}

void ks1_Push(char* Ks1FileName, KeySpace1* ks, int msize1, int lvl){
//    Загрузка таблицы в файл
    FILE *f = fopen(Ks1FileName, "r+b");
    if (!f) {
        perror("Error\n");
        return;
    }
    fwrite(&msize1, 1, sizeof(int), f);
    fwrite(&lvl, 1, sizeof(int), f);
    fwrite(ks, msize1, sizeof(KeySpace1), f);
    fclose(f);
}

KeySpace1* ks1_Pull(char* ks1FileName, int newSize1, int* lvl, int* msize1){
    if (newSize1 != -1) {
        KeySpace1 *space = (KeySpace1*)calloc(newSize1, sizeof(KeySpace1));
        *lvl = 0;
        ks1_Push(ks1FileName, space, newSize1, *lvl);
        *msize1 = newSize1;
        return space;
    }
    int n;
    FILE *f = fopen(ks1FileName, "r+b");
    fread(&n, sizeof(int), 1, f);
    fread(lvl, sizeof(int), 1, f);
    KeySpace1* space = (KeySpace1*)calloc(n, sizeof(KeySpace1));
    fread(space, sizeof(KeySpace1), n, f);
    KeySpace1* buf = space;
    for (int i = 0; i < *lvl; i++, buf++){
        buf->key = freadStr(buf->keyPos, f);
    }
    fclose(f);
    *msize1 = n;
    return space;
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


Node1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version, char* Ks1FileName){
//    Поиск элемента/тов с использованием версии
//    Входные данные: Искомый ключ, ks, кол-во эл-в, версия
//    Выходные данные: указатель на таблицу KS2 с результатами
    int pos = ks1_FindIndex(requiredKey, ks, lvl);
    if (pos == -1) return NULL;
    FILE *f = fopen(Ks1FileName, "r+b");
    fseek(f, ks[pos].node, SEEK_SET);
    Node1* buf = (Node1*)calloc(1, sizeof(Node1));
    fread(buf, sizeof(Node1), 1, f);
    while (buf->next && buf->release > version){
        fseek(f, buf->next, SEEK_SET);
        fread(buf, sizeof(Node1), 1, f);
    }
    fclose(f);
    if (buf->release == version){
        return buf;
    }
    else {
        free(buf);
        return NULL;
    }
}

Node1* getNode(long int item){
//    Создание объекта Node и запись в него данных
//    Входные данные: указатель на эл-т Item
//    Выходные данные: указатель на эл-т Node1
    Node1 *bufNode = (Node1*)calloc(1, sizeof(Node1));
    bufNode->info = item;
    return bufNode;
}

int ks1_Add(char* key, long int item, KeySpace1* ks, int* lvl, int max, char* Ks1FileName, int* index, int *ks1Pos){
//    Добавление элемента в таблицу
//    Входные данные: Ключ, информация, ks, количество эл-в и размер таблицы
//    Выходные данные: 0 - ок, 1 - таблица заполнена

    if (*lvl == max) return TABLE_FULL;                         // Проверка на заполненость таблицы
    Node1* bufNode = getNode(item);
    int pos = ks1_FindIndex(key, ks, *lvl);
    FILE *f = fopen(Ks1FileName, "r+b");
    if (pos != -1) {                                            // Если ключ уже есть в таблице
        bufNode->next = ks[pos].node;
        Node1* fileNode = (Node1*)calloc(1, sizeof(Node1));
        fseek(f, ks[pos].node, SEEK_SET);
        fread(fileNode, sizeof(Node1), 1, f);
        bufNode->release = fileNode->release + 1;
        *index = bufNode->release;
//        bufNode->info->index = bufNode->release;
//        bufNode->info->ptr1 = ks + pos;
        fseek(f, 0, SEEK_END);
        long int bufPos = ftell(f);
        fwrite(bufNode, sizeof(Node1), 1, f);
        ks[pos].node = bufPos;
        fclose(f);
        free(fileNode);
        free(bufNode);
        *ks1Pos = pos;
        return ST_OK;
    }
    *index = 0;
    KeySpace1 *buf = ks;
    int i = *lvl - 1;
    while (i >= 0 && str_compare(buf[i].key, key) > 0){        // Поиск места для вставки
        buf[i + 1] = buf[i];
        i--;
    }
    buf[i + 1].key = (char*)calloc(N, sizeof(char));
    fseek(f, 0, SEEK_END);
    long int keyPos = ftell(f);
    fwrite(key, sizeof(char), N, f);
    strcpy(buf[i + 1].key, key);                                      // Вставка
    buf[i + 1].keyPos = keyPos;
//    bufNode->info->index = 0;
//    bufNode->info->ptr1 = buf + i + 1;
    fseek(f, 0, SEEK_END);
    long int bufPos = ftell(f);
    fwrite(bufNode, sizeof(Node1), 1, f);
    free(bufNode);
    buf[i + 1].node = bufPos;
    (*lvl)++;
    fclose(f);
    *ks1Pos = i + 1;
    return ST_OK;
}

int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version, int base, char* Ks1FileName){
//  Удаление элемента по ключу и версии(или всех эл-в с конкретным ключом)
//  Входные данные: ключ удаляемого эл-та, ks1, кол-во эл-в, версия, указатель на эл-т или NULL, nf,kbwf
//  Выходные данные: 0 - ок, 2 - эл-т с заданным ключом не найден, 3 - эл-т с заданной версией не найден
    int pos;
    if (base != -1){
        pos = base;
    }
    else {
        pos = ks1_FindIndex(deletedKey, ks, *lvl);
        if (pos == -1) return EL_NOTFOUND;
    }
    FILE *f = fopen(Ks1FileName, "r+b");
    Node1* buf = (Node1*)calloc(1, sizeof(Node1));
    fseek(f, ks[pos].node, SEEK_SET);
    fread(buf, sizeof(Node1), 1, f);
    if (buf->release == version){                       // Если первый элемент
        ks[pos].node = buf->next;
//            nodeFree(buf);
        if (ks[pos].node != 0){                 // Если элемент не был единственным
//            ks[pos].keyPos = 0;
//            free(ks[pos].key);
            free(buf);
            fclose(f);
            return ST_OK;
        }
        else {
            free(ks[pos].key);
            for (int i = pos; i < *lvl - 1; i++){
                ks[i] = ks[i + 1];
            }
            ks[*lvl - 1].key = NULL;
            ks[*lvl - 1].keyPos = 0;
            (*lvl)--;
            fclose(f);
            free(buf);
            return ST_OK;
        }
    }
    Node1 *par = (Node1*)calloc(1, sizeof (Node1));
    long int parPos = ks[pos].node;
    *par = *buf;
    fseek(f, buf->next, SEEK_SET);
    fread(buf, sizeof(Node1), 1, f);
    while (buf->next && buf->release > version){              // Ищем нужный
        parPos = par->next;
        *par = *buf;
        fseek(f, buf->next, SEEK_SET);
        fread(buf, sizeof(Node1), 1, f);
    }
    if (buf->release == version){                // Проверка
        par->next = buf->next;
        fseek(f, parPos, SEEK_SET);
        fwrite(par, sizeof(Node1), 1, f);
        fclose(f);
        free(buf);
        free(par);
        return ST_OK;
    }
    else{
        fclose(f);
        free(par);
        free(buf);
        return VERS_NOTFOUND;
    }
}


void ks1_Free(KeySpace1* ks, int lvl){
    for (int i = 0; i < lvl; i++){
        if (ks[i].key) free(ks[i].key);
    }
    free(ks);
}

void ks1_Print(char* fileName){
    FILE *f = fopen(fileName, "r+b");
    int n;
    int lvl;
    fread(&n, sizeof(int), 1, f);
    fread(&lvl, sizeof(int), 1, f);
    KeySpace1 *space = (KeySpace1*)calloc(n, sizeof(KeySpace1));
    fread(space, sizeof(KeySpace1), n, f);
    KeySpace1 *buf = space;
    for (int i = 0; i < n; i++, space++){
        space->key = freadStr(space->keyPos, f);
        if (space->key && i < lvl) printf("%s\n", space->key);
        else printf("NULL\n");
    }
    fclose(f);
    ks1_Free(buf, lvl);
}


//int main(){
//    int msize1 = 10;
//    int lvl = 0;
//    int index;
////    ks1_Print(ks1file);
//    KeySpace1* ks1 = ks1_Pull(ks1file, msize1, &lvl);
////    ks1_Add("f", 123, ks1, &lvl, msize1, ks1file, &index);
////    ks1_Add("d", 123, ks1, &lvl, msize1, ks1file, &index);
////    ks1_Add("a", 123, ks1, &lvl, msize1, ks1file, &index);
////    ks1_Add("t", 123, ks1, &lvl, msize1, ks1file, &index);
////    ks1_Add("w", 123, ks1, &lvl, msize1, ks1file, &index);
////    ks1_Add("w", 123, ks1, &lvl, msize1, ks1file, &index);
//    ks1_Delete("w", ks1, &lvl, 1, NULL, ks1file);
//    ks1_Push(ks1file, ks1, msize1, lvl);
//    ks1_Free(ks1, msize1, lvl);
//    ks1_Print(ks1file);
//    return 0;
//}