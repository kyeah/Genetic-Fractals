###########################################################
# Aesthetic Fractals: Makefile

CC = g++
CFLAGS = -g -std=c++0x  # Use -std=c++0x for versions of gcc and g++ <4.7, c++11 otherwise
INCLUDE = -Ilibs/AntTweakBar/include
LIBDIR = -Llibs/AntTweakBar/lib
LIBS = -pthread -lpng -lglut -lGLU -lm -lX11 -lGLEW -lGL -lAntTweakBar #-lglfw3 -lX11 -lXi -lXxf86vm -lXrandr

###########################################################
# Options if compiling on Mac
UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
CC = g++
CFLAGS = -D__MAC__ -std=c++11 -stdlib=libc++ 
INCLUDE = -Ilibs/AntTweakBar/include
LIBDIR = -lpng -lAntTweakBar
LIBS = -framework OpenGL -framework GLUT
endif

###########################################################
# Uncomment the following line if you are using Mesa
#LIBS = -lglut -lMesaGLU -lMesaGL -lm

###########################################################

aesthetics: main.o mainWindow.o fractalEditor.o expression.o expressionParser.o fractal.o fbo.o color.o
	${CC} ${CFLAGS} $(INCLUDE) -o aesthetics main.o mainWindow.o fractalEditor.o expression.o fractal.o expressionParser.o fbo.o color.o ${LIBDIR} ${LIBS}

main.o: main.cpp mainWindow.h expression.h fractal.h common.h
	${CC} -c ${CFLAGS} $(INCLUDE) main.cpp

mainWindow.o: mainWindow.cpp mainWindow.h fractal.h common.h
	${CC} -c ${CFLAGS} $(INCLUDE) mainWindow.cpp

fractalEditor.o: fractalEditor.cpp fractalEditor.h fractal.h common.h tw.h
	${CC} -c ${CFLAGS} $(INCLUDE) fractalEditor.cpp

expression.o: expression.cpp expression.h expressionParser.h
	${CC} -c ${CFLAGS} $(INCLUDE) expression.cpp

fractal.o: fractal.cpp fractal.h vec.h types.h common.h rng.h tw.h
	${CC} -c ${CFLAGS} $(INCLUDE) fractal.cpp

expressionParser.o: expressionParser.cpp expressionParser.h
	${CC} -c ${CFLAGS} $(INCLUDE) expressionParser.cpp

fbo.o: fbo.cpp fbo.h common.h
	${CC} -c ${CFLAGS} $(INCLUDE) fbo.cpp

color.o: color.cpp common.h
	${CC} -c ${CFLAGS} $(INCLUDE) color.cpp

clean:
	rm -f aesthetics *.o *~ *# *.gch
