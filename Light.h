#ifndef _Light_H
#define _Light_H

#include "Vect.h"
#include "Color.h"

class Light {
	Vect position;
	Color* color;

	public:

	Light ();

	Light (Vect, Color*);

	// method functions
	 Vect getLightPosition () { return position; }
	Color* getLightColor () { return color; }

};

Light::Light () {
	position = Vect(0,0,0);
	color = new Color(1,1,1,0,0,0);
}

Light::Light (Vect p, Color* c) {
	position = p;
	color = c;
}

#endif
