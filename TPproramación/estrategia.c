#include "estrategia.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "pilasycolas.h"

static int posiciones_validas(Coordenada *casilla, TipoCasilla **casillas, int alto, int ancho) {
    int cant_posiciones_validas = 0;

    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            if (casillas[i][j] == VACIO) {
                casilla[cant_posiciones_validas].x = i;
                casilla[cant_posiciones_validas].y = j;
                cant_posiciones_validas++;
            }
        }
    }

    return cant_posiciones_validas;
}

int count_caminos(int posicion_x, int posicion_y, Mapa *mapa){

    int cantidad_caminos = 0;

    for (int rango_x = -mapa->distancia_ataque; rango_x <= mapa->distancia_ataque; rango_x++) {
        for (int rango_y = -mapa->distancia_ataque; rango_y <= mapa->distancia_ataque; rango_y++) {
            int x = posicion_x + rango_x;
            int y = posicion_y + rango_y;

            // Verificar que la casilla esté dentro del mapa
            if (x >= 0 && x < mapa->alto && y >= 0 && y < mapa->ancho && mapa->casillas[x][y] == CAMINO)
                    cantidad_caminos++;
        }
    }
    return cantidad_caminos;
}

void caminos_torre(Coordenada *validas, int cant_posiciones_validas, Mapa *mapa, Torre *torres_posibles){
    for (int i = 0; i < cant_posiciones_validas; i++){
        torres_posibles[i].posicion.x = validas[i].x;
        torres_posibles[i].posicion.y = validas[i].y;
        torres_posibles[i].enemigos_golpeados = count_caminos(validas[i].x, validas[i].y, mapa);
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

void intercambiar(Torre *a, Torre *b) {
    Torre temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(Torre torres[], int bajo, int alto) {
    int pivote = torres[alto].enemigos_golpeados;
    int i = bajo - 1;

    for (int j = bajo; j < alto; j++) {
        if (torres[j].enemigos_golpeados > pivote) {
            i++;
            intercambiar(&torres[i], &torres[j]);
        }
    }
    intercambiar(&torres[i + 1], &torres[alto]);
    return i + 1;
}

void quicksort(Torre torres[], int bajo, int alto) {
    if (bajo < alto) {
        int pi = particionar(torres, bajo, alto);
        quicksort(torres, bajo, pi - 1);
        quicksort(torres, pi + 1, alto);
    }
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

    int cantidad_casillas = mapa->alto * mapa->ancho;
    Coordenada casillas[cantidad_casillas];

    //contamos las casillas que puedan tener una torre.
    int cant_posiciones_validas = posiciones_validas(casillas, mapa->casillas, mapa->alto, mapa->ancho);;

    //arreglo con cordenadas de torre y daño de cada una
    Torre torres[cant_posiciones_validas];

    //compreto el arreglo con la informacion correcta
    caminos_torre(casillas, cant_posiciones_validas, mapa, torres);

    quicksort(torres, 0, cant_posiciones_validas - 1);

    for(int i = 0; i < mapa->cant_torres; i++){
        int nueva_torre_x = torres[i].posicion.x;
        int nueva_torre_y = torres[i].posicion.y;
        colocar_torre(mapa, nueva_torre_x, nueva_torre_y, i);
    }
}

void disponer_con_backtracking(Nivel* nivel, Mapa* mapa) {

}