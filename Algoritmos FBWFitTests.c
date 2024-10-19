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
