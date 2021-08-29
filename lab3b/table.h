#ifndef C_TABLE_H
#define C_TABLE_H
#include "keySpace1.h"
#include "keySpace2.h"

typedef struct InfoType {
    long int first, second; // char*
}InfoType;

typedef struct Item{
    long int info;         // InfoType
    int pos1;     // индекс в массиве эл-та 1го пр-ва
    int vers1;      // версия этого эл-та
    unsigned int key2; // ключ из 2-го пр-ва
}Item;

typedef struct table{
    char* ks1FN;
    char* ks2FN;
    char* infoFN;
    KeySpace1 *ks1;
    KeySpace2 *ks2;
    int csize1;
    int msize1;
    int msize2;
}table;

typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4,
    KEY2_AE = 5
}ans;

table* table_Pull(char* fks1, char* fks2, int msize1, int msize2);

void table_Push(table* t);

Item* table_Find(char* key1, unsigned int key2, table* t);

int table_Add(char* key1, unsigned int key2, char* first, char* second, table* t);

int table_Delete(char* key1, unsigned int key2, table* t);

void table_Free(table* t);

void table_Print(table* t);

#endif
