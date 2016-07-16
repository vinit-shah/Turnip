OBJS = PUT SOURCE FILES HERE

CC = g++ -std=c++11

COMPILER_FLAGS = -w -g

LINKER_FLAGS = -I/usr/local/include -L/usr/local/lib -lSDL2main -lSDL2 -lGLEW
FRAME_WORKS = -framework GLUT -framework OpenGL -framework Cocoa

OBJ_NAMES = Turnip

all: $(OBJS)
	$(CC) $(OBJS)  $(COMPILER_FLAGS) $(LINKER_FLAGS) $(FRAME_WORKS) -o $(OBJ_NAMES)
