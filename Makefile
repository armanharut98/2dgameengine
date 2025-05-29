build:
	g++ -Wall -std=c++17 -I"./libs" src/*.cpp -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -llua5.3 -o gameengine

run:
	./gameengine

cleanup:
	rm ./gameengine
