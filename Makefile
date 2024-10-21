# Nombre del ejecutable
EXEC=memoria

PRUEBAS=pruebas

# Carpeta de fuente
SRC_DIR=src

# Carpeta de pruebas
TEST_DIR=tests

# Carpeta de input por defecto
DEFAULT_INPUT=input/input.mem

# archivos de prueba
TEST_FILES=$(SRC_DIR)/pruebas.c

# Archivos fuente
SRC_FILES=$(SRC_DIR)/main.c $(SRC_DIR)/memory_manager.c

# Algortimo de default
algoritmo = "first_fit"

# Flags de compilación
CFLAGS=-Wall

# Regla principal, compila el ejecutable
all: $(SRC_FILES)
	@echo "Compilando programa..."
	# Compilar el ejecutable
	gcc $(CFLAGS) -o $(EXEC) $(SRC_FILES)

# Ejecutar el programa con un archivo de entrada
run:
	@echo "Ejecutando programa con archivo de entrada: $(DEFAULT_INPUT) con algoritmo: $(algoritmo)"
	# Ejecutar el programa con el archivo de entrada por defecto
	gcc $(CFLAGS) -o $(EXEC) $(SRC_FILES)
	./$(EXEC) $(DEFAULT_INPUT) $(algoritmo) > result_main.txt

# Ejecutar con un archivo de prueba especificado (si se pasa un argumento)
test:
	@echo "Ejecutando prueba con archivo de entrada: $(file) con algoritmo: $(algoritmo)"
	gcc $(CFLAGS) -o $(EXEC) $(SRC_FILES)
	@if [ -z "$(file)" ]; then \
	    echo "No se proporcionó un archivo de prueba. Usando archivo predeterminado: $(DEFAULT_INPUT)"; \
	    make run; \
	else \
	    echo "Ejecutando prueba con archivo: $(file)"; \
	    ./$(EXEC) $(TEST_DIR)/$(file) $(algoritmo) > result_test_$(file).txt ; \
	fi

# Limpiar los archivos generados
clean:

	@echo "Limpiando archivos generados..."
	rm -f $(EXEC)
	rm -f $(PRUEBAS)
	rm -f result_temp.txt result_main.txt result_test_*.txt


temp:
	@echo "Ejecutando programa con archivo de entrada: $(TEST_FILES)"
	gcc $(CFLAGS) -o $(PRUEBAS) $(TEST_FILES)
	# Ejecutar el programa con el archivo de entrada por defecto
	./$(PRUEBAS) > result_temp.txt