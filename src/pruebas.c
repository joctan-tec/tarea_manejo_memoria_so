#include "./hashmap.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Suponiendo que las estructuras y funciones ya están definidas aquí...

int main() {
    // Crear un nuevo HashMap
    HashMap* map = create_hashmap();

    // Crear algunas variables y sus direcciones
    int a = 42;
    float b = 3.14;
    char c = 'X';

    // Insertar direcciones de las variables en el HashMap
    hashmap_insert(map, "variable_a", &a);
    hashmap_insert(map, "variable_b", &b);
    hashmap_insert(map, "variable_c", &c);

    // Imprimir el contenido del HashMap
    printf("Contenido del HashMap:\n");
    print_hashmap(map);

    // Obtener direcciones de una clave específica
    AddressArray* addresses = get_hashmap(map, "variable_a");
    if (addresses) {
        printf("Direcciones de 'variable_a':\n");
        for (size_t i = 0; i < addresses->count; i++) {
            printf("  - 0x%lx\n", (unsigned long)addresses->addresses[i]);
        }
    } else {
        printf("'variable_a' no encontrada.\n");
    }

    // Borrar una clave del HashMap
    delete_hashmap(map, "variable_b");

    // Imprimir el contenido después de borrar
    printf("\nContenido del HashMap después de borrar 'variable_b':\n");
    print_hashmap(map);

    // Liberar la memoria utilizada por el HashMap
    free_hashmap(map);

    return 0;
}