#ifndef C_KEYSPACE1_H
#define C_KEYSPACE1_H
//#ifndef C_KEYSPACE2_H
//typedef struct InfoType {
//    char *first, *second;
//}InfoType;
//
//typedef struct KeySpace1 KeySpace1;
//
//typedef struct Item{
//    InfoType *info;
//    KeySpace1* ptr1;     // указатель на эл-т 1го пр-ва
//    int index;      // версия этого эл-та
//    unsigned int key2; // ключ из 2-го пр-ва
//}Item;
//
//#endif

typedef struct Node1{
    int release; /* номер версии */
    long int info; /* указатель на информацию Item* */
    long int next; /* указатель на следующий элемент struct Node1 */
}Node1;

typedef struct KeySpace1{
    long int keyPos; // Позиция ключа
    char* key; /* ключ элемента */
    long int node; /* указатель на информацию Node1*  */
}KeySpace1;

int ks1_FindIndex(char* requiredKey, KeySpace1* ks, int lvl);

Node1* ks1_Find(char* requiredKey, KeySpace1* ks, int lvl, int version, char* Ks1FileName);

void ks1_Push(char* Ks1FileName, KeySpace1* ks, int msize1, int lvl);

KeySpace1* ks1_Pull(char* ks1FileName, int newSize1, int* lvl, int* msize1);

int ks1_Add(char* key, long int item, KeySpace1* ks, int* lvl, int max, char* Ks1FileName, int* index, int *ks1Pos);

int ks1_Delete(char* deletedKey, KeySpace1* ks, int *lvl, int version, int base, char* Ks1FileName);

void ks1_Free(KeySpace1* ks, int lvl);



#endif //C_KEYSPACE1_H
