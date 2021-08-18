#ifndef C_KEYSPACE2_H
#define C_KEYSPACE2_H

#ifndef C_KEYSPACE1_H
typedef struct InfoType {
    char *first, *second;
}InfoType;

typedef struct KeySpace1 KeySpace1;

typedef struct Item{
    InfoType *info;
    KeySpace1* ptr1;     // указатель на эл-т 1го пр-ва
    int index;      // версия этого эл-та
    unsigned int key2; // ключ из 2-го пр-ва
}Item;

#endif




typedef struct KeySpace2{
    unsigned int key; /* ключ элемента */
    Item *info; /* указатель на информацию */
    struct KeySpace2 *next; /* указатель на следующий элемент */
}KeySpace2;


KeySpace2** ks2_Init(int size);

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2** ks, int max);

int ks2_Add(unsigned int key, Item* item, KeySpace2** ks, int max);

int ks2_Delete(unsigned int deletedKey, KeySpace2** ks, int max, int flag);

void ks2_Free(KeySpace2** ks, int max);

void ks2_Print(KeySpace2** ks, int max);

void freeKS(KeySpace2* el, int flag);

#endif
