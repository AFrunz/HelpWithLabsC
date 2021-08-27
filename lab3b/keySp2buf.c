//#include "keySpace2.h"
#include <stdio.h>
#include <stdlib.h>
#define ks1file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks1.bin"
#define ks2file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks2.bin"
#define infofile "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\info.bin"



typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4
}ans;



typedef struct InfoType {
    char *first, *second;
}InfoType;

typedef struct Item{
    InfoType *info;
//    KeySpace1* ptr1;     // указатель на эл-т 1го пр-ва
    int index;      // версия этого эл-та
    unsigned int key2; // ключ из 2-го пр-ва
}Item;

typedef struct KeySpace2{
    int status;         // заполненость эл-та (0 - эл-т пустой, 1 - эл-т заполнен)
    unsigned int key; /* ключ элемента */
    long int info;       /* указатель на информацию  Item*  */
    long int next; /* указатель на следующий элемент struct KeySpace2*  */
}KeySpace2;

KeySpace2* ks2_Pull(char* ks2FileName, int msize2){
//    Загрузка таблицы из файла
    FILE *f = fopen(ks2FileName, "r+b");
    if (msize2 != -1) {
        KeySpace2 *space = (KeySpace2*)calloc(msize2, sizeof(KeySpace2));
        fwrite(&msize2, sizeof(int), 1, f);
        fclose(f);
        return space;
    }
    int n;
    fread(&n, sizeof(int), 1, f);
    KeySpace2* space = (KeySpace2*)calloc(n, sizeof(KeySpace2));
    fread(space, sizeof(KeySpace2), n, f);
    fclose(f);
    return space;
}


void ks2_Push(char* Ks2FileName, KeySpace2* ks, int n){
//    Загрузка таблицы в файл
    FILE *f = fopen(Ks2FileName, "r+b");
    fwrite(&n, 1, sizeof(int), f);
    fwrite(ks, n, sizeof(KeySpace2), f);
    fclose(f);
}


int hash_f(unsigned int key, int max){
//    Хэш-функция
    return ((int)key % max);
}

KeySpace2* ks2_Find(unsigned int requiredKey, KeySpace2* ks, int max, char* Ks2FileName){
//    Поиск по ks2
//    Входные данные: искомый ключ, ks, размер таблицы
//    Выходные данные: указатель на эл-т
    int pos = hash_f(requiredKey, max);
    if (!(ks + pos)->status) return NULL;
    KeySpace2* buf = ks + pos;
    FILE *f = fopen(Ks2FileName, "r+b");
    while (buf){
        if (buf->key == requiredKey){
            fclose(f);
            return buf;
        }
        if (buf->next == 0) {
            fclose(f);
            return NULL;
        }
        fsetpos(f, &buf->next);
        fread(&buf, 1, sizeof(KeySpace2), f);
    }
    fclose(f);
    return NULL;
}

KeySpace2* getKS(long int item, unsigned int key, char* Ks2FileName){
//    Создание элемента ks и запись в него данных
//    Входные данные: данные, ключ
//    Выходные данные: указатель на эл-т ks2
    FILE *f = fopen(Ks2FileName, "r+b");
    KeySpace2* bufKS = (KeySpace2*)calloc(1, sizeof(KeySpace2));
    bufKS->info = item;
//    bufKS->info->key2 = key;
    bufKS->key = key;
    bufKS->status = 1;
    fclose(f);
    return bufKS;
}

int ks2_Add(unsigned int key, long int item, KeySpace2* ks, int max, char* Ks2FileName){
//    Добавление элемента
//    Входные данные: ключ, данные, ks, размер таблицы
//    Выходные данные: код ошибки
    if (ks2_Find(key, ks, max, Ks2FileName)) return KEY_AE;
    int pos = hash_f(key, max);
    FILE *f = fopen(Ks2FileName, "r+b");

    if (!(ks + pos)->status){
        (ks + pos)->key = key;
        (ks + pos)->status = 1;
        (ks + pos)->info = item;
        fclose(f);
        return ST_OK;
    }
    KeySpace2* bufKS = getKS(item, key, Ks2FileName);
    fseek(f, 0, SEEK_END);
    long int nextPos;
    fgetpos(f, &nextPos);
    fwrite(bufKS, sizeof(KeySpace2), 1, f);
    bufKS->next = (ks + pos)->next;
    (ks + pos)->next = nextPos;
    fclose(f);
    return ST_OK;
}


int ks2_Delete(unsigned int deletedKey, KeySpace2* ks, int max, int flag, char* Ks2FileName){
//    Удаление эл-та ks2
//    Входные данные: ключ, ks, размер таблицы, флаг(0 - очищать данные, 1 - нет)
//    Выходные данные: код ошибки
    KeySpace2* element = ks2_Find(deletedKey, ks, max, Ks2FileName);
    if (!element) return EL_NOTFOUND;
    int pos = hash_f(deletedKey, max);
    KeySpace2* buf = ks + pos;
    FILE *f = fopen(Ks2FileName, "r+b");
    if (buf->key == deletedKey){            // Если удаляемый эл-т - первый в списке
        if (!buf->next){
            buf->status = 0;
            //freeElement
            fclose(f);
            return ST_OK;
        }
        KeySpace2 *nextEl = (KeySpace2*)calloc(1, sizeof(KeySpace2));
        fsetpos(f, &buf->next);
        fread(nextEl, sizeof(KeySpace2), 1, f);
        buf->next = nextEl->next;
        buf->key = nextEl->key;
        buf->info = nextEl->info;
        fclose(f);
        return ST_OK;
    }
    KeySpace2* par = NULL;
    while (1) {                           // Поиск эл-та по списку
        if (buf->key == deletedKey){
            par->next = buf->next;
            break;
        }
        if (!buf->next) return EL_NOTFOUND;
        par = buf;
        fsetpos(f, &buf->next);
        fread(buf, sizeof(KeySpace2), 1, f);
    }
    fclose(f);
//    freeKS(buf, flag);
    return ST_OK;
}




void ks2_Print(char* fileName){
    FILE *f = fopen(fileName, "r+b");
    int n;
    fread(&n, sizeof(int), 1, f);
    KeySpace2 *ks = calloc(n, sizeof(KeySpace2));
    fread(ks, sizeof(KeySpace2), n, f);
    for (int i = 0; i < n; i++, ks++){
        printf("%d %u", ks->status, ks->key);
        KeySpace2 *buf = ks;
        while (buf->next){
            fsetpos(f, &buf->next);
            fread(buf, sizeof(KeySpace2), 1, f);
            printf("-> %d %u", buf->status, buf->key);
        }
        printf("\n");
    }
    fclose(f);
}


int main(){
    int msize2 = 15;
//    ks2_Print(ks2file);
    KeySpace2* ks2 = ks2_Pull(ks2file, -1);
    ks2_Delete(24, ks2, msize2, 0, ks2file);
//    ks2_Add(9, 150, ks2, msize2, ks2file);
    ks2_Push(ks2file,ks2, msize2);
    ks2_Print(ks2file);
    return 0;
}



