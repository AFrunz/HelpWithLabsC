#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

char *getString() {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = (int)strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }

    return res;
}


int main(){

//    ПУТЬ ПО УМОЛЧАНИЮ
    char* FNAME = "/mnt/c/Users/frunz/Desktop/c_or_c++/C/3b/tab.bin";


    char* msg[] = {"0.Exit", "1.Add", "2.Delete", "3.Find", "4.Print"};
    const int msgSize = sizeof(msg) / sizeof(char*);

    // РАБОТА С ФАЙЛОМ
    int defaultFile;
    printf("Use default file path? 1-Yes, 0-No\n");
    scanf("%d%*c", &defaultFile);
    if (!defaultFile){
        printf("Enter file path\n");
        FNAME = getString();
    }
    int continueStatus;
    printf("Continue? 1-Yes, 0-No\n");
    scanf("%d", &continueStatus);
    int size = 0;
    if (!continueStatus){
        printf("Enter table size\n");
        scanf("%d", &size);
    }


    KeySpace2 *ks = ks2FileInput(FNAME, &size, !continueStatus);

    // РАБОТА С ДИАЛОГОМ
    unsigned int key;
    int status;

    int n = -1;
    while (n != 0){
        //  ДОБАВЛЕНИЕ
        if (n == 1){
            printf("Enter key (uint)\n");
            scanf("%u%*c", &key);
            printf("Enter first string\n");
            char* first = getString();
            printf("Enter second string\n");
            char* second = getString();
            status = ks2Add(key, ks, size, first, second, FNAME);
            if (status == ST_OK) printf("Success\n");
            else if (status == TABLE_FULL) printf("Table is full\n");
            else if (status == EL_ALREADYEXIST) printf("Element already exist\n");
            free(first);
            free(second);
        }
        // УДАЛЕНИЕ
        else if (n == 2){
            printf("Enter key (uint)\n");
            scanf("%ud%*c", &key);
            status = ks2Delete(key, ks, size);
            if (status == ST_OK) printf("Success\n");
            if (status == EL_NOTFOUND) printf("Element not found\n");
        }
        // ПОИСК
        else if (n == 3){
            printf("Enter key (uint)\n");
            scanf("%ud%*c", &key);
            int pos = ks2Find(key, ks, size);
            if (pos != EL_NOTFOUND) ks2Print(ks + pos, 1, FNAME);
            else printf("Element not found\n");
        }
        // ВЫВОД
        else if (n == 4){
            ks2Print(ks, size, FNAME);
        }
        for (int i = 0; i < msgSize; i++){
            printf("%s\n", msg[i]);
        }
        scanf("%d",&n);
    }

    ks2FileOutput(FNAME, size, ks);
    ks2Free(ks);
    if (!defaultFile) free(FNAME);
    return 0;
}
