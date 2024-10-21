#include <stdio.h>
#include <stdlib.h>

typedef struct Block {
    int size;              // Tamaño del bloque
    int isFree;           // Estado del bloque: libre (1) o ocupado (0)
    struct Block* next;    // Puntero al siguiente bloque
} Block;

Block* head = NULL; // Cabeza de la lista de bloques

// Inicializa la memoria con un bloque
void initializeMemory(int size) {
    head = (Block*)malloc(sizeof(Block));
    head->size = size;  // Tamaño del bloque inicial
    head->isFree = 1;   // Libre
    head->next = NULL;
}

// Asigna memoria usando el algoritmo First-fit
void firstFit(int size) {
    Block* current = head;
    while (current) {
        if (current->isFree && current->size >= size) {
            current->isFree = 0; // Marca como ocupado
            printf("First-fit: Allocated %d bytes at block size %d\n", size, current->size);
            return;
        }
        current = current->next;
    }
    printf("First-fit: No suitable block found for %d bytes\n", size);
}

// Asigna memoria usando el algoritmo Best-fit
void bestFit(int size) {
    Block* current = head;
    Block* best = NULL;

    while (current) {
        if (current->isFree && current->size >= size) {
            if (!best || current->size < best->size) {
                best = current; // Actualiza el mejor bloque
            }
        }
        current = current->next;
    }

    if (best) {
        best->isFree = 0; // Marca como ocupado
        printf("Best-fit: Allocated %d bytes at block size %d\n", size, best->size);
    } else {
        printf("Best-fit: No suitable block found for %d bytes\n", size);
    }
}

// Asigna memoria usando el algoritmo Worst-fit
void worstFit(int size) {
    Block* current = head;
    Block* worst = NULL;

    while (current) {
        if (current->isFree && current->size >= size) {
            if (!worst || current->size > worst->size) {
                worst = current; // Actualiza el peor bloque
            }
        }
        current = current->next;
    }

    if (worst) {
        worst->isFree = 0; // Marca como ocupado
        printf("Worst-fit: Allocated %d bytes at block size %d\n", size, worst->size);
    } else {
        printf("Worst-fit: No suitable block found for %d bytes\n", size);
    }
}

// Libera un bloque de memoria
void freeMemory(int size) {
    Block* current = head;
    while (current) {
        if (!current->isFree && current->size == size) {
            current->isFree = 1; // Marca como libre
            printf("Freed %d bytes from block size %d\n", size, current->size);
            return;
        }
        current = current->next;
    }
    printf("No allocated block of size %d found to free\n", size);
}

// Agrega un nuevo bloque de memoria a la lista
void addBlock(int size) {
    Block* newBlock = (Block*)malloc(sizeof(Block));
    newBlock->size = size;
    newBlock->isFree = 1; // Libre
    newBlock->next = head; // Coloca al inicio
    head = newBlock;
}

// Imprime el estado de los bloques de memoria
void printMemory() {
    Block* current = head;
    printf("Memory Blocks:\n");
    while (current) {
        printf("Size: %d, Status: %s\n", current->size, current->isFree ? "Free" : "Occupied");
        current = current->next;
    }
}

int main() {
    // Inicializa la memoria con un bloque grande
    initializeMemory(500); // Bloque inicial de 500 bytes

    // Agrega más bloques de memoria para simular un sistema más realista
    addBlock(300);         // Agrega un bloque de 300 bytes
    addBlock(200);         // Agrega un bloque de 200 bytes
    addBlock(50);          // Agrega un bloque de 50 bytes
    addBlock(400);         // Agrega un bloque de 400 bytes
    addBlock(600);         // Agrega un bloque de 600 bytes
    addBlock(100);         // Agrega un bloque de 100 bytes

    // Imprime el estado inicial de la memoria
    printf("Estado inicial de la memoria:\n");
    printMemory();
    
    // Prueba los algoritmos de asignación con diferentes tamaños
    printf("\nAsignando memoria con First-fit (100 bytes):\n");
    firstFit(100);  // Prueba First-fit con 100 bytes

    printf("\nAsignando memoria con Best-fit (250 bytes):\n");
    bestFit(250);   // Prueba Best-fit con 250 bytes

    printf("\nAsignando memoria con Worst-fit (300 bytes):\n");
    worstFit(300);  // Prueba Worst-fit con 300 bytes

    // Imprime el estado de la memoria después de las asignaciones
    printf("\nEstado de la memoria despues de asignaciones:\n");
    printMemory();
    
    // Más pruebas de asignación
    printf("\nAsignando memoria con First-fit (50 bytes):\n");
    firstFit(50);   // Prueba First-fit con 50 bytes

    printf("\nAsignando memoria con Best-fit (90 bytes):\n");
    bestFit(90);    // Prueba Best-fit con 90 bytes

    printf("\nAsignando memoria con Worst-fit (600 bytes):\n");
    worstFit(600);  // Prueba Worst-fit con 600 bytes

    // Imprime el estado de la memoria después de más asignaciones
    printf("\nEstado de la memoria despues de más asignaciones:\n");
    printMemory();

    // Pruebas de liberación de memoria
    printf("\nLiberando 100 bytes:\n");
    freeMemory(100);  // Libera el bloque de 100 bytes

    printf("\nLiberando 250 bytes:\n");
    freeMemory(250);  // Libera el bloque de 250 bytes

    printf("\nLiberando 300 bytes:\n");
    freeMemory(300);  // Libera el bloque de 300 bytes

    // Imprime el estado final de la memoria
    printf("\nEstado final de la memoria despues de liberar:\n");
    printMemory();

    return 0;
}



/*

Quiero crear un hashmap en C, que me permita almacenar pares de clave valor de la forma:

string : array(hexadecimal)

donde se represente esto:

"nombre_variable" : [direccion_memoria_1, direccion_memoria_2, direccion_memoria_3, ...]

Donde las direcciones de memoria corresponden al inicio de un bloque de memoria de 32 bytes.

Esto para trabajar con estas estructuras ya definidas en C:

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct MemoryBlock {
    void* start_address;       // Dirección de inicio del bloque actual
    size_t offset;             // Tamaño del bloque
    int is_free;               // Estado del bloque (libre u ocupado)
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
        printf("Bloque #%d\n", index);
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

*/