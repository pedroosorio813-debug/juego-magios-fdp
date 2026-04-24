#include "ascension_magios.h"  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

bool es_valido(char movimiento){
    bool aux = false;
    char mov_mayus = (char)toupper(movimiento);
    if(mov_mayus == 'W' || mov_mayus == 'A' || mov_mayus == 'D' || mov_mayus == 'S' || mov_mayus == 'L'
    || mov_mayus == 'H'){
        aux = true;
    }
    else {
        aux = false;
    }
    return aux;
}

char pedir_movimiento(){
    bool aux = false;
    char movimiento;
    while(!aux){
        printf("Ingrese por favor un movimiento valido: (A,D,W,S,H,L): ");
        scanf(" %c", &movimiento);
        if(es_valido(movimiento)){
            aux = true;
        }else{
            printf("Movimiento invalido \n");
        }
    }
    return movimiento;
}

int main() {
    juego_t juego;
    inicializar_juego(&juego);   

    int estado = 0;
    while(estado == 0){
       mostrar_juego(juego);

       char movimiento = pedir_movimiento();
       realizar_jugada(&juego, movimiento);
      
       if (estado_nivel(juego.niveles[juego.nivel_actual], juego.homero) == 1) {
           if (juego.nivel_actual < juego.tope_niveles - 1) {
                cambiar_nivel(&juego); 
            }
        }

        estado = estado_juego(juego);
    }

    mostrar_juego(juego); 
    if (estado == 1) {
        printf("¡Ganaste!\n");
    } else {
        printf("Perdiste...\n");
    }

    return 0;

 }

   
