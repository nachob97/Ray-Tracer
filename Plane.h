#ifndef _Plane_H
#define _Plane_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Plane : public Object {
	Vect normal;
	double distance;
	Color* color;

	public:

	Plane ();

	Plane (Vect, double, Color*);

	// method functions
	Vect getPlaneNormal () { return normal; }
	double getPlaneDistance () { return distance; }
	virtual Color* getColor () { return  new Color(color->getColorRed(),color->getColorGreen(),color->getColorBlue(),color->getColorRefl(),color->getColorTransp(), color->getColorSpec()); }
     double getCoef(){return 0;}

	virtual Vect getNormalAt(Vect point) {
		return normal;
	}

	virtual double findIntersection(Ray* ray) {
		Vect ray_direction = ray->getRayDirection();

		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			// rayo paralelo al plano
			return -1;
		}
		else {
			double b = normal.dotProduct(ray->getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			return (b > 0.00001 ? -1*b/a : -1);
		}
	}

};

Plane::Plane () {
	normal = Vect(1,0,0);
	distance = 0;
	color = new Color(0.8,0.8,0.8,0,0,0);
}

Plane::Plane (Vect normalValue, double distanceValue, Color* colorValue) {
	normal = normalValue;
	distance = distanceValue;
	color = colorValue;
}

#endif
