#ifndef _OBJECT_H
#define _OBJECT_H

#include "Ray.h"
#include "Vect.h"
#include "Color.h"

class Object {
	public:

	Object ();

	// method functions
	virtual Color* getColor () = 0;

	virtual Vect getNormalAt(Vect intersection_position) = 0;

	virtual double findIntersection(Ray* ray) = 0;

	virtual double getCoef()=0;


};

Object::Object () {}

#endif
