#include "ascension_magios.h"  
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
int main() {
    juego_t juego;
    inicializar_juego(&juego);   
    mostrar_juego(juego);    
    
    char tecla;
    printf("Ingrese un movimiento: ");
    scanf(" %c", &tecla);
    realizar_jugada(&juego, tecla);
    mostrar_juego(juego);

    return 0;
}