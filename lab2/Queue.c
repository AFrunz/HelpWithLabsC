#include "Queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef enum ans {
    ST_OK = 0,
    QUEUE_FULL = -1
}ans;




#ifndef VECTOR

// list

// Инициализация
Queue* queueInit(){
    return (Queue*)calloc(1, sizeof(Queue));
}

// Запись в начало
int queuePushHead(Queue* q, char* id, int time_a, int time_s){
    Queue_el *buf = (Queue_el*)calloc(1, sizeof(Queue_el));
    buf->next = q->head;
    buf->arrival_time = time_a;
    buf->service_time = time_s;
    buf->id = (char*)calloc(strlen(id) + 1, sizeof(char));
    strcpy(buf->id, id);
    q->head = buf;
    if (!q->tail) q->tail = buf;
    return ST_OK;
}

// Запись в конец
int queuePushTail(Queue* q, char* id, int time_a, int time_s){
    Queue_el *buf = (Queue_el*)calloc(1, sizeof(Queue_el));
    buf->arrival_time = time_a;
    buf->service_time = time_s;
    buf->id = (char*)calloc(strlen(id) + 1, sizeof(char));
    strcpy(buf->id, id);
    if (q->tail){
        q->tail->next = buf;
    }
    else {
        q->tail = buf;
        q->head = buf;
    }
    q->tail = buf;
    return ST_OK;
}

// Чтение
Queue_el* queuePull(Queue* q){
    Queue_el *buf = q->head;
    if (buf) q->head = q->head->next;
    return buf;
}

// Печать очереди
void queuePrint(Queue* q){
    Queue_el *buf = q->head;
    if (!q->head) return;
    while (buf != q->tail){
        printf("%3s %3d %3d --->", buf->id, buf->arrival_time, buf->service_time);
        buf = buf->next;
    }
    printf("%3s %3d %3d", buf->id, buf->arrival_time, buf->service_time);
    printf("\n");
}

// Получение строкового представления
void queueGetStr();

// Очистка памяти очереди
void queueFree(Queue *q){
    if (!q->head) {
        free(q);
        return;
    }
    Queue_el *iter = q->head;
    while (iter != q->tail){
        Queue_el *buf = iter;
        iter = iter->next;
        free(buf->id);
        free(buf);
    }
    free(iter->id);
    free(iter);
    free(q);
}

#else


// Инициализация
Queue* queueInit(){
    Queue *q = (Queue*)calloc(1, sizeof(Queue));
    q->n = VECTOR;
    q->elements = (Queue_el*)calloc(VECTOR, sizeof(Queue_el));
    return q;
}

// Запись в начало
int queuePushHead(Queue* q, char* id, int time_a, int time_s){
    if (q->lvl == q->n) return QUEUE_FULL;
    Queue_el *buf = NULL;
    if (q->head != 0){
        q->head = q->head - 1;
    }
    else {
        q->head = q->n - 1;
    }
    buf = q->elements + q->head;
    buf->arrival_time = time_a;
    buf->service_time = time_s;
    buf->id = (char*)calloc(strlen(id) + 1, sizeof(char));
    strcpy(buf->id, id);
    q->lvl++;
    return ST_OK;
}

// Запись в конец
int queuePushTail(Queue* q, char* id, int time_a, int time_s){
    if (q->lvl == q->n) return QUEUE_FULL;
    Queue_el *buf = NULL;
    if (q->tail != q->n){
        q->tail = q->tail + 1;
    }
    else {
        q->tail = 1;
    }
    buf = q->elements + q->tail - 1;
    buf->arrival_time = time_a;
    buf->service_time = time_s;
    buf->id = (char*)calloc(strlen(id) + 1, sizeof(char));
    strcpy(buf->id, id);
    q->lvl++;
    return ST_OK;
}

// Чтение
Queue_el* queuePull(Queue* q){

    if (q->lvl == 0) return NULL;
    if (q->head != q->n){
        q->head++;
        return q->elements + q->head - 1;
    }
    q->head = 0;
    return q->elements;
}

// Печать очереди
void queuePrint(Queue* q){


    int start = q->head;
    for (int i = 0; i < q->lvl; i++, start++){
        int pos;
        if (start >= q->n) {
            pos = start - q->n;
        }
        else {
            pos = start;
        }
        printf("%3s %3d %3d --->", q->elements[pos].id, q->elements[pos].arrival_time, q->elements[pos].service_time);
    }
    printf("\n");
}

// Получение строкового представления
void queueGetStr();

// Очистка памяти очереди
void queueFree(Queue *q){
    int start = q->head;
    for (int i = 0; i < q->lvl; i++, start++){
        int pos;
        if (start >= q->n) {
            pos = start - q->n;
        }
        else {
            pos = start;
        }
        free(q->elements[pos].id);
    }
    free(q->elements);
    free(q);
}


#endif




