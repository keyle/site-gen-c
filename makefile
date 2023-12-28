COMPILER = clang
APP = site-gen-c
SOURCE_LIBS = -Ilibs/ -Llibs/
WARNINGS = -Wall -Wextra
DEBUG_BUILD = -ggdb -v -std=c2x
LEAKS_BUILD  = -fsanitize=address
RELEASE_FLAGS = -std=c2x -O3
CFILES = $(wildcard *.c)
# Discover all .c files in the libs directory
LIB_CFILES = $(wildcard libs/*.c)

# Create object files list from CFILES and LIB_CFILES
OBJ = $(patsubst %.c, %.o, $(CFILES)) $(patsubst libs/%.c, libs/%.o, $(LIB_CFILES))

export ASAN_OPTIONS := allocator_may_return_null=1
# Rule for compiling library .c files into .o files
libs/%.o: libs/%.c
	$(COMPILER) $(OBJ_FLAGS)  -c $< -o $@


$(APP): OBJ_FLAGS = -O2 # set those flags on the libs included, we
$(APP): DEBUG:=1
$(APP): $(OBJ)
	$(COMPILER) $^ -o $@ $(SOURCE_LIBS) $(DEBUG_BUILD) $(WARNINGS)
	echo "\n"
	./$(APP)

#leaks: OBJ_FLAGS = -ggdb
leaks: export ASAN_OPTIONS := allocator_may_return_null=1 # for debug -fsanitize=address
leaks: $(OBJ)
	$(COMPILER) $^ -o $(APP)-$@ $(SOURCE_LIBS) $(DEBUG_BUILD) $(WARNINGS) $(LEAKS_BUILD)


# NOTE: make clean first
release: OBJ_FLAGS = -O3
release: $(OBJ)
	$(COMPILER) $^ -o $(APP)-$@ $(RELEASE_FLAGS) $(SOURCE_LIBS)

clean:
	rm -rf $(APP).* $(APP)* **/*.o *.o
