#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ks1file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks1.bin"
#define ks2file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks2.bin"
#define infofile "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\info.bin"
#define N 100 // Максимальный размер ключа


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

typedef struct Node1{
    int release; /* номер версии */
    fpos_t info; /* указатель на информацию Item* */
    fpos_t next; /* указатель на следующий элемент struct Node1 */
}Node1;

struct KeySpace1{
    fpos_t keyPos; // Позиция ключа
    char* key; /* ключ элемента */
    fpos_t node; /* указатель на информацию Node1*  */
};

typedef enum ans {
    KEY_NF = -1,
    TABLE_FULL = 1,
    ST_OK = 0,
    EL_NOTFOUND = 2,
    VERS_NOTFOUND = 3,
    KEY_AE = 4
}ans;

int str_compare(char* s1, char* s2){
// Сравнение строк
//    Возвращает 1, s1 > s2
//               0, s1 = s2
//              -1, s1 < s2
    if ((s1 == NULL) && (s2 == NULL)) return 0;
    if ((s1 == NULL) && (s2 != NULL)) return -1;
    if ((s1 != NULL) && (s2 == NULL)) return 1;
    while ((*s1 != '\0') && (*s2 != '\0') && (*s1 == *s2)){
        s1++;
        s2++;
    }
    if ((*s1 == '\0') && (*s2 == '\0')) return 0;
    if (*s1 > *s2) return 1;
    return -1;
}

char* freadStr(fpos_t pos, FILE *f){
    if (pos == -1) return NULL;
    char* string = (char*)calloc(1, sizeof(char));
    char symbol = '1';
    int len = 0;
    fsetpos(f, &pos);
    while (symbol != '\0'){
        fread(&symbol, sizeof(char), 1, f);
        len++;
        string = (char*)realloc(string, len * sizeof(char));
        string[len - 1] = symbol;
    }
    return string;
}

KeySpace1* ks1_Pull(char* ks1FileName, int msize1, int* lvl){
    FILE *f = fopen(ks1FileName, "r+");
    if (msize1 != -1) {
        KeySpace1 *space = (KeySpace1*)calloc(msize1, sizeof(KeySpace1));
        for (int i = 0; i < msize1; i++){
            space[i].keyPos = -1;
        }
        fwrite(&msize1, sizeof(int), 1, f);
        fclose(f);
        *lvl = 0;
        return space;
    }
    int n;
    fread(&n, sizeof(int), 1, f);
    fread(lvl, sizeof(int), 1, f);
    KeySpace1* space = (KeySpace1*)calloc(n, sizeof(KeySpace1));
    fread(space, sizeof(KeySpace1), n, f);
    KeySpace1* buf = space;
    for (int i = 0; i < n; i++, buf++){
        buf->key = freadStr(buf->keyPos, f);
    }
    fclose(f);
    return space;
}

void ks1_Push(char* Ks1FileName, KeySpace1* ks, int msize1, int lvl){
//    Загрузка таблицы в файл
    FILE *f = fopen(Ks1FileName, "r+");
    fwrite(&msize1, 1, sizeof(int), f);
    fwrite(&lvl, 1, sizeof(int), f);
    fwrite(ks, msize1, sizeof(KeySpace1), f);
    fclose(f);
}

int ks1_FindIndex(char* requiredKey, KeySpace1* ks, int lvl){
//    Поиск в таблице по ключу (бинарный поиск)
//    Входные данные: Искомый ключ, ks1, количество эл-в
//    Выходные данные: Позиция эл-та или -1
    int up = lvl - 1, low = 0, i;
    while (up >= low){
        i = (up + low) / 2;
        int compare = str_compare(requiredKey, ks[i].key);
        if (compare  == 1) {
            low = i + 1;
        }
        else if (compare == -1){
            up = i - 1;
        }
        else {
            return i;
        }
    }
    return KEY_NF;
}

Node1* getNode(fpos_t item){
//    Создание объекта Node и запись в него данных
//    Входные данные: указатель на эл-т Item
//    Выходные данные: указатель на эл-т Node1
    Node1 *bufNode = (Node1*)calloc(1, sizeof(Node1));
    bufNode->info = item;
    return bufNode;
}

int ks1_Add(char* key, fpos_t item, KeySpace1* ks, int* lvl, int max, char* Ks1FileName){
//    Добавление элемента в таблицу
//    Входные данные: Ключ, информация, ks, количество эл-в и размер таблицы
//    Выходные данные: 0 - ок, 1 - таблица заполнена

    if (*lvl == max) return TABLE_FULL;                         // Проверка на заполненость таблицы
    Node1* bufNode = getNode(item);
    int pos = ks1_FindIndex(key, ks, *lvl);
    FILE *f = fopen(Ks1FileName, "r+");
    if (pos != -1) {                                            // Если ключ уже есть в таблице
        bufNode->next = ks[pos].node;
        Node1* fileNode = (Node1*)calloc(1, sizeof(Node1));
        fsetpos(f, &ks[pos].node);
        fread(fileNode, sizeof(Node1), 1, f);
        bufNode->release = fileNode->release + 1;
//        bufNode->info->index = bufNode->release;
//        bufNode->info->ptr1 = ks + pos;
        fseek(f, 0, SEEK_END);
        fpos_t bufPos;
        fgetpos(f, &bufPos);
        fwrite(bufNode, sizeof(Node1), 1, f);
        ks[pos].node = bufPos;
        fclose(f);
        return ST_OK;
    }
    KeySpace1 *buf = ks;
    int i = *lvl - 1;
    while (i >= 0 && str_compare(buf[i].key, key) > 0){        // Поиск места для вставки
        buf[i + 1] = buf[i];
        i--;
    }
    buf[i + 1].key = (char*)calloc(N, sizeof(char));
    fseek(f, 0, SEEK_END);
    fpos_t keyPos;
    fgetpos(f, &keyPos);
    fwrite(key, sizeof(char), strlen(key) + 1, f);
    strcpy(buf[i + 1].key, key);                                      // Вставка
    buf[i + 1].keyPos = keyPos;
//    bufNode->info->index = 0;
//    bufNode->info->ptr1 = buf + i + 1;
    fseek(f, 0, SEEK_END);
    fpos_t bufPos;
    fgetpos(f, &bufPos);
    fwrite(bufNode, sizeof(Node1), 1, f);
    buf[i + 1].node = bufPos;
    (*lvl)++;
    fclose(f);
    return ST_OK;
}

void ks1_Print(char* fileName){
    FILE *f = fopen(fileName, "r+");
    int n;
    int lvl;
    fread(&n, sizeof(int), 1, f);
    fread(&lvl, sizeof(int), 1, f);
    KeySpace1 *space = (KeySpace1*)calloc(1, sizeof(KeySpace1));
    fread(space, sizeof(KeySpace1), n, f);
    for (int i = 0; i < n; i++, space++){
        space->key = freadStr(space->keyPos, f);
        if (space->key) printf("%s\n",space->key);
        else printf("NULL\n");
    }
    fclose(f);
}

int main(){
    int msize1 = 15;
    int lvl = 0;
//    ks1_Print(ks1file);
//    KeySpace1* ks1 = ks1_Pull(ks1file, msize1, &lvl);
//    ks1_Add("a", 123, ks1, &lvl, msize1, ks1file);
//    ks1_Push(ks1file, ks1, msize1, lvl);
    ks1_Print(ks1file);
    return 0;
}