#ifndef _CYLINDER_H
#define CYLINDER_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"
#include "Ray.h"

class Cylinder : public Object {
    Vect base;
	Vect center;
	double radius;
	Color* color;
	double coef;

	public:

	Cylinder ();

	Cylinder(Vect, Vect, double, Color*,double);

	// method functions
	Vect getCylCenter () { return center; }
	double getCylRadius () { return radius; }
	Vect getCylBase(){return base;}
	virtual Color* getColor () { return new Color(color->getColorRed(),color->getColorGreen(),color->getColorBlue(),color->getColorRefl(),color->getColorTransp(), color->getColorSpec()); }
    double getCoef(){return coef;}



	virtual Vect getNormalAt(Vect point) {
		// Incido en una base
		double pr=point.getVectX();
		double cx=base.getVectX();
		double pz=point.getVectZ();
		double cz=base.getVectZ();
		double py=point.getVectY();
		double cy=base.getVectY();
		double height= center.magnitude();
	if (pr<cx+radius && pr>cx-radius && pz<cz+radius && pz>cz-radius)
	{
		double epsilon = 0.00000001;
		if (py < cy+height+epsilon && py>cy+height-epsilon){
			return Vect (0,1,0);
		}
		if (py < cy+epsilon && py>cy-epsilon){
			return Vect (0,-1,0);
		}
	}

	// Point is on lateral surface
 	Vect c0 = Vect(cx, py, cz);
 	Vect v = point.vectAdd(c0.negative());
 	v.normalize ();
return v;
	}


double findIntersection(Ray* ray){
	double cx=base.getVectX();
	double cy=base.getVectY();
	double cz=base.getVectZ();
	double rx=ray->getRayOrigin().getVectX();
	double ry=ray->getRayOrigin().getVectY();
	double rz=ray->getRayOrigin().getVectZ();
	double rdx=ray->getRayDirection().getVectX();
	double rdy=ray->getRayDirection().getVectY();
	double rdz=ray->getRayDirection().getVectZ();
    double height= center.magnitude();

	// translate the ray origin
	Vect p0 = Vect(rx-cx, ry-cy, rz-cz);

	// coefficients for the intersection equation
	// got them mathematically intersecting the line equation with the cylinder equation
    double a = rdx*rdx+rdz*rdz;
	double b = rdx*p0.getVectX() +rdz*p0.getVectZ();
	double c = p0.getVectX()*p0.getVectX()+p0.getVectZ()*p0.getVectZ()-radius*radius;

	double delta = b*b - a*c;

	//use epsilon because of computation errors between doubles
	double epsilon = 0.00000001;

	// delta < 0 means no intersections
	if (delta < epsilon)
		return -1;

	// nearest intersection
	double t = (-b - sqrt (delta))/a;

	// t<0 means the intersection is behind the ray origin
	// which we don't want
	if (t<=epsilon)
		return -1;

	double y = p0.getVectY()+t*rdy;

	// check if we intersect one of the bases
	if (y > height+epsilon || y < -epsilon) {
		double dist;
		double b1 = intersect_base (ray, base.vectAdd(center), dist);
		if(b1) t=dist;
		double b2 = intersect_base (ray, base, dist);
		if(b2 && dist>epsilon && t>=dist)
			t=dist;
        if(b1||b2)
            return t;
        else
            return false;
	}

return t;

}

// Calculate intersection with the base having center c
// We do this by calculating the intersection with the plane
// and then checking if the intersection is within the base circle

bool intersect_base (Ray* ray, Vect b, double& t)
{
    //Vect center = b.vectAdd()
    double cx=base.getVectX();
	double cy=base.getVectY();
	double cz=base.getVectZ();
	double rx=ray->getRayOrigin().getVectX();
	double ry=ray->getRayOrigin().getVectY();
	double rz=ray->getRayOrigin().getVectZ();
	double rdx=ray->getRayDirection().getVectX();
	double rdy=ray->getRayDirection().getVectY();
	double rdz=ray->getRayDirection().getVectZ();

	Vect normal = getNormalAt(b);
	Vect p0 = Vect(rx-cx, ry-cy, rz-cz);
	double A = normal.getVectX();
	double B = normal.getVectY();
	double C = normal.getVectZ();
	double D = - (A*(b.getVectX()-cx) +B*(b.getVectY()-cy)+C*(b.getVectZ()-cz));

	if (A*rdx+B*rdy+C*rdz==0)
		return false;

	double dist = - (A*p0.getVectX()+B*p0.getVectY()+C*p0.getVectZ()+D)/(A*rdx+B*rdy+C*rdz);

	double epsilon = 0.00000001;
	if (dist < epsilon)
		return false;

	Vect p;
	double x = p0.getVectX()+dist*rdx;
	double z = p0.getVectZ()+dist*rdz;
	if (x*x+z*z-radius*radius > epsilon)
		return false;

    t=dist;
    return true;
}


};

Cylinder::Cylinder() {
	center = Vect(0,0,0);
	radius = 1.0;
	base = Vect(0,0,0);
	color = new Color(0.5,0.5,0.5, 0,0,0);
	coef = 1;
}

Cylinder::Cylinder(Vect b, Vect cent, double radiusValue, Color* colorValue,double c) {
	base = b;
	center = cent;
	radius = radiusValue;
	color = colorValue;
	coef = c;
}

#endif
