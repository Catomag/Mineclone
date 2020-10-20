SOURCES = $(wildcard ./src/*.c)
SRC = $(addprefix src/, $(SOURCES))
OBJ = $(addsuffix .o, $(addprefix bin/, $(basename $(notdir $(SRC)))));
INCLUDE = -I include
CFLAGS = -W -O2# -D_DEBUG_

all: mineclone

again: clean mineclone

mineclone: $(OBJ)
	echo $(SOURCES)
	gcc -W $^ -lGL deps/glad.o -lglfw -lpthread -lm -ldl -o $@

bin/%.o : src/%.c
	gcc $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f bin/*
	rm mineclone

install:
	echo "Can't install surry"

try: mineclone
	./mineclone

run:
	./mineclone
