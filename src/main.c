// This file receives the path of the file to be read and returns the execution of the program.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./file_reader.h"


int main(int argc, char *argv[]) {
    
    if (argc != 2) {
        printf("Error: Debe ingresar el nombre del archivo.\n");
        return 1;
    }

    // Leemos el archivo y obtenemos las operaciones
    int numOperaciones;
    Operation *operaciones = leerArchivo(argv[1], &numOperaciones);
    if (operaciones == NULL) {
        return 1;
    }

    // Ejecutamos las operaciones
    imprimirOperaciones(operaciones, numOperaciones);

    // Liberamos la memoria
    free(operaciones);


    return 0;
}