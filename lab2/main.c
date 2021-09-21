#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"


char* readStr(int* flag){
    char* string = (char*)calloc(1, sizeof(char));
    int len = 0;
    char sym = '1';
    int st = 1;
    while (sym != '\n'){
        scanf("%c", &sym);
        if (sym == '/') break;
        if (st && (sym == '*')){
            *flag = 1;
            st = 0;
        }
        else if (sym == '\n'){
                free(string);
                return NULL;
        }
        else{
            len++;
            string = realloc(string, len * sizeof(char));
            string[len - 1] = sym;
        }
    }
    string = realloc(string, (len + 1) * sizeof(char));
    string[len] = '\0';
    return string;
}


void getInfo(Queue* q){

    int n;
    int ta, ts;
    char stop = '1';
    do {
        int flag = 0;
        char* id = readStr(&flag);
        if (!id) return;
        n = scanf("%d/%d", &ta, &ts);
        if (flag){
            queuePushHead(q, id, ta, ts);
        }
        else {
            queuePushTail(q, id, ta, ts);
        }
        n = scanf("%c", &stop);
        queuePrint(q);
        free(id);
    }while (n > 0 && stop != '\n');
}



int main(){
    Queue* q = queueInit();
    getInfo(q);
    queueFree(q);
    return 0;
}