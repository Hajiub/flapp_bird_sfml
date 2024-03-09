CC = c++
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
SRC = main.cpp
EXEC = flappy

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC)

run:
	./$(EXEC)
