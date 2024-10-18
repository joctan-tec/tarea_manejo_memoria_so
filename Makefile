# Nombre del ejecutable
EXEC=memoria

# Carpeta de fuente
SRC_DIR=src

# Carpeta de pruebas
TEST_DIR=tests

# Carpeta de input por defecto
DEFAULT_INPUT=input/input.mem

# Archivos fuente
SRC_FILES=$(SRC_DIR)/main.c $(SRC_DIR)/memory_manager.c

# Flags de compilación
CFLAGS=-Wall

# Regla principal, compila el ejecutable
all: $(SRC_FILES)
	@echo "Compilando programa..."
	# Compilar el ejecutable
	gcc $(CFLAGS) -o $(EXEC) $(SRC_FILES)

# Ejecutar el programa con un archivo de entrada
run:
	@echo "Ejecutando programa con archivo de entrada: $(DEFAULT_INPUT)"
	# Ejecutar el programa con el archivo de entrada por defecto
	./$(EXEC) $(DEFAULT_INPUT)

# Ejecutar con un archivo de prueba especificado (si se pasa un argumento)
test:

	@echo "Ejecutando prueba con archivo de entrada: $(file)"

	@if [ -z "$(file)" ]; then \
	    echo "No se proporcionó un archivo de prueba. Usando archivo predeterminado: $(DEFAULT_INPUT)"; \
	    make run; \
	else \
	    echo "Ejecutando prueba con archivo: $(file)"; \
	    ./$(EXEC) $(TEST_DIR)/$(file); \
	fi

# Limpiar los archivos generados
clean:

	@echo "Limpiando archivos generados..."
	rm -f $(EXEC)