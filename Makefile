######################################################################
# Declare some Makefile variables
######################################################################
PROJECT_NAME = 2dgameengine
CC = g++
LANG_STD = -std=c++17
OBJECT_NAME = $(PROJECT_NAME).a
COMPILER_FLAGS = -Wall -Wfatal-errors -o $(OBJECT_NAME)
SRC_FILES = $(shell find src/ -name "*.cpp")
INCLUDE_PATH = -I"./libs"
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.3

######################################################################
# Declare some Makefile rules
######################################################################
build:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATH) $(LANG_STD) $(SRC_FILES) $(LINKER_FLAGS)
run:
	./$(OBJECT_NAME)
cleanup:
	rm ./$(OBJECT_NAME)
