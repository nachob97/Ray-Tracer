all:
	g++ -Wextra -Wall -std=c++14 -g -O2 -Iinclude -c main.cpp -o obj/Release/main.o
	g++ -Wextra -Wall -std=c++14 -g -O2 -Iinclude -c tinyxml2.cpp -o obj/Release/tinyxml2.o
	g++ -LSDL-1.2.15/lib -LFreeImage -o bin/Release/RayTracer obj/Release/main.o obj/Release/tinyxml2.o  -s -static-libstdc++ -static-libgcc
	
rayTracer: Main.o tinyxml2.o
	g++ -Wextra -Wall -g -o rayTracer main.o tinyxml2.o

Main.o: main.cpp Camera.h Color.h Cylinder.h Light.h Object.h Plane.h Ray.h Sphere.h tinyxml2.h Triangle.h Vect.h
	g++ -g -c main.cpp
Camera.o: Camera.h Vect.h
	g++ -g -c Camera.h

tinyxml2.o: tinyxml2.cpp tinyxml2.h 
	g++ -g -c tinyxml2.cpp

clean:
	rm *.o main	