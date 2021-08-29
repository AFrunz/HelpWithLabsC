#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#define size1 10
#define size2 10
//#define ks1file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks1.bin"
//#define ks2file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks2.bin"
//#define infofile "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\info.bin"
#define ks1file "/student/b20503/b2050326/help/lab3b/ks1.bin"
#define ks2file "/student/b20503/b2050326/help/lab3b/ks2.bin"
#define infofile "/student/b20503/b2050326/help/lab3b/info.bin"






char* getString(){
    int n, len, len_res = 0;
    char buf[51];
    char *res = NULL;
    do{
        n = scanf("%50[^'\n']", buf);
        if (n < 0){
            if (!res) return NULL;
        }
        else if (n > 0) {
            len = (int)strlen(buf);
            len_res += len;
            res = realloc(res, (len_res + 1) * sizeof(char));
            memcpy(res + len_res - len, buf, len);
        }
        else{
            scanf("%*c");
        }
    } while (n > 0);
    if (len_res > 0){
        res[len_res] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

int main(){

    char *var[] = {"0. Exit", "1. Add(key1, key2)", "2. Find(key1, key2)", "3. Delete(Key1, Key2)", "4. TableFind(Key1)",
                   "5. TableFind(Key2)", "6. Table Delete(Key1)", "7. Table Delete(Key2)", "8. Print"};
    int sizeVar = sizeof(var) / sizeof(char*);
    int choice = -1;

    printf("Use base fileUrl?\n1-Yes, 0-No\n");
    int fileChoice;
    table *t = NULL;
    scanf("%d", &fileChoice);
    if (fileChoice){
        int newChoice;
        printf("Continue?\n1-Yes, 0-No\n");
        scanf("%d%*c", &newChoice);
        if (newChoice){
            t = table_Pull(ks1file, ks2file, -1, -1);
            t->infoFN = infofile;
        }
        else {
            t = table_Pull(ks1file, ks2file, size1, size2);
            t->infoFN = infofile;
        }
    }
    else return 0;
    int status;

    while (choice != 0){
        if (choice == 1) {
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter key2 (u int)\n");
            unsigned int key2;
            scanf("%u%*c", &key2);
            printf("Enter first str\n");
            char* s1 = getString();
            printf("Enter second str\n");
            char* s2 = getString();
            status = table_Add(key1, key2, s1, s2, t);
            if (status == -1) printf("Error, key2 already exist\n");
            if (status == -2) printf("Error, keyspace1 is full\n");
            if (status == 0) printf("Success\n");
            free(s1);
            free(s2);
            free(key1);
        }
        else if (choice == 2){
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter key2 (u int)\n");
            unsigned int key2;
            scanf("%u", &key2);
            Item* buf = table_Find(key1, key2, t);
            if (!buf) printf("Element not found\n");
            else {
                FILE *f = fopen(t->infoFN, "r+b");
                fseek(f, buf->info, SEEK_SET);
                InfoType* info = (InfoType*)calloc(1, sizeof(InfoType));
                fread(info, sizeof(InfoType), 1, f);
                char* first = readStr(f, info->first);
                char* second = readStr(f, info->second);
                printf("%s %s\n", first, second);
                free(buf);
                free(first);
                free(second);
                fclose(f);
            }
            free(key1);
        }
        else if (choice == 3){
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter key2 (u int)\n");
            unsigned int key2;
            scanf("%u", &key2);
            status = table_Delete(key1, key2, t);
            if (status == -1) printf("Element not found\n");
            else printf("Success\n");
            free(key1);
        }
        else if (choice == 4){
            //TableFind(Key1)
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter version (int)\n");
            int vers;
            scanf("%d", &vers);
            Node1* res = ks1_Find(key1, t->ks1, t->csize1, vers, t->ks1FN);
            if (!res) printf("Element not found\n");
            else {
                FILE *f = fopen(t->infoFN, "rb");
                fseek(f, res->info, SEEK_SET);
                Item* item = (Item*)calloc(1, sizeof(Item));
                fread(item, sizeof(Item), 1, f);
                fseek(f, item->info, SEEK_SET);
                InfoType* info = (InfoType*)calloc(1, sizeof(InfoType));
                fread(info, sizeof(InfoType), 1, f);
                char* first = readStr(f, info->first);
                char* second = readStr(f, info->second);
                printf("%5s %5s %5s %5s %5s\n", "key1", "vers", "key2", "first", "second");
                printf("%5s %5d %5u %5s %5s\n", key1, vers, item->key2, first, second);
                fclose(f);
                free(res);
                free(item);
                free(info);
                free(first);
                free(second);
                }
            free(key1);
            }
        else if (choice == 5){
            //TableFind(Key2)
            printf("Enter key2 (u int)\n");
            unsigned int key2;
            scanf("%u", &key2);
            KeySpace2 *res = ks2_Find(key2, t->ks2, t->msize2, t->ks2FN);
            if (!res) printf("Element not found\n");
            else {
                Item* item = (Item*)calloc(1, sizeof(Item));
                FILE *f = fopen(t->infoFN, "rb");
                fseek(f, res->info, SEEK_SET);
                fread(item, sizeof(Item), 1, f);
                fseek(f, item->info, SEEK_SET);
                InfoType* info = (InfoType*)calloc(1, sizeof(InfoType));
                fread(info, sizeof(InfoType), 1, f);
                char* first = readStr(f, info->first);
                char* second = readStr(f, info->second);
                printf("%5s %5s %5s %5s %5s\n", "key1", "vers", "key2", "first", "second");
                printf("%5s %5d %5u %5s %5s\n", t->ks1[item->pos1].key, item->vers1, key2, first, second);
                free(res);
                free(item);
                free(info);
                free(first);
                free(second);
            }
        }
        else if (choice == 6){
            //Table Delete(Key1)
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter version (int)\n");
            int vers;
            scanf("%d", &vers);
            Node1* nodeBuf = ks1_Find(key1, t->ks1, t->csize1, vers, t->ks1FN);
            if (nodeBuf){
                FILE *f = fopen(t->infoFN, "rb");
                fseek(f, nodeBuf->info, SEEK_SET);
                Item* item = (Item*)calloc(1, sizeof(Item));
                fread(item, sizeof(Item), 1, f);
                ks2_Delete(item->key2, t->ks2, t->msize2, t->ks2FN);
                ks1_Delete(key1, t->ks1, &(t->csize1), vers, -1, t->ks1FN);
                printf("Success\n");
                fclose(f);
                free(item);
                free(nodeBuf);
            }
            else {
                printf("Key not found\n");
            }
            free(key1);
        }
        else if (choice == 7){
            // Table Delete(Key2)
            printf("Enter key2 (u int)\n");
            unsigned int key2;
            scanf("%u", &key2);
            KeySpace2* res = ks2_Find(key2, t->ks2, t->msize2, t->ks2FN);
            if (!res) printf("Element not found\n");
            else {
                Item* item = (Item*)calloc(1, sizeof(Item));
                FILE *f = fopen(t->infoFN, "rb");
                fseek(f, res->info, SEEK_SET);
                fread(item, sizeof(Item), 1, f);
                status = ks1_Delete(NULL, t->ks1, &t->csize1, item->vers1, item->pos1, t->ks1FN);
                ks2_Delete(key2, t->ks2, t->msize2, t->ks2FN);
                printf("Success\n");
                fclose(f);
                free(res);
                free(item);
            }

        }
        else if (choice == 8){
            table_Print(t);
        }

        for (int i = 0; i < sizeVar; i++) {
            printf("%s\n", var[i]);
        }
        scanf("%d%*c", &choice);
        }
    table_Push(t);
    table_Free(t);
    return 0;
}