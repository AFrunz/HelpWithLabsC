#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#define size1 10
#define size2 10
#define ks1file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks1.bin"
#define ks2file "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\ks2.bin"
#define infofile "C:\\Users\\frunz\\Desktop\\c_or_c++\\C\\lab3b\\info.bin"


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

    char *var[] = {"0. Exit", "1. Add(key1, key2)", "2. !Find(key1, key2)", "3. Delete(Key1, Key2)", "!4. TableFind(Key1)",
                   "!5. TableFind(Key2)", "6. Table Delete(Key1)", "!7. Table Delete(Key2)", "8. Print"};
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
        }
        else {
            t = table_Pull(ks1file, ks2file, size1, size2);
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
//        else if (choice == 2){
//            printf("Enter key1 (str)\n");
//            char* key1 = getString();
//            printf("Enter key2 (u int)\n");
//            unsigned int key2;
//            scanf("%u", &key2);
//            Item* buf = table_Find(key1, key2, t);
//            if (!buf) printf("Element not found\n");
//            else {
////                printf("%s %s\n", buf->info->first, buf->info->second);
//                free(buf);
//            }
//            free(key1);
//        }
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
//        else if (choice == 4){
//            //TableFind(Key1)
//            printf("Enter key1 (str)\n");
//            char* key1 = getString();
//            printf("Enter version or -1 (int)\n");
//            int vers;
//            scanf("%d", &vers);
//            KeySpace1* res = ks1_Find(key1, t->ks1, t->csize1, vers);
//            if (!res) printf("Element not found\n");
//            else {
//                Node1* bufNode = res->node;
//                Node1* delNode = NULL;
//                while (bufNode){
//                    delNode = bufNode;
//                    printf("%s %u %s %s\n", bufNode->info->ptr1->key, bufNode->info->key2,
//                           bufNode->info->info->first, bufNode->info->info->second);
//                    bufNode = bufNode->next;
//                    free(delNode);
//                }
//                free(res->key);
//                free(res);
//            }
//        }
//        else if (choice == 5){
//            //TableFind(Key2)
//            printf("Enter key2 (u int)\n");
//            unsigned int key2;
//            scanf("%u", &key2);
//            KeySpace2 *res = ks2_Find(key2, t->ks2, t->msize2, t->ks2FN);
//            if (!res) printf("Element not found\n");
//            else printf("%s %u %s %s\n",res->info->ptr1->key, res->key, res->info->info->first, res->info->info->second);
//        }
        else if (choice == 6){
            //Table Delete(Key1)
            printf("Enter key1 (str)\n");
            char* key1 = getString();
            printf("Enter version (int)\n");
            int vers;
            scanf("%d", &vers);
            status = ks1_Delete(key1, t->ks1, &(t->csize1), vers, NULL, t->ks1FN);
            if (status == EL_NOTFOUND) printf("Key not found\n");
            if (status == VERS_NOTFOUND) printf("Key with this version not found\n");
            if (status == ST_OK) printf("Success\n");
            free(key1);
        }
//        else if (choice == 7){
//            // Table Delete(Key2)
//            printf("Enter key2 (u int)\n");
//            unsigned int key2;
//            scanf("%u", &key2);
//            KeySpace2* res = ks2_Find(key2, t->ks2, t->msize2);
//            if (!res) printf("Element not found\n");
//            else {
//                status = ks1_Delete(res->info->ptr1->key, t->ks1, &(t->csize1), res->info->index, res->info->ptr1, t);
//                if (status == KEY_NF) printf("Key not found\n");
//                if (status == VERS_NOTFOUND) printf("Key with this version not found\n");
//                if (status == ST_OK) printf("Success\n");
//            }
//
//        }
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