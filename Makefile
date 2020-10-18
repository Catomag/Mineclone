SOURCES = main.c batch2d.c loadwav.c
SRC = $(addprefix src/, $(SOURCES))
OBJ = $(addsuffix .lib, $(addprefix bin/, $(basename $(notdir $(SRC)))));
INCLUDE = -I include -I deps/include
CFLAGS = -W -O2# -D_DEBUG_

all: skelehunt

again: clean skelehunt

skelehunt: $(OBJ)
	# windows
	x86_64-w64-mingw32-gcc -L deps/bin $^ -lOpenAL32 -lopengl32 --static -lglfw3 -lglad -lm -lpthread -o $@
	# linux (assuming all libraries are installed in the system
	#gcc -W $^ -lGL deps/bin/glad.o -lGL -lopenal -lglfw -lpthread -lm -ldl -o $@

bin/%.lib : src/%.c
	x86_64-w64-mingw32-gcc $(INCLUDE) $(CFLAGS) -c $< -o $@
	# linux
	#gcc $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -f bin/*
	rm skelehunt

install:
	echo "Can't install surry"

try: skelehunt
	./skelehunt

run:
	./skelehunt
