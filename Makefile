SOURCES = $(wildcard ./src/*.c)
SRC = $(addprefix src/, $(SOURCES))
OBJ = $(addsuffix .o, $(addprefix bin/, $(basename $(notdir $(SRC)))));
INCLUDE = -I include
CFLAGS = -W -O0 -D_DEBUG_ -mrecip -ffast-math -std=c99#-O2

all: mineclone

again: clean mineclone

mineclone: $(OBJ)
	echo $(SOURCES)
	clang -W $^ -lGL deps/glad.o -lglfw -lpthread -lm -ldl -o $@

bin/%.o : src/%.c
	clang $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f bin/*
	rm mineclone

install:
	echo "Can't install surry"

try: mineclone
	./mineclone

run:
	./mineclone
