#ifndef C_TABLE_H
#define C_TABLE_H
#include "keySpace1.h"
#include "keySpace2.h"

typedef struct Table {
    KeySpace1 *ks1;         /* указатель на первое пространство ключей */
    KeySpace2 **ks2;
    int csize1;
    int msize1;
    int msize2;
}Table;

typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4,
    KEY2_AE = 5
}ans;

Table* table_Init(int size1, int size2);

int table_Add(char* key1, unsigned int key2, Table* t, char* first, char* second);

Item* table_Find(char* key1, unsigned int key2, Table* t);

int table_Delete(char* key1, unsigned int key2, Table* t);

#endif
