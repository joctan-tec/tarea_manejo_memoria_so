#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "./hashmap.h"

typedef struct MemoryBlock {
    void* start_address;       // Dirección de inicio del bloque actual
    size_t offset;             // Tamaño del bloque
    int is_free;               // Estado del bloque (1 = libre, 0 = ocupado)
    struct MemoryBlock* next;  // Apunta al siguiente bloque en la lista
} MemoryBlock;

// Nueva estructura que contendrá la lista de bloques
typedef struct {
    MemoryBlock* head;  // Apunta al primer bloque
    MemoryBlock* tail;  // Apunta al último bloque
} MemoryList;

MemoryBlock* create_block(void* start_address, size_t offset, int is_free) {
    MemoryBlock* block = (MemoryBlock*)malloc(sizeof(MemoryBlock));
    block->start_address = start_address;
    block->offset = offset;
    block->is_free = is_free;
    block->next = NULL;
    return block;
}

void add_block(MemoryList* list, void* start_address, size_t offset, int is_free) {
    // Crear el nuevo bloque
    MemoryBlock* new_block = create_block(start_address, offset, is_free);

    // Si la lista está vacía, el nuevo bloque será el primero y el último
    if (list->head == NULL) {
        list->head = new_block;
        list->tail = new_block;
    } else {
        // Enlazar el nuevo bloque al final de la lista
        list->tail->next = new_block;
        // Actualizar la referencia al último bloque
        list->tail = new_block;
    }
}

// Particiona la memoria en bloques pequeños de tamaño fijo = 32 bytes
void partition_memory(unsigned char* memory, size_t total_memory_size, MemoryList* list) {
    size_t block_size = 32;
    size_t num_blocks = total_memory_size / block_size;

    for (size_t i = 0; i < num_blocks; i++) {
        unsigned char* block_address = memory + i * block_size;
        add_block(list, block_address, block_size, 1);
    }
}

// Obtiene el primer bloque libre de la lista
MemoryBlock* get_free_block(MemoryList* list) {
    MemoryBlock* current = list->head;

    while (current != NULL) {
        if (current->is_free) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}



// Devuelve el bloque que inicia en la dirección de memoria dada
MemoryBlock* get_block_from_address(MemoryList* list, void* address) {
    MemoryBlock* current = list->head;

    while (current != NULL) {
        if (current->start_address == address) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// Obtiene el primer bloque libre a partir de una dirección de memoria dada
MemoryBlock* get_free_block_from_address(MemoryList memory_list ,void* address) {
    MemoryBlock* current = get_block_from_address(&memory_list, address)->next;

    while (current != NULL) {
        if (current->is_free) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

// Imprime el contenido de un bloque
void print_block_content(MemoryBlock* block) {
    printf("\nDirección de memoria\n");
    printf("------------------------------------------------\n");

    for (size_t i = 0; i < block->offset; i++) {
        unsigned char* mem_address = (unsigned char*)block->start_address + i;
        printf("| 0x%lx : 0x%lx | %02x ", 
               (unsigned long)(mem_address), 
               (unsigned long)(mem_address), 
               *mem_address);

        if (*mem_address >= 32 && *mem_address <= 126) {
            printf("--> %c ", *mem_address);
        } else {
            printf("    ");
        }
        printf("|\n");
    }
    printf("------------------------------------------------\n");
}

void print_memory_blocks(MemoryList* list) {
    MemoryBlock* current = list->head;
    int index = 0;

    while (current != NULL) {
        printf("Bloque #%d ", index);
        printf("Estado: %s\n", 
               
               current->is_free ? "Libre" : "Ocupado");
        print_block_content(current);
        printf("\n");
        current = current->next;
        index++;
    }
}

void print_memory_content(unsigned char* memory, size_t total_memory_size) {
    printf("\nDirección de memoria\n");
    printf("------------------------------------------------\n");

    for (size_t i = 0; i < total_memory_size; i++) {
        unsigned char* mem_address = memory + i;
        printf("| 0x%lx : 0x%lx | %02x ", 
               (unsigned long)(mem_address), 
               (unsigned long)(mem_address), 
               *mem_address);

        if (*mem_address >= 32 && *mem_address <= 126) {
            printf("--> %c ", *mem_address);
        } else {
            printf("    ");
        }
        printf("|\n");
    }
    printf("------------------------------------------------\n");
}

// Funcion para agregar contenido a un bloque de memoria
void add_content_to_block(MemoryBlock* block, char* content) {
    size_t content_length = strlen(content);
    if (content_length > block->offset) {
        printf("Error: El contenido no cabe en el bloque\n");
        return;
    }

    for (size_t i = 0; i < content_length; i++) {
        unsigned char* mem_address = (unsigned char*)block->start_address + i;
        *mem_address = content[i];
    }
}

/////////////////////////////////////////////////////////////////////////////////////

// Funcion para generar un char* con el contenido de un string, con la cantidad de bytes especificada
char* generate_string(int size, char* varName) {
    // Obtener la longitud de la cadena original
    int varNameLength = strlen(varName);
    
    // Calcular cuántas veces necesitamos repetir la cadena
    int repetitions = size / varNameLength;
    int remainder = size % varNameLength;

    // Reservar memoria para el nuevo string (tamaño total + 1 para el terminador nulo)
    char* result = (char*)malloc(size + 1);
    if (result == NULL) {
        return NULL; // Manejo de error si malloc falla
    }

    // Llenar el nuevo string con las repeticiones
    for (int i = 0; i < repetitions; i++) {
        strcpy(result + (i * varNameLength), varName);
    }
    
    // Copiar el resto de la cadena si es necesario
    if (remainder > 0) {
        strncpy(result + (repetitions * varNameLength), varName, remainder);
    }
    
    // Asegurar que el string resultante esté terminado en nulo
    result[size] = '\0';

    return result;
}



// Limpiar la memoria de un bloque
void clear_block(MemoryBlock* block) {
    memset(block->start_address, 0, block->offset);
}

// Funcion para liberar un bloque de memoria
void free_block(char* variable, MemoryList* memory_list, HashMap* assignments_map) {
    // Obtener la dirección de memoria asignada a la variable
    void* address = get_hashmap(assignments_map, variable);
    if (address == NULL) {
        printf("Variable %s no encontrada\n", variable);
        return;
    }

    // Obtener el bloque de memoria correspondiente
    MemoryBlock* block = get_block_from_address(memory_list, address);
    if (block == NULL) {
        printf("Variable %s no encontrada\n", variable);
        return;
    }

    // Liberar el bloque
    block->is_free = 1;
    printf("Liberado %ld bytes en la dirección %p\n", block->offset, block->start_address);
}

int free_memory(char* variable, MemoryList* memory_list, HashMap* assignments_map) {
    // Obtener la dirección de memoria asignada a la variable
    AddressArray* address = get_hashmap(assignments_map, variable);
    
    if (address == NULL) {
        printf("Variable %s no encontrada\n", variable);
        return 1;
    }

    // Recorrer el array de direcciones para liberar la memoria de esos bloques
    for (size_t i = 0; i < address->count; i++) {
        MemoryBlock* block = get_block_from_address(memory_list, address->addresses[i]);
        if (block == NULL) {
            printf("Variable %s no encontrada\n", variable);
            return 1;
        }
        block->is_free = 1;
        printf("Liberado %ld bytes en la dirección %p\n", block->offset, block->start_address);
    }

    // Liberar las direcciones de memoria asociadas a la variable
    delete_from_hashmap(assignments_map, variable);

    return 0;
}

// Funcion para buscar N bloques libres contiguos en la memoria
MemoryBlock* find_free_blocks(int num_blocks, MemoryList* memory_list) {
    MemoryBlock* current = memory_list->head;
    MemoryBlock* start_block = NULL;
    int count = 0;

    while (current != NULL) {
        if (current->is_free) {
            if (count == 0) {
                start_block = current;
            }
            count++;
            if (count == num_blocks) {
                return start_block;
            }
        } else {
            count = 0;
        }
        current = current->next;
    }

    return NULL;
}

int get_num_blocks(int size){
    int num_blocks = size / 32;
    if (size % 32 != 0) {
        num_blocks++;
    }
    return num_blocks;
}

int alloc_function(MemoryBlock* current, int num_blocks, int char_size, HashMap* assignments_map, char* variable){

    // Valida que el current no sea NULL
    if (current == NULL) {
        printf("No se encontraron bloques libres contiguos para %d bytes\n", char_size);
        // Levantar error de memoria insuficiente
        printf("Error: Memoria insuficiente\n");
        return 1;
    }

    for (int i = 0; i < num_blocks; i++) {
        clear_block(current);
        current->is_free = 0;
        add_content_to_block(current, generate_string(char_size > 32 ? 32 : char_size, variable));
        char_size -= 32;
        hashmap_insert(assignments_map, variable, current->start_address);
        current = current->next;
    }
    printf("Variable %s asignada correctamente\n", variable);
    return 0;
}

// Funcion para allocar memoria usando el algoritmo First-fit
int first_fit_alloc(char* variable,int size, MemoryList* memory_list, HashMap* assignments_map) {
    
    //Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    
    // Buscar los bloques libres necesarios
    MemoryBlock* start_block = find_free_blocks(num_blocks, memory_list);

    if (start_block == NULL) {
        printf("No se encontraron bloques libres contiguos para %d bytes\n", char_size);
        // Levantar error de memoria insuficiente
        printf("Error: Memoria insuficiente\n");
        return 1;
    }

    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}




int first_fit_realloc(char* variable, int size, MemoryList* memory_list, HashMap* assignments_map) {
    // Obtener la dirección de memoria asignada a la variable
    int result = free_memory(variable, memory_list, assignments_map);

    if(result == 1){
        return 1;
    }

    //Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    
    // Buscar los bloques libres necesarios
    MemoryBlock* start_block = find_free_blocks(num_blocks, memory_list);
    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}

MemoryBlock* find_best_fit_blocks(int num_blocks, MemoryList* memory_list) {
    MemoryBlock* current = memory_list->head;
    MemoryBlock* best_fit_start = NULL;
    int best_fit_size = INT_MAX;
    
    while (current != NULL) {
        if (current->is_free) {
            int count = 0;
            MemoryBlock* temp = current;

            // Contar bloques libres contiguos
            while (temp != NULL && temp->is_free) {
                count++;
                temp = temp->next;
            }

            // Verificar si el bloque actual es el mejor ajuste
            if (count >= num_blocks && count < best_fit_size) {
                
                best_fit_size = count;
                best_fit_start = current;
                
            }
        }
        current = current->next;
    }


    return best_fit_start;
}

int best_fit_realloc(char* variable, int size, MemoryList* memory_list, HashMap* assignments_map) {
    // Obtener la dirección de memoria asignada a la variable
    int result = free_memory(variable, memory_list, assignments_map);

    if(result == 1){
        return 1;
    }

    // Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    // Buscar el mejor ajuste de bloques libres necesarios
    MemoryBlock* start_block = find_best_fit_blocks(num_blocks, memory_list);
    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}


// Funcion para allocar memoria usando el algoritmo Best-fit
int best_fit_alloc(char* variable, int size, MemoryList* memory_list, HashMap* assignments_map) {
    
    // Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    // Buscar el mejor ajuste de bloques libres necesarios
    MemoryBlock* start_block = find_best_fit_blocks(num_blocks, memory_list);

    
    if (start_block == NULL) {
        printf("No se encontraron bloques libres contiguos para %d bytes\n", char_size);
        // Levantar error de memoria insuficiente
        printf("Error: Memoria insuficiente\n");
        return 1;
    }

    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}


MemoryBlock* find_worst_fit_blocks(int num_blocks, MemoryList* memory_list) {
    MemoryBlock* current = memory_list->head;
    MemoryBlock* worst_fit_start = NULL;
    int worst_fit_size = 0;
    
    while (current != NULL) {
        if (current->is_free) {
            int count = 0;
            MemoryBlock* temp = current;

            // Contar bloques libres contiguos
            while (temp != NULL && temp->is_free) {
                count++;
                temp = temp->next;
            }

            // Verificar si el bloque actual es el peor ajuste
            if (count >= num_blocks && count > worst_fit_size) {
                worst_fit_size = count;
                worst_fit_start = current;
            }
        }
        current = current->next;
    }

    printf("Worst fit block: %p\n", worst_fit_start->start_address);

    return worst_fit_start;
}

// Funcion para allocar memoria usando el algoritmo Worst-fit
int worst_fit_alloc(char* variable, int size, MemoryList* memory_list, HashMap* assignments_map) {
    // Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    // Buscar el peor ajuste de bloques libres necesarios
    MemoryBlock* start_block = find_worst_fit_blocks(num_blocks, memory_list);

    if (start_block == NULL) {
        printf("No se encontraron bloques libres contiguos para %d bytes\n", char_size);
        // Levantar error de memoria insuficiente
        printf("Error: Memoria insuficiente\n");
        return 1;
    }

    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}


int worst_fit_realloc(char* variable, int size, MemoryList* memory_list, HashMap* assignments_map) {
    // Obtener la dirección de memoria asignada a la variable
    int result = free_memory(variable, memory_list, assignments_map);

    if(result == 1){
        return 1;
    }
    
    // Cantidad de bloques necesarios
    int num_blocks = get_num_blocks(size);
    int char_size = size;
    // Buscar el peor ajuste de bloques libres necesarios
    MemoryBlock* start_block = find_worst_fit_blocks(num_blocks, memory_list);
    return alloc_function(start_block, num_blocks, char_size, assignments_map, variable);
}



