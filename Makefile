CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lgmp

TARGET_GEN = poly1305-gen
TARGET_CHECK = poly1305-check
SRC = poly1305-gen.c poly1305-check.c utils.c
OBJ = $(SRC:.c=.o)

all: $(TARGET_GEN) $(TARGET_CHECK)

gen: $(TARGET_GEN)

check: $(TARGET_CHECK)

$(TARGET_GEN): poly1305-gen.o utils.o
	$(CC) $(CFLAGS) -o $@ poly1305-gen.o utils.o $(LDFLAGS)

$(TARGET_CHECK): poly1305-check.o utils.o 
	$(CC) $(CFLAGS) -o $@ poly1305-check.o utils.o $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(TARGET_GEN) $(TARGET_CHECK) $(OBJ)
