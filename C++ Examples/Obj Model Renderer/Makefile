.SUFFIXES: .c .o .cpp .cc .cxx .C

UNAME = $(shell uname)
PLATFORM = $(shell uname -p)

CC = g++
#COPTS = -g -Wall
#COPTS = -g -Wno-deprecated-declarations -std=c++11
COPTS = -g -std=c++11 -DGL_GLEXT_PROTOTYPES

ifeq ("$(shell uname)", "Linux")
INC =
LIBDIR = \
  -L/usr/lib -L/usr/X11R6/lib
LIBS = \
  -lpthread -lglfw -lGLU -lGL -lGLEW -lXmu -lXi -lXext -lX11 -lm
# what learnopengl.com suggests
#LIBS = \
#  -lglfw3 -lGL -lX11 -lpthread -lXrandr -lXi -ldl
else
ifeq ("$(shell uname)", "Darwin")
INC = \
 -I/opt/local/include
LIBDIR = \
  -L/opt/local/lib
LIBS = \
  -framework OpenGL -framework Foundation -lstdc++ -lglfw
endif
endif

.c.o:
				gcc -c $(INC) -g -o $@ $<
#       $(CC) -c $(INC) $(COPTS) -o $@ $<

.cpp.o:
				$(CC) -c $(INC) $(COPTS) -o $@ $<

all: main

OBJECTS = \
shader.o \
aw_t.o

main: $(OBJECTS) main.o
				$(CC) -o $@ $(INC) $(COPTS) $(OBJECTS) $@.o $(LIBDIR) $(LIBS)

main.o: main.cpp
shader.o: shader.h shader.cpp
aw_t.o: aw_t.h aw_t.cpp

clean:
				rm -f *.o
				rm -rf main
