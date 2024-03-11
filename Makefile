CC = c++
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
SRC = main.cpp ./src/bird.cpp ./src/pipe.cpp
EXEC = flappy

all:
	$(CC) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC)

run:
	./$(EXEC)
