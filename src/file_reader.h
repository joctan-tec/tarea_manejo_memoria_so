#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definimos el struct para almacenar las operaciones
typedef struct {
    char operation[10];  // ALLOC, REALLOC, FREE, PRINT
    char varName[20];    // Nombre de la variable (null para PRINT)
    int size;            // Tamaño (null para PRINT y FREE)
} Operation;

// Función para contar las líneas válidas (no son comentarios) en el archivo
int contarLineasValidas(FILE *archivo) {
    char linea[100];
    int cuenta = 0;
    
    while (fgets(linea, sizeof(linea), archivo)) {
        if (linea[0] != '#' && linea[0] != '\n' && strlen(linea) != 1) {
            cuenta++;
        }
        
    }

    // Volvemos al inicio del archivo
    rewind(archivo);
    
    return cuenta;
}

// Función para parsear el archivo y llenar el array de operaciones
Operation* leerArchivo(const char *nombreArchivo, int *numOperaciones) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return NULL;
    }

    // Contamos las líneas válidas
    *numOperaciones = contarLineasValidas(archivo);
    
    // Creamos un array dinámico de operaciones
    Operation *operaciones = (Operation *)malloc(*numOperaciones * sizeof(Operation));
    if (operaciones == NULL) {
        printf("Error al asignar memoria.\n");
        fclose(archivo);
        return NULL;
    }

    char linea[100];
    int index = 0;

    // Leemos línea por línea
    while (fgets(linea, sizeof(linea), archivo)) {
        // Ignoramos comentarios
        if (linea[0] == '#' || linea[0] == '\n' || strlen(linea) == 1 ) { 
            continue;
        }

        // Parseamos las líneas según la operación
        if (strstr(linea, "ALLOC") || strstr(linea, "REALLOC")) {
            sscanf(linea, "%s %s %d", operaciones[index].operation, operaciones[index].varName, &operaciones[index].size);
        } else if (strstr(linea, "FREE")) {
            sscanf(linea, "%s %s", operaciones[index].operation, operaciones[index].varName);
            operaciones[index].size = -1;  // Indicamos que no tiene tamaño
        } else if (strstr(linea, "PRINT")) {
            strcpy(operaciones[index].operation, "PRINT");
            strcpy(operaciones[index].varName, "null");
            operaciones[index].size = -1;  // Indicamos que no tiene tamaño
        }

        index++;
    }

    fclose(archivo);
    return operaciones;
}

// Función para imprimir el array de operaciones
void imprimirOperaciones(Operation *operaciones, int numOperaciones) {
    printf("List:\n");
    for (int i = 0; i < numOperaciones; i++) {
        printf("Operacion: %s", operaciones[i].operation);
        if (strcmp(operaciones[i].varName, "null") != 0) {
            printf(", Variable: %s", operaciones[i].varName);
        }
        if (operaciones[i].size != -1) {
            printf(", Size: %d", operaciones[i].size);
        }
        printf("\n");
    }
}