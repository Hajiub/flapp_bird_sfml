CC = g++

CFLAGS = -Wall -Wextra -std=c++11

LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRCDIR = src
INCDIR = include
BINDIR = bin

SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

EXEC = $(BINDIR)/flappy

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -c $< -o $@

clean:
	rm -rf $(BINDIR)

run: $(EXEC)
	$(EXEC)
