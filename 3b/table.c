#include "table.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



char* readStr(FILE* f, long int pos){
    fseek(f, pos, SEEK_SET);
    char* string = (char*)calloc(1, sizeof(char));
    int len = 0;
    char sym = '0';
    while (sym != '\0'){
        fread(&sym, sizeof(char), 1, f);
        len++;
        string = realloc(string, len * sizeof(char));
        string[len - 1] = sym;
    }
    string = realloc(string, (len + 1) * sizeof(char));
    string[len] = '\0';
    return string;
}

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
    fclose(f);
}

unsigned int hashFunc(unsigned int x, int i, int size){
       return (x + i) % size;
}

int ks2Add(unsigned int key, KeySpace2* table, int size, const char* first, const char* second, const char* filename){
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
    InfoType *info = (InfoType*)calloc(1, sizeof(InfoType));
    FILE *f = fopen(filename, "r+b");
    fseek(f, 0, SEEK_END);
    info->first = ftell(f);
    fwrite(first, sizeof(char), strlen(first) + 1, f);
    info->second = ftell(f);
    fwrite(second, sizeof(char), strlen(second) + 1, f);
    buf->info = ftell(f);
    fwrite(info, sizeof(InfoType), 1, f);
    fclose(f);
    free(info);
    return ST_OK;
}

int ks2Find(unsigned int key, KeySpace2* table, int size){
    int i = 0;
    unsigned int pos = hashFunc(key, i, size);
    while (table[pos].busy != 0){
        if (table[pos].busy == 1 && table[pos].key == key) return (int)pos;
        pos = hashFunc(key, ++i, size);
        if (i == size) return EL_NOTFOUND;
    }
    return EL_NOTFOUND;
}

int ks2Delete(unsigned int key, KeySpace2* table, int size){
    int pos = ks2Find(key, table, size);
    if (pos < 0) return EL_NOTFOUND;
    KeySpace2 *buf = table + pos;
    buf->busy = -1;
    return ST_OK;
}

void ks2Print(KeySpace2* table, int size, const char* filename){
    FILE *f = fopen(filename, "r+b");
    printf("%3s %4s %4s %4s\n", "ST", "KEY", "STR1", "STR2");
    InfoType *inf = (InfoType*)calloc(1, sizeof(InfoType));
    for (int i = 0; i < size; i++, table++){
        if (table->busy == 1) {
            fseek(f, table->info, SEEK_SET);
            fread(inf, sizeof(InfoType), 1, f);
            char* first = readStr(f, inf->first);
            char* second = readStr(f, inf->second);
            printf("%3d %4u %4s %4s\n", table->busy, table->key, first, second);
            free(first);
            free(second);
        }
        else {
            printf("%3d %4u\n", table->busy, table->key);
        }
    }
    free(inf);
    fclose(f);
}

void ks2Free(KeySpace2* table){
    free(table);
}


//int main(){
//    int size = 3;
//    KeySpace2 *space = ks2FileInput(FNAME, &size, 0);
////    KeySpace2* space = ks2Init(size);
////    for (unsigned int i = 1; i < 14; i += 3){
////        ks2Add(i, space, size, "a", "a", FNAME);
////    }
//    ks2Print(space, size, FNAME);
//    ks2FileOutput(FNAME, size, space);
//    return 0;
//}




