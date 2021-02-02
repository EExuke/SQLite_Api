# ==========================================
# © 2020 EExuke. All Rights Reserved.
# ==========================================

DIR_OBJ = .
DIR_SRC = .
INC_SRC = .

SRC = $(wildcard ${DIR_SRC}/*.cpp)
OBJ = $(patsubst %.cpp, ${DIR_OBJ}/%.o, $(notdir $(SRC)))

BIN_TARGET = sqlite_test

CC = g++
CFLAGS =

${BIN_TARGET}: ${OBJ}
	$(warning Building $(BIN_TARGET) ...)
	@$(CC) $(OBJ) -o $@ -lsqlite3

${DIR_OBJ}/%.o: ${DIR_SRC}/%.cpp
	$(warning Comping $< ...)
	@$(CC) $(CFLAGS) -c $< -o $@

# Prevent naming conflicts such as the existence of a file named clean.
.PHONY: clean

clean:
	@find ${DIR_OBJ} -name *.o -exec rm -rf {} \;
	@rm -rf $(BIN_TARGET)

