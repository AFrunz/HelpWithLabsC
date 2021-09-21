#ifndef C_QUEUE_H
#define C_QUEUE_H
//#define VECTOR 100

#ifndef VECTOR


typedef struct Queue_el{
    char* id;
    int arrival_time;   // время прибытия
    int service_time;   // длительность приема
    struct Queue_el *next;
}Queue_el;

typedef struct Queue{
    Queue_el* head;
    Queue_el* tail;
}Queue;

Queue* queueInit();

int queuePushHead(Queue* q, char* id, int time_a, int time_s);

int queuePushTail(Queue* q, char* id, int time_a, int time_s);

Queue_el* queuePull(Queue* q);

void queuePrint(Queue* q);

void queueFree(Queue *q);


#else

typedef struct Queue_el{
    char* id;
    int arrival_time;   // время прибытия
    int service_time;   // длительность приема
    struct Queue_el *next;
}Queue_el;

typedef struct Queue{
    int head;
    int tail;
    int n;
    int lvl;
    Queue_el *elements;
}Queue;

Queue* queueInit();

int queuePushHead(Queue* q, char* id, int time_a, int time_s);

int queuePushTail(Queue* q, char* id, int time_a, int time_s);

Queue_el* queuePull(Queue* q);

void queuePrint(Queue* q);

void queueFree(Queue *q);


#endif






#endif //C_QUEUE_H
