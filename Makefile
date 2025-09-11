CC := gcc
CFLAGS := -std=c99 -Wall -Wextra -O2 -MMD -MP -Iinclude -lcurl
SRC := $(wildcard src/*.c)
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))
DEP := $(OBJ:.o=.d)
BIN := build/app
all: $(BIN)

$(BIN): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@ -lcurl
build/%.o: src/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@ -MF $(basename $@).d -MT $@
clean:
	$(RM) -r build

run : $(BIN)
	./$(BIN)
# target: dependencies
#    commands


# final: $(OBJS)
# 	$(CC) $(FLAGS) $(OBJS) -o final -lcurl

# main.o: main.c
# 	$(CC) $(FLAGS) -c main.c -o main.o

# weather.o: weather.c
# 	$(CC) $(FLAGS) -c weather.c -o weather.o

# clean:
# 	rm $(OBJS) final

-include $(DEP)
.PHONY: all clean