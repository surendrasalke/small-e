all: main

main:	smalle.o main.o PPMReader.o 
	g++ -lglut -lGLU -lGL -o main smalle.o main.o PPMReader.o

main.o: main.cpp smalle.cpp smalle.hpp PPMReader.cpp
	g++ -lglut -lGLU -lGL -c main.cpp 

smalle.o: smalle.cpp smalle.hpp
	g++ -lglut -lGLU -lGL -c smalle.cpp

PPMReader.o: PPMReader.cpp PPMImageIO.h 
	g++ -lglut -lGLU -lGL -c PPMReader.cpp

clean: 
	rm *.o
