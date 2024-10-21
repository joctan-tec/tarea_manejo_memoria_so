#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

// Definimos el struct para almacenar las operaciones
typedef struct {
    char operation[10];  // ALLOC, REALLOC, FREE, PRINT
    char varName[20];    // Nombre de la variable (null para PRINT)
    int size;            // Tamaño (null para PRINT y FREE)
} Operation;

// Función para validar si el nombre de la variable es válido
int validar_nombre_variable(const char *varName) {
    regex_t regex;
    const char *regex_varName = "^[A-Za-z_][A-Za-z0-9_]*$";
    regcomp(&regex, regex_varName, REG_EXTENDED);
    int reti = regexec(&regex, varName, 0, NULL, 0);
    regfree(&regex);
    return !reti;  // Retorna 1 si es válido, 0 si es inválido
}

// Función para validar las líneas de operación y devolver la razón del error
int validar_linea(const char *linea, char *razonError) {
    regex_t regex;
    int reti;
    char operation[10], varName[20];
    int size;

    // Expresiones regulares para las operaciones válidas
    const char *regex_alloc_realloc = "^\\s*(ALLOC|REALLOC)\\s+[A-Za-z_][A-Za-z0-9_]*\\s+[1-9][0-9]*\\s*$";
    const char *regex_free = "^\\s*FREE\\s+[A-Za-z_][A-Za-z0-9_]*\\s*$";
    const char *regex_print = "^\\s*PRINT\\s*$";

    // Intentamos con la primera expresión regular (ALLOC o REALLOC)
    reti = regcomp(&regex, regex_alloc_realloc, REG_EXTENDED);
    if (reti) {
        sprintf(razonError, "Error al compilar la expresión regular ALLOC/REALLOC.");
        return 0;
    }
    reti = regexec(&regex, linea, 0, NULL, 0);
    regfree(&regex);
    if (!reti) {
        // Verificamos si el nombre de la variable es válido
        sscanf(linea, "%s %s %d", operation, varName, &size);
        if (!validar_nombre_variable(varName)) {
            sprintf(razonError, "Error en el nombre de la variable: '%s'. Los nombres válidos deben comenzar con una letra o guion bajo (_), seguido de letras, números o guion bajo. Caracteres válidos: [A-Za-z0-9_]", varName);
            return 0;
        }
        // Verificamos si el tamaño es un número positivo
        if (size <= 0) {
            sprintf(razonError, "Error en el tamaño: '%d'. El tamaño debe ser un número entero positivo mayor que 0.", size);
            return 0;
        }
        return 1;  // Línea válida
    }

    // Intentamos con la segunda expresión regular (FREE)
    reti = regcomp(&regex, regex_free, REG_EXTENDED);
    if (reti) {
        sprintf(razonError, "Error al compilar la expresión regular FREE.");
        return 0;
    }
    reti = regexec(&regex, linea, 0, NULL, 0);
    regfree(&regex);
    if (!reti) {
        sscanf(linea, "%s %s", operation, varName);
        // Verificamos si el nombre de la variable es válido
        if (!validar_nombre_variable(varName)) {
            sprintf(razonError, "Error en el nombre de la variable: '%s'. Los nombres válidos deben comenzar con una letra o guion bajo (_), seguido de letras, números o guion bajo. Caracteres válidos: [A-Za-z0-9_]", varName);
            return 0;
        }
        return 1;  // Línea válida
    }

    // Intentamos con la tercera expresión regular (PRINT)
    reti = regcomp(&regex, regex_print, REG_EXTENDED);
    if (reti) {
        sprintf(razonError, "Error al compilar la expresión regular PRINT.");
        return 0;
    }
    reti = regexec(&regex, linea, 0, NULL, 0);
    regfree(&regex);
    if (!reti) return 1;  // Coincidencia PRINT

    // Si llegamos aquí, la línea es inválida. Ahora revisamos por qué.
    // Verificamos si hay un problema con ALLOC o REALLOC
    if (strstr(linea, "ALLOC") || strstr(linea, "REALLOC")) {
        if (strstr(linea, "ALLOC")) {
            sprintf(razonError, "Error en ALLOC: La operación debe seguir el formato 'ALLOC <var> <size>' con size > 0.");
        } else {
            sprintf(razonError, "Error en REALLOC: La operación debe seguir el formato 'REALLOC <var> <size>' con size > 0.");
        }
        return 0;
    }

    // Verificamos si hay un problema con FREE
    if (strstr(linea, "FREE")) {
        sprintf(razonError, "Error en FREE: La operación debe seguir el formato 'FREE <var>'.");
        return 0;
    }

    // Verificamos si hay un problema con PRINT
    if (strstr(linea, "PRINT")) {
        sprintf(razonError, "Error en PRINT: La operación 'PRINT' no debe tener argumentos.");
        return 0;
    }

    // Si no coincide con ninguna operación válida
    sprintf(razonError, "Operación no reconocida o malformada.");
    return 0;
}

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
    char razonError[100];  // Buffer para almacenar la razón del error
    int index = 0;
    int line_number = 0;

    // Leemos línea por línea
    while (fgets(linea, sizeof(linea), archivo)) {
        line_number++;  // Incrementamos el número de línea

        // Ignoramos comentarios y líneas vacías
        if (linea[0] == '#' || linea[0] == '\n' || strlen(linea) == 1 ) { 
            continue;
        }

        // Validamos si la línea tiene un formato válido
        if (!validar_linea(linea, razonError)) {
            printf("Error en la línea %d: %s\n", line_number, razonError);
            printf("Línea: %s", linea);
            fclose(archivo);
            free(operaciones);
            return NULL;
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
