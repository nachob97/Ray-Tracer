all:
	g++ -Wextra -Wall -std=c++14 -g -fno-tree-vectorize -Iinclude -c main.cpp -o obj/Release/main.o
	g++ -Wextra -Wall -std=c++14 -g -fno-tree-vectorize -Iinclude -c tinyxml2.cpp -o obj/Release/tinyxml2.o
	g++ -LSDL-1.2.15/lib -LFreeImage -o bin/Release/RayTracer obj/Release/main.o obj/Release/tinyxml2.o  -s -static-libstdc++ -static-libgcc
	
rayTracer: Main.o tinyxml2.o
	g++ -Wextra -Wall -g -o rayTracer main.o tinyxml2.o

Main.o: main.cpp Camera.h Color.h Cylinder.h Light.h Object.h Plane.h Ray.h Sphere.h tinyxml2.h Triangle.h Vect.h
	g++ -g -c main.cpp

tinyxml2.o: tinyxml2.cpp tinyxml2.h 
	g++ -g -c tinyxml2.cpp

MainMPI.o: mainMPI.cpp Camera.h Color.h Cylinder.h Light.h Object.h Plane.h Ray.h Sphere.h tinyxml2.h Triangle.h Vect.h
	mpic++ -g -c mainMPI.cpp

rayTracerMPI: MainMPI.o tinyxml2.o
	mpic++ -Wextra -Wall -g -o rayTracerMPI mainMPI.o tinyxml2.o

clean:
	rm *.o main	