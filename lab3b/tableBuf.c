#include "keySpace1.h"
#include "keySpace2.h"
#include "table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 100


//char[n] key1 uint key2

char* readStr(FILE* f, long int pos){
    fseek(f, pos, SEEK_SET);
    char* string = (char*)calloc(1, sizeof(char));
    int len = 0;
    char sym = '1';
    while (sym != '\0'){
        fread(&sym, sizeof(char), 1, f);
        len++;
        string = realloc(string, len * sizeof(char));
        string[len - 1] = sym;
    }
    string[len] = '\0';
    return string;
}

table* table_Pull(char* fks1, char* fks2, int msize1, int msize2){
//    Считывание таблицы из файла
    table* t = (table*)calloc(1, sizeof(table));
    t->ks1FN = (char*)calloc(strlen(fks1) + 1, sizeof(char));
    t->ks2FN = (char*)calloc(strlen(fks2) + 1, sizeof(char));
    strcpy(t->ks1FN, fks1);
    strcpy(t->ks2FN, fks2);
    if (msize1 < 0 || msize2 < 0){
        t->ks1 = ks1_Pull(fks1, -1, &t->csize1, &t->msize1);
        t->ks2 = ks2_Pull(fks2, -1, &t->msize2);
        return t;
    }
    t->ks1 = ks1_Pull(fks1, msize1, &t->csize1, &t->msize1);
    t->ks2 = ks2_Pull(fks2, msize2, &t->msize2);
    return t;
}

void table_Push(table* t){
    ks1_Push(t->ks1FN, t->ks1, t->msize1, t->csize1);
    ks2_Push(t->ks2FN, t->ks2, t->msize2);
}

Item* table_Find(char* key1, unsigned int key2, table* t){
    KeySpace2* find2Buf = ks2_Find(key2, t->ks2, t->msize2, t->ks2FN);
    if (!find2Buf) return NULL;
    FILE *f = fopen(t->infoFN, "r+b");
    Item *item = (Item*)calloc(1, sizeof(Item));
    fseek(f, find2Buf->info, SEEK_SET);
    fread(item, sizeof(Item), 1, f);
    Node1* find1Buf = ks1_Find(key1, t->ks1, t->csize1, item->vers1, t->ks1FN);
    if (!find1Buf) return NULL;
    return item;
}

int table_Add(char* key1, unsigned int key2, char* first, char* second, table* t){
    KeySpace2* findBuf = ks2_Find(key2, t->ks2, t->msize2, t->ks2FN);
    if (findBuf) {
        free(findBuf);
        return -1;
    }
    if (t->csize1 == t->msize1) return -2;
    FILE *f = fopen(t->infoFN, "r+b");
    Item* item = (Item*)calloc(1, sizeof(Item));
    InfoType* info = (InfoType*)calloc(1, sizeof(InfoType));
    fseek(f, 0, SEEK_END);      // записываем строки
    info->first = ftell(f);
    fwrite(first, sizeof(char), strlen(first) + 1, f);
    info->second = ftell(f);
    fwrite(second, sizeof(char), strlen(second) + 1, f);
    item->info = ftell(f);
    fwrite(info, sizeof(InfoType), 1, f);
    item->key2 = key2;
    long int itemPos = ftell(f);
    ks1_Add(key1, itemPos, t->ks1, &t->csize1, t->msize1, t->ks1FN, &item->vers1, &item->pos1);
    ks2_Add(key2, itemPos, t->ks2, t->msize2, t->ks2FN);
    fwrite(item, sizeof(Item), 1, f);
    fclose(f);
    return 0;
}

int table_Delete(char* key1, unsigned int key2, table* t){
    Item* resFind = table_Find(key1, key2, t);
    if (!resFind) return -1;
    ks1_Delete(key1, t->ks1, &t->csize1, resFind->vers1, -1, t->ks1FN);
    ks2_Delete(key2, t->ks2, t->msize2, t->ks2FN);
    return 0;
}

void table_Free(table* t){
    ks1_Free(t->ks1, t->msize1, t->csize1);
    ks2_Free(t->ks2);
    if (t->ks1FN) free(t->ks1FN);
    if (t->ks2FN) free(t->ks2FN);
    free(t);
}

void table_Print(table* t){
    printf("%5s %5s %5s %5s %5s\n", "key1", "vers", "key2", "first", "second");
    KeySpace1* bufKs1 = t->ks1;
    FILE *f = fopen(t->ks1FN, "rb");
    FILE *fInf = fopen(t->infoFN, "rb");
    Item* item = (Item*)calloc(1, sizeof(Item));
    InfoType* inf = (InfoType*)calloc(1, sizeof(InfoType));
    Node1* bufNode = calloc(1, sizeof(Node1));
    for (int i = 0; i < t->csize1; i++, bufKs1++){
        char* key1 = bufKs1->key;
        fseek(f, bufKs1->node, SEEK_SET);
        fread(bufNode, sizeof(Node1), 1, f);
        while (bufNode->release >= 0){
            fseek(fInf, bufNode->info, SEEK_SET);
            fread(item, sizeof(Item), 1, fInf);
            fseek(fInf, item->info, SEEK_SET);
            fread(inf, sizeof(InfoType), 1, fInf);
            char* first = readStr(fInf, inf->first);
            char* second = readStr(fInf, inf->second);
            printf("%5s %5d %5u %5s %5s\n", key1, bufNode->release, item->key2, first, second);
            free(first);
            free(second);
            if (bufNode->release == 0) break;
            fseek(f, bufNode->next, SEEK_SET);
            fread(bufNode, sizeof(Node1), 1, f);
        }
    }
    fclose(f);
    fclose(fInf);
    free(item);
    free(inf);
    free(bufNode);
}

