#ifndef C_KEYSPACE1_H
#define C_KEYSPACE1_H

typedef struct InfoType {
    char *first, *second;
}InfoType;

typedef struct Item{
    InfoType *info;
}Item;

typedef struct Node1{
    int release; /* номер версии */
    Item *info; /* указатель на информацию */
    struct Node1 *next; /* указатель на следующий элемент */
}Node1;

typedef struct KeySpace1{
    char* key; /* ключ элемента */
    Node1 *node; /* указатель на информацию */
}KeySpace1;


KeySpace1* ks1_Init(int size);

int ks1_FindIndex(char* requiredKey, KeySpace1* ks, int lvl);

int ks1_Add(char* key, char* first, char* second, KeySpace1* ks, int* lvl, int max);

KeySpace1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version);

int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version);

void ks1_Free(KeySpace1* ks, int lvl);

#endif //C_KEYSPACE1_H
