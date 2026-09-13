FUNC := g++
copt := -c
OBJ_DIR := ./bin/
FLAGS := -O3 -lm -g -Werror -Wall -Wextra -fopenmp -flto

CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR),$(notdir $(CPP_FILES:.cpp=.obj)))

TEXTURE_CPP_FILES := $(wildcard src/Textures/*.cpp)
TEXTURE_OBJ_FILES := $(addprefix $(OBJ_DIR)Textures/,$(notdir $(TEXTURE_CPP_FILES:.cpp=.obj)))

all:
	cd ./src && make
	$(FUNC) ./main.cpp -o ./main.exe ./src/*.obj ./src/Textures/*.obj $(FLAGS)

opt: export FLAGS := -O3 -lm -g -Werror -ffast-math -fopt-info-vec -fopt-info-vec-missed -march=native
opt:
	cd ./src && make FLAGS='$(FLAGS)'
	$(FUNC) ./main.cpp -o ./main.exe ./src/*.obj ./src/Textures/*.obj $(FLAGS)

clean:
	cd ./src && make clean
	rm -f ./*.exe
	rm -f ./*.obj
