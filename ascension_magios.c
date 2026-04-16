#include "ascension_magios.h"
#include "utiles.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>


//REPRESENTACION DE ELEMENTOS

const char PERGAMINO = 'P';
const char HOMERO = 'H';
const char ALTAR = 'A';
const char RUNA = 'U';
const char PARED = 'X';
const char CAMINO = '*';
const char TOTEM = 'T';
const char PIEDRA = 'R';
const char CATAPULTA = 'F';

//ENTRADAS DEL USUARIO

const char ARRIBA = 'W';
const char ABAJO = 'S';
const char DERECHA = 'D';
const char IZQUIERDA = 'A';
const char USAR_HECHIZO = 'H';
const char USAR_ANTORCHA = 'L';

// VALORES DE INICIALIZACION

const int VIDAS_INICIALES = 5;
const int HECHIZOS_INICIALES = 5;
const int ANTORCHAS_POR_NIVEL = 5;
const int DISTANCIA_ANTORCHA = 3;
const int CANTIDAD_TOTEMS = 5;
const int CANTIDAD_PIEDRAS = 10;


//FUNCIONES-PRIVADAS

bool esta_ocupada(nivel_t nivel, coordenada_t pos) {
    for (int i = 0; i < nivel.tope_paredes; i++) {
        if (nivel.paredes[i].fil == pos.fil && nivel.paredes[i].col == pos.col) {
            return true;
        }
    }

    for (int i = 0; i < nivel.tope_camino; i++) {
        if (nivel.camino[i].fil == pos.fil && nivel.camino[i].col == pos.col) {
            return true;
        }
    }

    for(int i = 0; i < nivel.tope_herramientas; i++) {
        if(nivel.herramientas[i].posicion.fil == pos.fil && nivel.herramientas[i].posicion.col == pos.col){
            return true;
        }
    }

     for(int i = 0; i < nivel.tope_obstaculos; i++) {
        if(nivel.obstaculos[i].posicion.fil == pos.fil && nivel.obstaculos[i].posicion.col == pos.col){
            return true;
        }
     }
    
     if (nivel.pergamino.fil == pos.fil && nivel.pergamino.col == pos.col) {
        return true;
     }

    return false; 
}


coordenada_t obtener_posicion_vacia(nivel_t nivel, personaje_t homero) {
    coordenada_t posicion;
    do {
        posicion.fil = rand() % MAX_FILAS; 
        posicion.col = rand() % MAX_COLUMNAS;
    } while(esta_ocupada(nivel, posicion) || 
            (posicion.fil == homero.posicion.fil && 
             posicion.col == homero.posicion.col));

    return posicion;

}

void inicializar_objetos_dinamicos(nivel_t* nivel, personaje_t homero) {

    for (int i = 0; i < CANTIDAD_TOTEMS; i++) {
        (*nivel).herramientas[(*nivel).tope_herramientas].tipo = TOTEM;
        (*nivel).herramientas[(*nivel).tope_herramientas].posicion = obtener_posicion_vacia(*nivel, homero);
        (*nivel).tope_herramientas++;
    }

    for (int i = 0; i < CANTIDAD_PIEDRAS; i++) {
        (*nivel).obstaculos[(*nivel).tope_obstaculos].tipo = PIEDRA; 
        (*nivel).obstaculos[(*nivel).tope_obstaculos].posicion = obtener_posicion_vacia(*nivel, homero);
        (*nivel).tope_obstaculos++;
    }


    (*nivel).obstaculos[(*nivel).tope_obstaculos].tipo = CATAPULTA; 
    (*nivel).obstaculos[(*nivel).tope_obstaculos].posicion = obtener_posicion_vacia(*nivel, homero);
    (*nivel).tope_obstaculos++;
}

int num_aleatorio_pergamino(int tope_camino) {
    int minimo = 1;
    int maximo = tope_camino - 2;
    int num = rand() % (maximo - minimo + 1) + minimo;
    
    return num;
}

char mostrar_celda(juego_t juego, int fil, int col) {

    nivel_t nivel_actual = juego.niveles[juego.nivel_actual];
    int ultima_posicion = nivel_actual.tope_camino;

    if (juego.homero.posicion.fil == fil && juego.homero.posicion.col == col){
        return HOMERO;
    }


    for (int i = 0; i < nivel_actual.tope_herramientas; i++)
        if (nivel_actual.herramientas[i].posicion.fil == fil && nivel_actual.herramientas[i].posicion.col == col){
            return nivel_actual.herramientas[i].tipo;
        }

    
    for (int i = 0; i < nivel_actual.tope_obstaculos; i++)
        if (nivel_actual.obstaculos[i].posicion.fil == fil && nivel_actual.obstaculos[i].posicion.col == col){
            return nivel_actual.obstaculos[i].tipo;
        }


    for (int i = 0; i < nivel_actual.tope_paredes; i++) {
        if (nivel_actual.paredes[i].fil == fil && nivel_actual.paredes[i].col == col){
            return PARED;
        }
    }


    if (juego.camino_visible) {
    
         if (nivel_actual.camino[0].fil == fil && nivel_actual.camino[0].col == col) {
             return RUNA;
         }
    
         if (nivel_actual.camino[ultima_posicion - 1].fil == fil && nivel_actual.camino[ultima_posicion - 1].col == col) {
            return ALTAR;
         }

        for (int i = 1; i < ultima_posicion - 1; i++) { 
            if (nivel_actual.camino[i].fil == fil && nivel_actual.camino[i].col == col) {
                return CAMINO;
            }    
        }
    }

    if (!juego.homero.recolecto_pergamino && 
        juego.niveles[juego.nivel_actual].pergamino.fil == fil && 
        juego.niveles[juego.nivel_actual].pergamino.col == col) {
    
        return PERGAMINO; 
    }   

    return '.';
}

//_______________________________________________________________________________________


void inicializar_juego(juego_t *juego){

    int pergamino_aleatorio = num_aleatorio_pergamino((*juego).niveles[0].tope_camino);
    (*juego).tope_niveles = MAX_NIVELES;
    (*juego).nivel_actual = 0;
    (*juego).homero.posicion = (*juego).niveles[0].camino[0];

    for(int i = 0; i < MAX_NIVELES; i++){
        obtener_mapa((*juego).niveles[i].paredes, &((*juego).niveles[i].tope_paredes),
        (*juego).niveles[i].camino,&((*juego).niveles[i].tope_camino), i + 1);
    
        (*juego).niveles[i].tope_herramientas = 0;
        (*juego).niveles[i].tope_obstaculos = 0;

        (*juego).niveles[i].pergamino = (*juego).niveles[i].camino[pergamino_aleatorio];
        inicializar_objetos_dinamicos(&((*juego).niveles[i]),(*juego).homero); 
    }

    (*juego).homero.vidas_restantes = VIDAS_INICIALES;
    (*juego).homero.hechizos_reveladores = HECHIZOS_INICIALES;
    (*juego).homero.antorchas = ANTORCHAS_POR_NIVEL;
    (*juego).homero.antorcha_encendida = false;
    (*juego).camino_visible = false;
    (*juego).homero.recolecto_pergamino = false;
    
}


void mostrar_juego(juego_t juego){
    system("clear");
    
    printf("========== NIVEL %d ==========\n", juego.nivel_actual + 1);
    printf("Vidas: %d | Hechizos: %d | Antorchas: %d\n",
           juego.homero.vidas_restantes,
           juego.homero.hechizos_reveladores,
           juego.homero.antorchas);
    printf("\n");

    for(int fil = 0; fil < MAX_FILAS; fil++){
        for(int col = 0; col < MAX_COLUMNAS; col++){
            printf("%c ", mostrar_celda(juego,fil,col));
        }
        printf("\n");
    }
  
}

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */
void realizar_jugada(juego_t *juego, char movimiento){ 
    coordenada_t pos = (*juego).homero.posicion ;
    int derecha = pos.col++;
    int izquierda = pos.col--;
    int arriba = pos.fil--;
    int abajo = pos.fil++;

    for(int fil = 0; fil < MAX_FILAS; fil++){
        for(int col = 0; col < MAX_COLUMNAS; col++){
            if(arriba != fil && arriba != col){
                (*juego).homero.posicion.fil = pos.fil;
            }
             if(abajo != fil && abajo != col){
                (*juego).homero.posicion.fil = pos.fil;
            } 
             if(derecha != fil && derecha != col){
                (*juego).homero.posicion.col = pos.col;
             }
             if(izquierda != fil && izquierda != col){
                (*juego).homero.posicion.col = pos.col;
             }
        }
    }


    if(movimiento == DERECHA){
        (*juego).homero.posicion.col++;
    }else if(movimiento == IZQUIERDA){
        (*juego).homero.posicion.col--;
    }else if(movimiento == ARRIBA){
        (*juego).homero.posicion.fil--;
    }else {
        (*juego).homero.posicion.fil++;
    }
     
}







/*
    Fase 3: Movimiento y Lógica Básica

    Ciclo principal en juego.c: Armá el while que pida la tecla
    (W, A, S, D, H, L), valide que sea correcta y llame a realizar_jugada.

    realizar_jugada (Movimiento): Programá primero solo el movimiento de Homero y las colisiones con paredes.

    Lógica de visibilidad: Implementá cuándo camino_visible es true o false 
    (si está en la runa, si usó hechizo, etc.).
*/

/*
    Los Elementos Dinámicos (objeto_t)
    Estos son los que el enunciado pide guardar en los vectores herramientas y obstaculos.
    Se definen por tener un tipo (un char) y una posicion.
    Herramientas: Tótems (T).
    Obstáculos: Piedras de castigo (R) y la Catapulta (F).
*/


