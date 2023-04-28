CC = g++

BREW = $(shell brew --prefix)

CFLAGS = -g -std=c++11 -Wno-deprecated-declarations
INCFLAGS = -Iinclude -I$(BREW)/include
LDFLAGS = -framework OpenGL -L$(BREW)/lib -lglfw

RM = /bin/rm -f
all: menv
menv: main.o Camera.o Cube.o Shader.o Tokenizer.o Joint.o Skeleton.o Skin.o Channel.o Animation.o Player.o Window.o
	$(CC) -o menv main.o Camera.o Cube.o Shader.o Tokenizer.o Joint.o Skeleton.o Skin.o Channel.o Animation.o Player.o Window.o $(LDFLAGS)
main.o: main.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Camera.o: src/Camera.cpp include/Camera.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Camera.cpp
Cube.o: src/Cube.cpp include/Cube.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Cube.cpp
Shader.o: src/Shader.cpp include/Shader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Shader.cpp
Tokenizer.o: src/Tokenizer.cpp include/Tokenizer.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Tokenizer.cpp
Joint.o:  src/Joint.cpp include/DOF.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Joint.cpp
Skeleton.o:  src/Skeleton.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Skeleton.cpp
Skin.o: src/Skin.cpp include/Skin.h include/Triangle.h include/Vertex.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Skin.cpp
Channel.o: src/Channel.cpp include/Channel.h include/Keyframe.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Channel.cpp
Animation.o: src/Animation.cpp include/Animation.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Animation.cpp
Player.o: src/Player.cpp include/Player.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Player.cpp
Window.o: src/Window.cpp include/Window.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c src/Window.cpp


clean: 
	$(RM) *.o menv
