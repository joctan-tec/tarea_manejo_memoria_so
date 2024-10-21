# Simulación de Administración de Memoria en C 🖥️💾

Este proyecto implementa un programa en C que simula la gestión dinámica de memoria utilizando las funciones `malloc`, `calloc`, `realloc` y `free`. Además, se implementan tres algoritmos de asignación de memoria: **Best-fit**, **First-fit** y **Worst-fit**, demostrando problemas como fragmentación de memoria y fugas de memoria.

## Descripción 📄

El objetivo de este proyecto es simular la gestión de la memoria dinámica a partir de un archivo de entrada que contiene una secuencia de operaciones de memoria. Entre las operaciones soportadas están:

- **ALLOC <variable_nombre> <tamaño>**: Asigna un bloque de memoria de `<tamaño>` bytes y lo asocia a `<variable_nombre>`.
- **REALLOC <variable_nombre> <nuevo_tamaño>**: Reasigna el bloque de memoria de `<variable_nombre>` a un nuevo tamaño.
- **FREE <variable_nombre>**: Libera el bloque de memoria asociado a `<variable_nombre>`.
- **PRINT**: Muestra el estado actual de las asignaciones de memoria.
- Las líneas que comienzan con `#` son comentarios y deben ser ignoradas.

El programa debe demostrar los siguientes escenarios:

- **Fragmentación de memoria**.
- **Fugas de memoria**.

## Requisitos del Programa ✅

1. **Lectura del archivo de entrada**: El programa acepta un archivo como argumento y ejecuta las operaciones especificadas.
2. **Gestión de memoria**:
   - Asignación dinámica de memoria con `malloc` o `calloc`.
   - Reasignación de memoria con `realloc`.
   - Liberación de memoria con `free`.
3. **Simulación de escenarios**:
   - Fragmentación de memoria.
   - Fugas de memoria.

## Algoritmos de Asignación de Memoria 🔄

- **Best-fit**: Selecciona el bloque más pequeño que sea suficientemente grande para satisfacer la solicitud.
- **First-fit**: Selecciona el primer bloque que sea suficientemente grande para la solicitud.
- **Worst-fit**: Selecciona el bloque más grande disponible.

## Ejemplo de Archivo de Entrada 📂

```txt
# Simulación de fragmentación
ALLOC A 100
ALLOC B 200
FREE A
ALLOC C 50
PRINT

# Simulación de fuga de memoria
ALLOC D 300
ALLOC E 400
# No liberamos D
FREE E
PRINT
```

## Compilación y Ejecución ⚙️

1. **Compilación del programa**:

   Para compilar el proyecto, ejecuta el siguiente comando en la terminal desde la carpeta raíz del proyecto:
   ```bash
   make
   ```

   Esto compilará el archivo principal `main.c` y generará el ejecutable `memoria`.

2. **Ejecución del programa con parámetros por defecto**:

   Para ejecutar el programa con los valores predeterminados (archivo de entrada `input/input.mem` y el algoritmo `first_fit`), usa:
   ```bash
   make run
   ```

   También puedes especificar el algoritmo directamente:
   ```bash
   make run algoritmo=<algoritmo>
   ```
   Donde `<algoritmo>` puede ser:
   - `first_fit` (predeterminado)
   - `best_fit`
   - `worst_fit`

3. **Ejecución de pruebas**:

   Para ejecutar una prueba predeterminada, utiliza:
   ```bash
   make test
   ```
   Esto ejecutará el programa con el archivo de prueba por defecto (`input/input.mem`).

   Si deseas especificar un archivo de prueba y un algoritmo:
   ```bash
   make test file=<nombre_prueba.mem> algoritmo=<algoritmo>
   ```
   Donde `<nombre_prueba.mem>` es uno de los archivos de prueba ubicados en la carpeta `tests/`, y `<algoritmo>` puede ser `first_fit`, `best_fit`, o `worst_fit`.

4. **Limpieza**:

   Para eliminar los ejecutables y los archivos generados durante la ejecución, utiliza:
   ```bash
   make clean
   ```

   Este comando eliminará los archivos ejecutables y los resultados temporales.


## Video de Ejecución 🎥🚧🚧🚧

## Estructura del Código 🧱

El proyecto está organizado de la siguiente manera:

```
├── a.json
├── algoritmos
├── input
│   ├── input.mem
│   └── README.md
├── LICENSE
├── Makefile
├── mem
│   ├── CHANGELOG.md
│   ├── language-configuration.json
│   ├── package.json
│   ├── README.md
│   ├── syntaxes
│   │   └── mem.tmLanguage.json
│   └── vsc-extension-quickstart.md
├── memoria
├── README.md
├── result_main.txt
├── result_test_goodResult.mem.txt
├── setup.sh
├── src
│   ├── algoritmos.c
│   ├── file_reader.h
│   ├── hashmap.h
│   ├── main
│   ├── main.c
│   ├── memory_manager.c
│   ├── memory_manager.h
│   └── pruebas.c
└── tests
    ├── goodResult.mem
    ├── memory_lack.mem
    ├── no_sufficient_memory.mem
    ├── not_enough_contiguous_space.mem
    ├── README.md
    └── realloc_to_larger_size.mem
```

- **input/**: Archivos de entrada utilizados para pruebas.
- **mem/**: Archivos de configuración y documentación para extensiones.
- **src/**: Contiene el código fuente principal y archivos de implementación de algoritmos.
- **tests/**: Archivos de prueba para simular distintos escenarios de administración de memoria.
- **Makefile**: Script para compilar y ejecutar el proyecto.
- **result_main.txt** y **result_test_goodResult.mem.txt**: Resultados de la ejecución de las pruebas.



---


>👩‍💻 Brenda Badilla Rodriguez  
>👨‍💻 Isaac Brenes Torres  
>👨‍💻 Joctan  Porras Esquivel  
