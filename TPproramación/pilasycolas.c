#include "pilasycolas.h"

Node* crearNode(void* newData){
    Node* newNode = (Node*)malloc(sizeof (Node));
    newNode->dato = newData;
    newNode->next = NULL;
    return newNode;
}

void inicializarPila(Stack* stack){
    stack->head = NULL;
}

int vacioPila(Stack* stack){
    return stack->head == NULL;
}

void apilarPila(Stack* stack, void* newData){
    Node* newNode = crearNode(newData);

    if(!newNode){
        printf("\n Stack Over");
        return;
    }

    newNode->next = stack->head;
    stack->head = newNode;
}

void desapilarPila(Stack* stack){
    if(vacioPila(stack)){
        printf("\n Stack Under");
        return;
    } else {
        Node* temp = stack->head;
        stack->head = stack->head->next;
        free(temp);
    }
}

void* topePila(Stack* stack){
    if(!vacioPila(stack))
        return stack->head->dato;
    else {
        printf("\nStack es Vacio");
        return NULL;
    }
}

void imprimirPila(Stack* pila, void (*imprimir_dato)(void*)) {
    Node* actual = pila->head;
    while (actual != NULL) {
        imprimir_dato(actual->dato);
        actual = actual->next;
    }
}


