#ifndef C_TABLE_H
#define C_TABLE_H

typedef struct InfoType{
    long int first, second;         //char*
}InfoType;

typedef enum ans{
    ST_OK = 0,
    EL_NOTFOUND = -1,
    EL_ALREADYEXIST = -2,
    TABLE_FULL
}ans;

typedef struct KeySpace2{
    short int busy;
    unsigned int key;
    long int info;              //InfoType*
}KeySpace2;

KeySpace2* ks2FileInput(char* filename, int* size, int newMode);

void ks2FileOutput(char* filename, int size, KeySpace2* table);

unsigned int hashFunc(unsigned int x, int i, int size);

int ks2Add(unsigned int key, KeySpace2* table, int size, const char* first, const char* second, const char* filename);

int ks2Find(unsigned int key, KeySpace2* table, int size);

int ks2Delete(unsigned int key, KeySpace2* table, int size);

void ks2Print(KeySpace2* table, int size, const char* filename);

void ks2Free(KeySpace2* table);

#endif //C_TABLE_H
