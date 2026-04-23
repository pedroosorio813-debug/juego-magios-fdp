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

    bool es_ocupada = false;
    for (int i = 0; i < nivel.tope_paredes; i++) {
        if (nivel.paredes[i].fil == pos.fil && nivel.paredes[i].col == pos.col) {
            es_ocupada = true;
        }
    }

    for (int i = 0; i < nivel.tope_camino; i++) {
        if (nivel.camino[i].fil == pos.fil && nivel.camino[i].col == pos.col) {
            es_ocupada = true;
        }
    }

    for(int i = 0; i < nivel.tope_herramientas; i++) {
        if(nivel.herramientas[i].posicion.fil == pos.fil && nivel.herramientas[i].posicion.col == pos.col){
            es_ocupada = true;
        }
    }

     for(int i = 0; i < nivel.tope_obstaculos; i++) {
        if(nivel.obstaculos[i].posicion.fil == pos.fil && nivel.obstaculos[i].posicion.col == pos.col){
            es_ocupada = true;
        }
     }
    
     if (nivel.pergamino.fil == pos.fil && nivel.pergamino.col == pos.col) {
        es_ocupada = true;
     }

    return es_ocupada;
}

int distancia_manhattan(coordenada_t pos_1, coordenada_t pos_2) {
    return abs(pos_1.fil - pos_2.fil) + abs(pos_1.col - pos_2.col);
}

bool esta_totem(juego_t juego ,coordenada_t pos_homero){
    bool aux = false;
    nivel_t nivel = juego.niveles[juego.nivel_actual];
    for(int i = 0; i < nivel.tope_herramientas; i++){
        if(nivel.herramientas[i].posicion.fil == pos_homero.fil && nivel.herramientas[i].posicion.col == pos_homero.col
           && nivel.herramientas[i].tipo == TOTEM){
            aux = true;
        }
    }
    return aux;
}

void eliminar_elemento_herramientas(juego_t* juego, coordenada_t pos_homero) {
    int indice = 0;
    nivel_t* nivel = &((*juego).niveles[(*juego).nivel_actual]);
    for(int i = 0; i < (*nivel).tope_herramientas;i++){
        if(pos_homero.fil == (*nivel).herramientas[i].posicion.fil && pos_homero.col == (*nivel).herramientas[i].posicion.col){
            indice = i;
        }
    }

    for(int i = indice; i < (*nivel).tope_herramientas - 1; i++){
        (*nivel).herramientas[i] = (*nivel).herramientas[i + 1];
    }
    (*nivel).tope_herramientas--;
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
    char celda = '.';

    if (juego.homero.posicion.fil == fil && juego.homero.posicion.col == col) {
        celda = HOMERO;
    }

    for (int i = 0; i < nivel_actual.tope_herramientas && celda == '.'; i++) {
        if (nivel_actual.herramientas[i].posicion.fil == fil && 
            nivel_actual.herramientas[i].posicion.col == col) {
            celda = nivel_actual.herramientas[i].tipo;
        }
    }

    for (int i = 0; i < nivel_actual.tope_obstaculos && celda == '.'; i++) {
        if (nivel_actual.obstaculos[i].posicion.fil == fil && 
            nivel_actual.obstaculos[i].posicion.col == col) {
            celda = nivel_actual.obstaculos[i].tipo;
        }
    }

    
    for (int i = 0; i < nivel_actual.tope_paredes && celda == '.'; i++) {
        if (nivel_actual.paredes[i].fil == fil && 
            nivel_actual.paredes[i].col == col) {
            celda = PARED;
        }
    }

    if (celda == '.' && (juego.camino_visible || juego.homero.antorcha_encendida)) {
        for (int i = 0; i < ultima_posicion && celda == '.'; i++) {
            if (nivel_actual.camino[i].fil == fil && 
                nivel_actual.camino[i].col == col) {
                int distancia = distancia_manhattan(juego.homero.posicion, nivel_actual.camino[i]);
                if (juego.camino_visible || distancia <= DISTANCIA_ANTORCHA) {
                    if (i == 0) {
                        celda = RUNA;
                    } else if (i == ultima_posicion - 1) {
                        celda = ALTAR;
                    } else {
                        celda = CAMINO;
                    }
                }
            }
        }
    }

    // Pergamino
    if (celda == '.' && !juego.homero.recolecto_pergamino && 
        juego.niveles[juego.nivel_actual].pergamino.fil == fil && 
        juego.niveles[juego.nivel_actual].pergamino.col == col) {
        celda = PERGAMINO;
    }

    return celda;
}

bool es_pared(nivel_t nivel, coordenada_t posicion) {
    bool aux = false;
    int i = 0;

    while (i < nivel.tope_paredes && !aux) {
        if (nivel.paredes[i].fil == posicion.fil && nivel.paredes[i].col == posicion.col) {
            aux = true;
        }
        i++;
    }

    return aux;
}

bool esta_fuera_mapa(coordenada_t posicion){
    if(posicion.fil < 0 || posicion.fil >= MAX_FILAS || posicion.col < 0 || posicion.col >= MAX_COLUMNAS){
        return true;
    }else{
        return false;
    }
}



//_______________________________________________________________________________________


void inicializar_juego(juego_t *juego) {
    (*juego).tope_niveles = MAX_NIVELES;
    (*juego).nivel_actual = 0;
    (*juego).homero.posicion = (*juego).niveles[0].camino[0];

    for (int i = 0; i < MAX_NIVELES; i++) {
        (*juego).niveles[i].tope_herramientas = 0;
        (*juego).niveles[i].tope_obstaculos = 0;
        obtener_mapa((*juego).niveles[i].paredes, &((*juego).niveles[i].tope_paredes),  
        (*juego).niveles[i].camino, &((*juego).niveles[i].tope_camino), i + 1);
    }

   
    (*juego).homero.vidas_restantes = VIDAS_INICIALES;
    (*juego).homero.hechizos_reveladores = HECHIZOS_INICIALES;
    (*juego).homero.antorchas = ANTORCHAS_POR_NIVEL;
    (*juego).homero.antorcha_encendida = false;
    (*juego).homero.recolecto_pergamino = false;
    (*juego).camino_visible = false;

    for (int i = 0; i < MAX_NIVELES; i++) {
        int pergamino_aleatorio = num_aleatorio_pergamino((*juego).niveles[i].tope_camino);
        (*juego).niveles[i].pergamino = (*juego).niveles[i].camino[pergamino_aleatorio];
        inicializar_objetos_dinamicos(&((*juego).niveles[i]), (*juego).homero);
    }
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
    printf("\n");
    printf("%d ", juego.niveles[juego.nivel_actual].tope_herramientas);
    printf("\n");
}

/*
 * Pre condiciones: El juego debe estar inicializado previamente con `inicializar_juego` y la acción
 * debe ser válida.
 * Post condiciones: Realizará la acción recibida por parámetro actualizando el juego.
 */

void realizar_jugada(juego_t *juego, char movimiento) {
     coordenada_t nueva_pos = (*juego).homero.posicion;
     nivel_t nivel = (*juego).niveles[(*juego).nivel_actual];

     if(movimiento == DERECHA){
        nueva_pos.col++;
     }
     else if(movimiento == IZQUIERDA){
        nueva_pos.col--;
     }
     else if(movimiento == ARRIBA){
        nueva_pos.fil--;
     }
     else if(movimiento == ABAJO){
        nueva_pos.fil++;
     }


     if(!es_pared(nivel,nueva_pos) && !esta_fuera_mapa(nueva_pos)){
        (*juego).homero.posicion = nueva_pos;
     }

     if(esta_totem((*juego),nueva_pos)){
        (*juego).homero.vidas_restantes++;
        eliminar_elemento_herramientas(juego,nueva_pos);
     }

    
    /*
        recibe las teclas que presiona el usuario que pueden ser D A L S W H luego de ser presionanda 
        homero debe moverse de su posicion actual pero no debe pasar los limites del mapa 
        ni tampoco pisar ni traspasar las paredes
    */
  
}


/*
    3.7.3 Tótem
Habra 5 tótems inicializados de forma aleatoria en todo el terreno en cada nivel. Si Homero se posiciona sobre uno, 
este le otorgará una vida extra. 
Una vez que Homero se haya posicionado sobre un tótem, este se eliminará del vector de herramientas.
*/



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


