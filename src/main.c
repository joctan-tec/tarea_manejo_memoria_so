// This file receives the path of the file to be read and returns the execution of the program.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "./memory_manager.h"
#include "./file_reader.h"



// Definición de la función principal
int main(int argc, char *argv[]) {
    
    // Verificar si se ingresaron los argumentos necesarios
    if (argc != 3) {
        printf("Error: Debe ingresar el nombre del archivo de operaciones y el algoritmo de asignación de memoria.\n");
        return 1;
    }

    const char* nombreArchivo = argv[1];
    const char* algoritmo = argv[2];
    

    // Verificar si el archivo de operaciones existe
    if (access(nombreArchivo, F_OK) == -1) {
        printf("Error: El archivo '%s' no existe.\n", nombreArchivo);
        return 1;
    }

    printf("Algoritmo Seleccionado: %s\n\n", algoritmo);

    // Verificar si el algoritmo de asignación es válido
    if (strcmp(algoritmo, "first_fit") != 0 && strcmp(algoritmo, "best_fit") != 0 && strcmp(algoritmo, "worst_fit") != 0) {
        printf("Error: El algoritmo de asignación '%s' no es válido.\n", algoritmo);
        printf("Los algoritmos válidos son: first-fit, best-fit, worst-fit.\n");
        return 1;
    }


    // Leemos el archivo y obtenemos las operaciones
    int numOperaciones;
    Operation *operaciones = leerArchivo(nombreArchivo, &numOperaciones);
    if (operaciones == NULL) {
        return 1;
    }

    size_t total_memory_size = 2048; // 2 KiB
    unsigned char* memory = (unsigned char*)malloc(total_memory_size);
    HashMap* assignments_map = create_hashmap();
    
    if (!memory) {
        printf("Error: No se pudo asignar el bloque de memoria grande.\n");
        return -1;
    }

    printf("Memoria solicitada al sistema operativo: %zu bytes\n\n", total_memory_size);

    // Estructura que maneja la lista de bloques de memoria
    MemoryList memory_list = {NULL, NULL};
    // Particionar la memoria en bloques de 32 bytes
    partition_memory(memory, total_memory_size, &memory_list);


    // Ejecutar las operaciones
    for (int i = 0; i < numOperaciones; i++) {
        Operation op = operaciones[i];

        // Comparar la operación ALLOC
        if (strcmp(op.operation, "ALLOC") == 0) {
            printf("Operación: %s, Variable: %s, Tamaño: %d\n", op.operation, op.varName, op.size);
            if (strcmp(algoritmo, "first_fit") == 0) {
                int result = first_fit_alloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s asignada correctamente\n", op.varName);
                } else {
                    printf("Error al asignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }else if (strcmp(algoritmo, "best_fit") == 0) {
                int result = best_fit_alloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s asignada correctamente\n", op.varName);
                } else {
                    printf("Error al asignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }else if (strcmp(algoritmo, "worst_fit") == 0) {
                int result = worst_fit_alloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s asignada correctamente\n", op.varName);
                } else {
                    printf("Error al asignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }




        }else if(strcmp(op.operation, "REALLOC") == 0){
            printf("Operación: %s, Variable: %s, Tamaño: %d\n", op.operation, op.varName, op.size);
            
            if (strcmp(algoritmo, "first_fit") == 0) {
                int result = first_fit_realloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s reasignada correctamente\n", op.varName);
                } else {
                    printf("Error al reasignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }else if (strcmp(algoritmo, "best_fit") == 0) {
                int result = best_fit_realloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s reasignada correctamente\n", op.varName);
                } else {
                    printf("Error al reasignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }else if(strcmp(algoritmo, "worst_fit") == 0){
                int result = worst_fit_realloc(op.varName, op.size, &memory_list, assignments_map);
                if (result == 0) {
                    printf("Variable %s reasignada correctamente\n", op.varName);
                } else {
                    printf("Error al reasignar la variable %s\n", op.varName);
                    // Liberar toda la memoria y salir
                    free_hashmap(assignments_map);
                    free(memory);
                    free(operaciones);
                    return 1;
                }
            }

        }else if(strcmp(op.operation, "FREE") == 0){
            printf("Operación: %s, Variable: %s\n", op.operation, op.varName);
            int result = free_memory(op.varName, &memory_list, assignments_map);
            if (result == 0) {
                printf("Variable %s liberada correctamente\n", op.varName);
            } else {
                printf("Error al liberar la variable %s\n", op.varName);
                // Liberar toda la memoria y salir
                free_hashmap(assignments_map);
                free(memory);
                free(operaciones);
                return 1;
            }
            
        }else if(strcmp(op.operation, "PRINT") == 0){
            printf("Operación: %s\n", op.operation);
            print_hashmap(assignments_map);
            print_memory_blocks(&memory_list);
        }else{
            printf("Operación no válida\n");
        }


    }

    // Liberar la memoria reservada al final
    free(memory);
    free(assignments_map);
    free(operaciones);


    return 0;
}