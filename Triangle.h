#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "math.h"
#include "Object.h"
#include "Vect.h"
#include "Color.h"

class Triangle : public Object {
   	Vect A;
   	Vect B;
   	Vect C;
	Color* color;
	double coef;

public:
	Triangle (Vect, Vect,Vect, Color*,double);
	Vect getP1(){return A;}
	Vect getP2(){return B;}
	Vect getP3(){return C;}
	Color* getColor(){return color;}
	double getCoef(){return coef;}
	Vect getNormal();
	double getTriangleDistance();
	virtual Vect getNormalAt(Vect point) {return getNormal();}

    virtual double findIntersection(Ray* ray) {
		Vect ray_direction = ray->getRayDirection();
		Vect ray_o = ray->getRayOrigin();
        Vect normal = getNormal();
        double distance = getTriangleDistance();
		double a = ray_direction.dotProduct(normal);

		if (a == 0) {
			// Rayo paralelo al triangulo
			return -1;
		}
		else {
			double b = normal.dotProduct(ray->getRayOrigin().vectAdd(normal.vectMult(distance).negative()));
			double distancePlane = -1*b/a;

			double Qx = ray_direction.vectMult(distancePlane).getVectX() + ray_o.getVectX();
			double Qy = ray_direction.vectMult(distancePlane).getVectY() + ray_o.getVectY();
			double Qz = ray_direction.vectMult(distancePlane).getVectZ() + ray_o.getVectZ();
			Vect Q = Vect(Qx,Qy,Qz); //punto interseccion

            //[CAxQA]*n >= 0
            Vect CA = Vect(C.getVectX() - A.getVectX(),C.getVectY() - A.getVectY(),C.getVectZ() - A.getVectZ());
            Vect QA = Vect (Q.getVectX() - A.getVectX(),Q.getVectY() - A.getVectY(),Q.getVectZ() - A.getVectZ());
			double cond1 = (CA.crossProduct(QA).dotProduct(normal));
			//[BCxQC]*n >= 0
            Vect BC = Vect (B.getVectX() - C.getVectX(),B.getVectY() - C.getVectY(),B.getVectZ() - C.getVectZ());
            Vect QC = Vect (Q.getVectX() - C.getVectX(),Q.getVectY() - C.getVectY(),Q.getVectZ() - C.getVectZ());
			double cond2 = (BC.crossProduct(QC).dotProduct(normal));
			//[ABxQB]*n >= 0
            Vect AB = Vect (A.getVectX() - B.getVectX(),A.getVectY() - B.getVectY(),A.getVectZ() - B.getVectZ());
            Vect QB = Vect (Q.getVectX() - B.getVectX(),Q.getVectY() - B.getVectY(),Q.getVectZ() - B.getVectZ());
			double cond3 = (AB.crossProduct(QB).dotProduct(normal));

			if( (cond1 >= 0)&&(cond2>=0)&&(cond3>=0) ){ //dentro del triangulo
                return -1*b/a; //(b > 0.00001 ? -1*b/a : -1);
			}else return -1;
		}
	}


};


Triangle::Triangle (Vect p1, Vect p2,Vect p3, Color* colorValue,double c) {
    this->A = p1;
    this->B = p2;
    this->C = p3;
    this->color = colorValue;
    this->coef = c;
}

Vect Triangle::getNormal(){
    Vect CA = Vect (C.getVectX() - A.getVectX(),C.getVectY() - A.getVectY(),C.getVectZ() - A.getVectZ());
    Vect BA = Vect (B.getVectX() - A.getVectX(),B.getVectY() - A.getVectY(),B.getVectZ() - A.getVectZ());
    return CA.crossProduct(BA).normalize();
}

double Triangle::getTriangleDistance (){
    return (getNormal().dotProduct(A));
}

#endif
