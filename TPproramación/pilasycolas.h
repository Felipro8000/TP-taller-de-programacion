#ifndef IMPLE_H
#define IMPLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include "nivel.h"
#endif

typedef struct Node {
    Torre* dato;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* head;
} Stack;

Node* crearNode(void*);

void inicializarPila(Stack*);

int vacioPila(Stack*);

void apilarPila(Stack*, void*);

void desapilarPila(Stack*);

void* topePila(Stack*);

void imprimirPila(Stack* pila, void (*imprimir_dato)(void*));


typedef struct Cola {
    Node* front;
    Node* rear;
} Queue;


Queue* createQueue();

int vacioQueue(Queue*);

void enqueue(Queue*);

void dequeue(Queue*);

void printQueue(Queue*);

