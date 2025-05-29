#include "estrategia.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pilasycolas.h"

static int posiciones_validas(Coordenada *validas, TipoCasilla **casillas, int alto, int ancho) {
    int cant_posiciones_validas = 0;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            if (casillas[i][j] == VACIO) {
                validas[cant_posiciones_validas].x = i;
                validas[cant_posiciones_validas].y = j;
                cant_posiciones_validas++;
            }
        }
    }

    return cant_posiciones_validas;
}

int count_caminos(Coordenada posicion, , Mapa *mapa){
    int i = 0;
    for (int rangoy = posicion.x - mapa->distancia_ataque; rangoy <= posicion.x + mapa->distancia_ataque; rangoy++){
        for (int rangox = posicion.y - mapa->distancia_ataque; rangox <= posicion.y + mapa->distancia_ataque; rangox++){
            if (rangox >= 0 && rangoy >= 0 && rangox < mapa->ancho && rangoy < mapa->alto) { // chequeo de límites
                if(mapa->casillas[rangox][rangoy] == CAMINO)
                    i++;
            }
        }
    }
    return i;
}

void caminos_torre(Coordenada *validas, int cant_posiciones_validas, Mapa *mapa, Torre *torres_posibles){
    for (int i = 0; i < cant_posiciones_validas; i++){
        torres_posibles[i].posicion.x = validas[i].x;
        torres_posibles[i].posicion.y = validas[i].y;
        torres_posibles[i].enemigos_golpeados = count_caminos(validas[i], mapa);
    }
}

void swap(Torre *a, Torre *b) {
    Torre temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(Torre arr[], int bajo, int alto) {
    int pivote = arr[alto].enemigos_golpeados; 
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (arr[j].enemigos_golpeados > pivote) { 
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[alto]);
    return i + 1;
}

void quicksort(Torre arr[], int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionar(arr, bajo, alto);
        quicksort(arr, bajo, pi - 1);
        quicksort(arr, pi + 1, alto);
    }
}

static void colocar_torre(Mapa *mapa, int x, int y, int nro_torre) {
    // actualizar torre
    mapa->torres[nro_torre].x = x;
    mapa->torres[nro_torre].y = y;

    // actualizar mapa
    mapa->casillas[x][y] = TORRE;
}

static int determinar_posicion_torre(int *casilla_elegida, int cant_validas) {
    int nueva_posicion = rand() % cant_validas;
    while(casilla_elegida[nueva_posicion])
        nueva_posicion = rand() % cant_validas;

    return nueva_posicion;
}


void disponer(Nivel* nivel, Mapa* mapa) {

    int cantidad_casillas = mapa->alto * mapa->ancho;

    Coordenada posiciones_validas_torre[cantidad_casillas];

    int casilla_elegida[cantidad_casillas];
    for(int i = 0; i < cantidad_casillas; casilla_elegida[i++] = 0);

    int cant_validas = posiciones_validas(posiciones_validas_torre, mapa->casillas, mapa->alto, mapa->ancho);

    for (int colocadas = 0; colocadas < mapa->cant_torres; colocadas++) {
        int nueva_torre = determinar_posicion_torre(casilla_elegida, cant_validas);
        casilla_elegida[nueva_torre] = 1;
        int nueva_torre_x = posiciones_validas_torre[nueva_torre].x;
        int nueva_torre_y = posiciones_validas_torre[nueva_torre].y;
        colocar_torre(mapa, nueva_torre_x, nueva_torre_y, colocadas);
    }
}

void disponer_custom(Nivel* nivel, Mapa* mapa) {
    
    Torre *torres_posibles = malloc(sizeof(Torre) * cant_posiciones_validas);
    
    if (torres_posibles == NULL) {
        fprintf(stderr, "Error de memoria\n");
        return;
    }
    int cantidad_casillas = mapa->alto * mapa->ancho;
    
    Coordenada validas[cantidad_casillas];

    int cant_posiciones_validas = posiciones_validas(validas, mapa->casillas, mapa->alto, mapa->ancho);;

    caminos_torre(validas, cant_posiciones_validas, mapa, torres_posibles);

    quicksort(torres_posibles, 0, cant_posiciones_validas - 1);

    Torre *torres_mejores = malloc(sizeof(Torre) * mapa->cant_torres);
    if (torres_mejores == NULL) {
        fprintf(stderr, "Error de memoria\n");
        free(torres_posibles);
        return;
    }

    for(int i = 0; i < mapa->cant_torres; i++){
        torres_mejores[i] = torres_posibles[i];
    }

    free(torres_posibles);
    free(torres_mejores);
}

void disponer_con_backtracking(Nivel* nivel, Mapa* mapa) {

}
