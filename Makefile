all:
	compile link

compile:
	g++ -I src/include -c mandelbrot.cpp

link:
	g++ mandelbrot.o -o mandelbrot -L src/lib -l sfml-graphics -l sfml-window -l sfml-system
