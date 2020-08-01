rayTracer: Main.o tinyxml2.o
	mpic++ -Wextra -Wall -g -o rayTracer main.o tinyxml2.o

Main.o: main.cpp Camera.h Color.h Cylinder.h Light.h Object.h Plane.h Ray.h Sphere.h tinyxml2.h Triangle.h Vect.h
	mpic++ -g -c main.cpp

tinyxml2.o: tinyxml2.cpp tinyxml2.h 
	mpic++ -g -c tinyxml2.cpp

MainMPI.o: mainMPI.cpp Camera.h Color.h Cylinder.h Light.h Object.h Plane.h Ray.h Sphere.h tinyxml2.h Triangle.h Vect.h
	mpic++ -g -c mainMPI.cpp

rayTracerMPI: MainMPI.o tinyxml2.o
	mpic++ -Wextra -Wall -g -o rayTracerMPI mainMPI.o tinyxml2.o

clean:
	rm *.o main	
