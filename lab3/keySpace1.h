#ifndef C_KEYSPACE1_H
#define C_KEYSPACE1_H
#ifndef C_KEYSPACE2_H
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


typedef struct Node1{
    int release; /* номер версии */
    Item *info; /* указатель на информацию */
    struct Node1 *next; /* указатель на следующий элемент */
}Node1;

struct KeySpace1{
    char* key; /* ключ элемента */
    Node1 *node; /* указатель на информацию */
};


int str_compare(char* s1, char* s2);

KeySpace1* ks1_Init(int size);

int ks1_FindIndex(char* requiredKey, KeySpace1* ks, int lvl);

int ks1_Add(char* key, Item* item, KeySpace1* ks, int* lvl, int max);

KeySpace1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version);

//int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version, KeySpace1* base, Table* t);

void ks1_Free(KeySpace1* ks, int lvl);

void ks1_Print(KeySpace1* ks, int lvl);

void nodeFree(Node1* el);

#endif //C_KEYSPACE1_H
