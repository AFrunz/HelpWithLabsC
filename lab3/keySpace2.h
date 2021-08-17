#ifndef C_KEYSPACE2_H
#define C_KEYSPACE2_H

typedef struct InfoType {
    char *first, *second;
}InfoType;

typedef struct Item{
    InfoType *info;
}Item;

typedef struct KeySpace2{
    unsigned int key; /* ключ элемента */
    Item *info; /* указатель на информацию */
    struct KeySpace2 *next; /* указатель на следующий элемент */
}KeySpace2;


KeySpace2** ks2_Init(int size);

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2** ks, int max);

int ks2_Add(unsigned int key, char* first, char* second, KeySpace2** ks, int max);

int ks2_Delete(unsigned int deletedKey, KeySpace2** ks, int max);

void ks2_Free(KeySpace2** ks, int max);

void ks2_Print(KeySpace2** ks, int max);

#endif
