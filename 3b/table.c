#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct InfoType{
    char *first, *second;
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
    InfoType *info;
}KeySpace2;


KeySpace2* ks2FileInput(char* filename, int* size, int newMode){
    if (newMode){
        FILE *f = fopen(filename, "w+b");
        if (!f) return NULL;
        fwrite(size, sizeof(int), 1, f);
        KeySpace2* buf = (KeySpace2*)calloc(*size, sizeof(KeySpace2));
        fwrite(buf, sizeof(KeySpace2), *size, f);
        fclose(f);
        return buf;
    }
    FILE* f = fopen(filename, "r+b");
    if (!f) return NULL;
    fread(size, sizeof(int), 1, f);
    KeySpace2* buf = (KeySpace2*)calloc(*size, sizeof(KeySpace2));
    fread(buf, sizeof(KeySpace2), *size, f);
    fclose(f);
    return buf;
}

void ks2FileOutput(char* filename, int size, KeySpace2* table){
    FILE *f = fopen(filename, "r+b");
    if (!f) return;
    fwrite(&size, sizeof(int), 1, f);
    fwrite(table, sizeof(KeySpace2), size, f);
}



unsigned int hashFunc(unsigned int x, int i, int size){
       return (x + i) % size;
}

KeySpace2* ks2Init(int size){
    if (size <= 0) return NULL;
    KeySpace2 *buf = (KeySpace2*)calloc(size, sizeof(KeySpace2));
    return buf;
}

int ks2Add(unsigned int key, KeySpace2* table, int size, char* first, char* second){
    int i = 0;
    unsigned int pos = hashFunc(key, i, size);
    while (table[pos].busy == 1){
        if (table[pos].busy == 1 && table[pos].key == key) return EL_ALREADYEXIST;
        pos = hashFunc(key, ++i, size);
        if (i == size) return TABLE_FULL;
    }
    KeySpace2 *buf = table + pos;
    buf->busy = 1;
    buf->key = key;
    buf->info = (InfoType*)calloc(1, sizeof(InfoType));
    buf->info->first = (char*)calloc(strlen(first) + 1, sizeof(char));
    buf->info->second = (char*)calloc(strlen(second) + 1, sizeof(char));
    strcpy(buf->info->first, first);
    strcpy(buf->info->second, second);
    return ST_OK;
}

int ks2Find(unsigned int key, KeySpace2* table, int size){
    int i = 0;
    unsigned int pos = hashFunc(key, i, size);
    while (table[pos].busy != 0){
        if (table[pos].busy == 1 && table[pos].key == key) return (int)pos;
        pos = hashFunc(key, ++i, size);
    }
    return -1;
}

int ks2Delete(unsigned int key, KeySpace2* table, int size){
    int pos = ks2Find(key, table, size);
    if (pos < 0) return EL_NOTFOUND;
    KeySpace2 *buf = table + pos;
    buf->busy = -1;
    if (buf->info){
        if (buf->info->first) free(buf->info->first);
        if (buf->info->second) free(buf->info->second);
        free(buf->info);
    }
    return ST_OK;
}

void ks2Print(KeySpace2* table, int size){
    printf("%3s %4s %4s %4s\n", "ST", "KEY", "STR1", "STR2");
    for (int i = 0; i < size; i++, table++){
        if (table->busy == 1) {
            printf("%3d %4u %4s %4s\n", table->busy, table->key, table->info->first, table->info->second);
        }
        else {
            printf("%3d %4u\n", table->busy, table->key);
        }
    }
}

void ks2Free(KeySpace2* table, int size){
    KeySpace2 *buf = table;
    for (int i = 0; i < size; i++, table++){
        if (table->busy == 1){
            if (table->info){
                if (table->info->first) free(table->info->first);
                if (table->info->second) free(table->info->second);
                free(table->info);
            }
        }
    }
    free(buf);
}


char* FNAME = "/mnt/c/Users/frunz/Desktop/c_or_c++/C/3b/tab.bin";

int main(){
    int size = 3;
    KeySpace2 *space = ks2FileInput(FNAME, &size, 0);
//    KeySpace2* space = ks2Init(size);
//    for (unsigned int i = 1; i < 14; i += 3){
//        ks2Add(i, space, size, "a", "a");
//    }
    ks2Print(space, size);
    ks2FileOutput(FNAME, size, space);
    return 0;
}




