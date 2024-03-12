# Compiler
CC = g++

# Compiler flags
CFLAGS = -Wall -Wextra -std=c++11

# Libraries
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Directories
SRCDIR = src
INCDIR = include
BINDIR = bin

# Source files
SRC = $(wildcard $(SRCDIR)/*.cpp)
OBJ = $(SRC:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

# Executable name
EXEC = $(BINDIR)/flappy

# Targets
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
