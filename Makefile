CC = clang
CFLAGS = -Wall -Wextra -std=c99 -g
LFLAGS = 
LIBS = -lm -pthread
SOURCES = lab3.c 
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = lab3
.PHONY: clean help

lab3: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "  clean:  removes .o and binary files"
