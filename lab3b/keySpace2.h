#include <stdio.h>
#ifndef C_KEYSPACE2_H
#define C_KEYSPACE2_H

//#ifndef C_KEYSPACE1_H
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
typedef struct KeySpace2{
    int status;         // заполненость эл-та (0 - эл-т пустой, 1 - эл-т заполнен)
    unsigned int key; /* ключ элемента */
    long int info;       /* указатель на информацию  Item*  */
    long int next; /* указатель на следующий элемент struct KeySpace2*  */
}KeySpace2;

void ks2_Push(char* Ks2FileName, KeySpace2* ks, int n);

KeySpace2* ks2_Pull(char* ks2FileName, int newSize2, int* msize2);

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2* ks, int max, char* Ks2FileName);

int ks2_Add(unsigned int key, long int item, KeySpace2* ks, int max, char* Ks2FileName);

int ks2_Delete(unsigned int deletedKey, KeySpace2* ks, int max, int flag, char* Ks2FileName);

void ks2_Free(KeySpace2* ks);

#endif
