SOURCES=gofish.c player.c deck.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=gofish
CFLAGS=-Wall
all: $(EXECUTABLE)

$(EXECUTABLE):$(OBJECTS)
	@echo EXE: the rulename is $@ and the first dependency is $<
	gcc $(CFLAGS) -o $@ $(OBJECTS)

$(OBJECTS):%.o:%.c
	@echo OBJ: the rulename is $@ and the first dependency is $<
	gcc $(CFLAGS) -c $<

debug:CFLAGS += -g
debug:$(OBJECTS)
	@echo EXE: the rulename is $@ and the first dependency is $<
	gcc $(CFLAGS) -o $@ $(OBJECTS)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)
