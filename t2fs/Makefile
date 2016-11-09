#
# Makefile ESQUELETO
#
# DEVE ter uma regra "all" para geração da biblioteca
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: disk_interface_compile file_interface_compile lib_interface_compile
	ar rvs $(LIB_DIR)/libt2fs.a $(BIN_DIR)/*.o

disk_interface_compile:
	$(CC) -o $(BIN_DIR)/disk_interface.o $(SRC_DIR)/disk_interface.c -Wall -c

file_interface_compile:
	$(CC) -o $(BIN_DIR)/file_interface.o $(SRC_DIR)/file_interface.c -Wall -c

lib_interface_compile:
	$(CC) -o $(BIN_DIR)/lib_interface.o $(SRC_DIR)/lib_interface.c -Wall -c

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
