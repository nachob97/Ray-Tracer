#ifndef _VECT_H
#define _VECT_H

#include "math.h"

class Vect {
	double x, y, z;

	public:

	Vect ();

	Vect (double, double, double);

	// method functions
	double getVectX() { return x; }
	double getVectY() { return y; }
	double getVectZ() { return z; }

	double magnitude () {
		return sqrt((x*x) + (y*y) + (z*z));
	}

	Vect normalize () {
		double magnitude = sqrt((x*x) + (y*y) + (z*z));
		return Vect (x/magnitude, y/magnitude, z/magnitude);
	}

	Vect negative () {
		return Vect (-x, -y, -z);
	}

	double dotProduct(Vect v) {
		return x*v.getVectX() + y*v.getVectY() + z*v.getVectZ();
	}

	Vect crossProduct(Vect v) {
		return Vect (y*v.getVectZ() - z*v.getVectY(), z*v.getVectX() - x*v.getVectZ(), x*v.getVectY() - y*v.getVectX());
	}

	Vect vectAdd (Vect v) {
		return Vect (x + v.getVectX(), y + v.getVectY(), z + v.getVectZ());
	}

	Vect vectMult (double scalar) {
		return Vect (x*scalar, y*scalar, z*scalar);
	}

	double vectAng(Vect v){
    double dot = x*v.x + y*v.y + z*v.z ;
    double lenSq1 = x*x + y*y + z*z;
    double lenSq2 = v.x*v.x + v.y*v.y + v.z*v.z;
    double angle = acos(dot/sqrt(lenSq1 * lenSq2));
    return angle;
	}

	Vect vectRot(Vect k, Vect v,double ang){
        double cang = cos(ang);
        double sang = sin(ang);
        Vect res = v.vectMult(cang).vectAdd(k.crossProduct(v).vectMult(sang)).vectAdd(k.vectMult(k.dotProduct(v))).vectMult(1-cang);
        return res;
	}
};

Vect::Vect () {
	x = 0;
	y = 0;
	z = 0;
}

Vect::Vect (double i, double j, double k) {
	x = i;
	y = j;
	z = k;
}

#endif
