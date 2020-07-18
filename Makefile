all:
	g++ -Wextra -Wall -std=c++14 -g -O2 -Iinclude -c main.cpp -o obj/Release/main.o
	g++ -Wextra -Wall -std=c++14 -g -O2 -Iinclude -c tinyxml2.cpp -o obj/Release/tinyxml2.o
	g++ -LSDL-1.2.15/lib -LFreeImage -o bin/Release/RayTracer.exe obj/Release/main.o obj/Release/tinyxml2.o  -s -static-libstdc++ -static-libgcc
	
