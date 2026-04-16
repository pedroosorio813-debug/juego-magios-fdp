#include "ascension_magios.h"  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
int main() {
    juego_t juego;
    inicializar_juego(&juego);   
    mostrar_juego(juego);    
    char fuera = 'X';
    char tecla;
    printf("Ingrese un movimiento: ");
    scanf(" %c", &tecla);
    while(tecla != fuera){
       realizar_jugada(&juego, tecla);
       mostrar_juego(juego);
       printf("Ingrese un movimiento: ");
       scanf(" %c", &tecla);
    }

    return 0;
}