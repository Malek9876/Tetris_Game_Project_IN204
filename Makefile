all: compile
    ./TETRIC.out

compile: clean
    @g++ -Wall -O3 main.cpp display.cpp logic.cpp tetromino.cpp pixel.cpp client.cpp server.cpp network_declarations.cpp -o TETRIC.out -lGL -lGLU -lglfw -lGLEW -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lpthread

clean:
    @rm -rf *.o TETRIC.out