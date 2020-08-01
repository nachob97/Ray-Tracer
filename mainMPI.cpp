#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <limits>

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>

#include "Vect.h"
#include "Ray.h"
#include "Camera.h"
#include "Color.h"
#include "Light.h"
#include "Object.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Cylinder.h"
#include "tinyxml2.h"
#define MAX_DEPH 5



using namespace std;

struct RGBType {
	double r;
	double g;
	double b;
};

void savebmp (const char *filename, int w, int h, int dpi, RGBType *data) {
	FILE *f;
	int k = w*h;
	int s = 4*k;
	int filesize = 54 + s;

	double factor = 39.375;
	int m = static_cast<int>(factor);

	int ppm = dpi*m;

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

	bmpfileheader[ 2] = (unsigned char)(filesize);
	bmpfileheader[ 3] = (unsigned char)(filesize>>8);
	bmpfileheader[ 4] = (unsigned char)(filesize>>16);
	bmpfileheader[ 5] = (unsigned char)(filesize>>24);

	bmpinfoheader[ 4] = (unsigned char)(w);
	bmpinfoheader[ 5] = (unsigned char)(w>>8);
	bmpinfoheader[ 6] = (unsigned char)(w>>16);
	bmpinfoheader[ 7] = (unsigned char)(w>>24);

	bmpinfoheader[ 8] = (unsigned char)(h);
	bmpinfoheader[ 9] = (unsigned char)(h>>8);
	bmpinfoheader[10] = (unsigned char)(h>>16);
	bmpinfoheader[11] = (unsigned char)(h>>24);

	bmpinfoheader[21] = (unsigned char)(s);
	bmpinfoheader[22] = (unsigned char)(s>>8);
	bmpinfoheader[23] = (unsigned char)(s>>16);
	bmpinfoheader[24] = (unsigned char)(s>>24);

	bmpinfoheader[25] = (unsigned char)(ppm);
	bmpinfoheader[26] = (unsigned char)(ppm>>8);
	bmpinfoheader[27] = (unsigned char)(ppm>>16);
	bmpinfoheader[28] = (unsigned char)(ppm>>24);

	bmpinfoheader[29] = (unsigned char)(ppm);
	bmpinfoheader[30] = (unsigned char)(ppm>>8);
	bmpinfoheader[31] = (unsigned char)(ppm>>16);
	bmpinfoheader[32] = (unsigned char)(ppm>>24);

	f = fopen(filename,"wb");

	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);

	for (int i = 0; i < k; i++) {
		RGBType rgb = data[i];

		double red = (data[i].r)*255;
		double green = (data[i].g)*255;
		double blue = (data[i].b)*255;

		unsigned char color[3] = {(int)floor(blue),(int)floor(green),(int)floor(red)};

		fwrite(color,1,3,f);
	}

	fclose(f);
}

int winningObjectIndex(vector<double> object_intersections) {
	int index_of_minimum_value;
	if (object_intersections.size() == 0) {
		return -1;
	}
	else if (object_intersections.size() == 1) {
		if (object_intersections.at(0) > 0) {
			return 0;
		}
		else {
			return -1;
		}
	}
	else { // Hay mas de una interseccion
        double max = 0;
        // Primero se encuentra el maximo valor
		for (int i = 0; i < object_intersections.size(); i++) {
			if (max < object_intersections.at(i)) {
				max = object_intersections.at(i);
			}
		}

		// Se encuentra el minimo valor
		if (max > 0) {
			for (int index = 0; index < object_intersections.size(); index++) {
				if (object_intersections.at(index) > 0 && object_intersections.at(index) <= max) {
					max = object_intersections.at(index);
					index_of_minimum_value = index;
				}
			}

			return index_of_minimum_value;
		}
		else {
			return -1;
		}
	}
}



Vect refract(Vect normal, Vect incident, double n1, double n2)
{
 double n = n1 / n2;
 double cosI = normal.dotProduct(incident);
 double sinT2 = n * n * (1.0 - cosI * cosI);
if (sinT2 > 1.0)
{
return Vect(-666,-666,-666);
}
return (incident.vectMult(n)).vectAdd(normal.vectMult((-1)*(n + sqrt(1.0 - sinT2))));
}

void makeTable(Vect corner1, Vect corner2,Color* color,vector<Object*> &scene_objects, double coef){
   double c1x = corner1.getVectX();
   double c1y = corner1.getVectY();
   double c1z = corner1.getVectZ();
   double c2x = corner2.getVectX();
   double c2y = corner2.getVectY();
   double c2z = corner2.getVectZ();
//TECHO
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c2z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),Vect(c2x,c2y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c2z),Vect((c2x+c1x)/2,c2y,c2z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c2y,c1z),Vect(c2x,c2y,c1z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c2z),Vect(c1x,c2y,c1z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c2y,c2z),Vect(c1x,c2y,c2z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c1z),Vect((c2x+c1x)/2,c2y,c1z),Vect((c2x+c1x)/2,c2y,(c1z+c2z)/2),color,coef));
//FRENTE
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c1z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect(c2x,c1y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect(c2x,c2y,c1z),Vect((c2x+c1x)/2,c2y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect((c2x+c1x)/2,c1y,c1z),Vect(c2x,c1y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect(c1x,c2y,c1z),Vect(c1x,c1y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c1z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect((c2x+c1x)/2,c2y,c1z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c1y,c1z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c1z),Vect(c1x,c1y,c1z),color,coef));

//FONDO

    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect(c2x,c2y,c2z),Vect(c2x,c1y,c2z),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c2z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect((c2x+c1x)/2,c2y,c2z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c1y,c2z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect(c2x,c1y,c2z),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c2z),Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect(c1x,c1y,c2z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect(c1x,c2y,c2z),Vect((c2x+c1x)/2,c2y,c2z),color,coef));
    scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,(c2y+c1y)/2,c2z),Vect((c2x+c1x)/2,c1y,c2z),Vect(c1x,c1y,c2z),color,coef));
//LADO DERECHO
    scene_objects.push_back(new Triangle(Vect(c2x,c1y,c2z),Vect(c2x,c2y,c2z),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c2z),Vect(c2x,c2y,(c2z+c1z)/2),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c1y,(c2z+c1z)/2),Vect(c2x,c1y,c2z),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,c1z),Vect(c2x,c1y,c1z),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c2y,(c2z+c1z)/2),Vect(c2x,c2y,c1z),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c2x,c1y,c1z),Vect(c2x,c1y,(c2z+c1z)/2),Vect(c2x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
//LADO IZQUIERDO
    scene_objects.push_back(new Triangle(Vect(c1x,c1y,c2z),Vect(c1x,c2y,c2z),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c2z),Vect(c1x,c2y,(c2z+c1z)/2),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c1y,(c2z+c1z)/2),Vect(c1x,c1y,c2z),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,c1z),Vect(c1x,c1y,c1z),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c2y,(c2z+c1z)/2),Vect(c1x,c2y,c1z),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
    scene_objects.push_back(new Triangle(Vect(c1x,c1y,c1z),Vect(c1x,c1y,(c2z+c1z)/2),Vect(c1x,(c1y+c2y)/2,(c2z+c1z)/2),color,coef));
//PISO
    if (c1y != -1){
        scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),Vect(c2x,c1y,c2z),Vect(c2x,c1y,c1z),color,coef));
        scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c1y,c2z),Vect(c2x,c1y,c2z),Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),color,coef));
        scene_objects.push_back(new Triangle(Vect(c2x,c1y,c1z),Vect((c2x+c1x)/2,c1y,c1z),Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),color,coef));
        scene_objects.push_back(new Triangle(Vect(c1x,c1y,c1z),Vect(c1x,c1y,c2z),Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),color,coef));
        scene_objects.push_back(new Triangle(Vect(c1x,c1y,c2z),Vect((c2x+c1x)/2,c1y,c2z),Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),color,coef));
        scene_objects.push_back(new Triangle(Vect((c2x+c1x)/2,c1y,c1z),Vect(c1x,c1y,c1z),Vect((c2x+c1x)/2,c1y,(c1z+c2z)/2),color,coef));
    }

}


Color* getColor(vector<Object*> scene_objects,int index, Ray* ray, Vect point, Vect normal,vector<Light*> light_sources ,int depth,double transp);

Color* trace(Ray* ray,vector<Object*> scene_objects,vector<Light*> light_sources,int depth,double transp){
    vector<double> intersections;
    Color* res = new Color(0,0,0,0,0,0);
    for (int index = 0; index < scene_objects.size(); index++) {
        intersections.push_back(scene_objects.at(index)->findIntersection(ray));
    }
    int index_of_winning_object = winningObjectIndex(intersections);
    if (index_of_winning_object == -1) {
    // Fondo negro
        return res;
    }
    else{
    // El indice corresponde a un objeto
            Vect intersection_position = ray->getRayOrigin().vectAdd(ray->getRayDirection().vectMult(intersections.at(index_of_winning_object)));
            Vect winning_obj_normal = scene_objects.at(index_of_winning_object)->getNormalAt(intersection_position);
            if (depth == 1){
                 res->setColorRefl(scene_objects.at(index_of_winning_object)->getColor()->getColorRefl());
                 res->setColorTransp(scene_objects.at(index_of_winning_object)->getColor()->getColorTransp());
                 Color* color_aux = getColor(scene_objects,index_of_winning_object,ray,intersection_position,winning_obj_normal,light_sources,depth,transp);
                 res = res->colorAdd(color_aux);
                 delete(color_aux);
                 return res;
            }else
                delete(res);
                return getColor(scene_objects,index_of_winning_object,ray,intersection_position,winning_obj_normal,light_sources,depth,transp);

        }

}


Vect displace(Vect dir, Vect point){
    return point.vectAdd(dir.vectMult(0.01));
}

Vect reflect(Ray* ray, Vect normal){
    double dot1 = normal.dotProduct(ray->getRayDirection().negative());
    Vect scalar1 = normal.vectMult(dot1);
    Vect add1 = scalar1.vectAdd(ray->getRayDirection());
    Vect scalar2 = add1.vectMult(2);
    Vect add2 = ray->getRayDirection().negative().vectAdd(scalar2);
    return add2.normalize();
}
float clamp(float in){
    if(in<-1){in = -1;} else{
        if(in>1){
            in=1;
        }
    }
    return in;
}

Vect refract(Vect I, Vect N, float ior)
{
float cosi = clamp(I.dotProduct(N));
float etai = 1, etat = ior;
Vect n = N;
if (cosi < 0) { cosi = (-cosi); } else { std::swap(etai, etat); n= (N.negative()); }
float eta = etai / etat;
float k = 1 - eta * eta * (1 - cosi * cosi);
return k < 0 ? Vect() : I.vectMult(eta).vectAdd(n.vectMult((eta * cosi - sqrtf(k))));
}

Color* getColor(vector<Object*> scene_objects,int index, Ray* ray, Vect point, Vect normal,vector<Light*> light_sources ,int depth,double transp){
     if (depth > MAX_DEPH) {
    return new Color(0,0,0,0,0,0);
    }
    Object* obj = scene_objects.at(index);
    Color* winning_object_color = obj->getColor();
	Color* final_color = winning_object_color->colorScalar(0.2);
    Vect reflection_direction = reflect(ray, normal);
	Color* luz_amb = new Color(0,0,0,0,0,0);
	Color* color_spec = new Color(0,0,0,0,0,0);
for (int light_index = 0; light_index < light_sources.size(); light_index++){
        Vect light_direction = light_sources.at(light_index)->getLightPosition().vectAdd(point.negative());
        light_direction = light_direction.normalize();
        float cosine_angle = obj->getNormalAt(point).dotProduct(light_direction);
        if (cosine_angle > 0) {
            //BUSCO INTERSECCIONES CON LOS OBJETOS Y EL RAYO DE LA SOMBRA
			Vect distance_to_light = light_sources.at(light_index)->getLightPosition().vectAdd(point.negative());
			float distance_to_light_magnitude = distance_to_light.magnitude();
			Ray* shadow_ray = new Ray(point, distance_to_light.normalize());
            vector<double> secondary_intersections;
            double opac = 1.0d;
			for (int object_index = 0; object_index < scene_objects.size(); object_index++) {
                double dist = scene_objects.at(object_index)->findIntersection(shadow_ray);
			    if(scene_objects.at(object_index)->findIntersection(shadow_ray) != -1){
                    if(dist > 0.00001){
                        if(dist <= distance_to_light_magnitude){
                            opac = opac*scene_objects.at(object_index)->getColor()->getColorTransp();
                    }
			    }
            }
			}
            delete (shadow_ray);
            Color* aux_luz_amb = (light_sources.at(light_index)->getLightColor())->colorScalar(cosine_angle*opac*(1.5f/light_sources.size()));
            luz_amb = luz_amb->colorAdd(aux_luz_amb);
            delete(aux_luz_amb);
            Ray* reflect_ray = new Ray(point, distance_to_light.normalize().negative());
            Vect reflDir = reflect(reflect_ray, normal);
            double specular = max(0.d, -(reflDir.dotProduct(ray->getRayDirection())));
            delete(reflect_ray);
            double specular_ind = pow(specular, obj->getColor()->getColorSpec()); //obj->specular
			if (obj->getColor()->getColorSpec() > 0) {
            Color* aux_color_spec = light_sources.at(light_index)->getLightColor()->colorScalar(specular_ind);
			color_spec = color_spec->colorAdd(aux_color_spec);
            delete(aux_color_spec);
			}
        }
	}
    Color* aux_luz_amb = (obj->getColor())->colorScalar(0.6);
	final_color = final_color->colorAdd(luz_amb->colorMultiply(aux_luz_amb));
    delete (aux_luz_amb);
    
    Color* aux_color_spec = color_spec->colorScalar(obj->getColor()->getColorRefl()+obj->getColor()->getColorTransp()+0.05);
	final_color = final_color->colorAdd(aux_color_spec);
    delete (aux_color_spec);
    delete (luz_amb);
    delete(color_spec);
    if (depth < MAX_DEPH){
        //EL OBJETO ES REFLEJANTE
        if (winning_object_color->getColorRefl() > 0 && winning_object_color->getColorRefl() <= 1){
            Ray* reflection = new Ray(displace(reflection_direction.normalize(), point),reflection_direction.normalize());
            Color* color_r = trace(reflection,scene_objects,light_sources,depth+1,transp);
            delete(reflection);
            //Color* color_r = new Color(0,0,0,0,0,0);
            Color* aux_color_r= color_r->colorScalar(winning_object_color->getColorRefl());
            final_color = final_color->colorAdd(aux_color_r);
            delete(aux_color_r);
            delete(color_r);
        }
        //EL OBJETO ES TRANSPARENTE
        double coef_out;

        coef_out = obj->getCoef();
        if (winning_object_color->getColorTransp() > 0){
            //double ang = obj->getNormalAt(point).vectAng(ray->getRayDirection().negative());
            //if(!(ang> asin(coef_out/transp))){
                Vect dir_out = refract(ray->getRayDirection(), normal, obj->getCoef());
                Ray* refraction = new Ray(displace(dir_out.normalize(), point), dir_out.normalize());
                Color* color_t = trace(refraction,scene_objects,light_sources,depth+1, coef_out);
                delete(refraction);
                Color* aux_color_t = color_t->colorScalar(winning_object_color->getColorTransp());
                final_color = final_color->colorAdd(aux_color_t);
                delete(aux_color_t);
                delete(color_t);
           // }

        }
    }
    return final_color->clip();
}

    int main (int argc, char *argv[]) {
    MPI_Init(&argc, &argv ); /* iniciar MPI */
    tinyxml2::XMLDocument doc;
	doc.LoadFile( "config.xml" );
    tinyxml2::XMLElement* resolution = doc.FirstChildElement("RayTracer")->FirstChildElement("Resolution");

	Vect O (0,0,0);
	Vect X (1,0,0);
	Vect Y (0,1,0);
	Vect Z (0,0,1);
/*
    -----------------
    |    CAMERA     |
    -----------------
*/

    tinyxml2::XMLElement* cam = doc.FirstChildElement("RayTracer")->FirstChildElement("Camera")->FirstChildElement("campos");
	Vect campos (cam->FloatAttribute("x", 0), cam->FloatAttribute("y", 1), cam->FloatAttribute("z", 0));
	cam=doc.FirstChildElement("RayTracer")->FirstChildElement("Camera")->FirstChildElement("look_at");
	Vect look_at (cam->FloatAttribute("x", 0), cam->FloatAttribute("y", 0), cam->FloatAttribute("z", 0));

	Vect diff_btw (campos.getVectX() - look_at.getVectX(), campos.getVectY() - look_at.getVectY(), campos.getVectZ() - look_at.getVectZ());

	Vect camdir = diff_btw.negative().normalize();
	Vect camright = Y.crossProduct(camdir).normalize();
	Vect camdown = camright.crossProduct(camdir);
	Camera* scene_cam = new Camera(campos, camdir, camright, camdown);

/*
    -----------------
    |    OBJECTS     |
    -----------------

*/
    vector<Object*> scene_objects;
    //Spheres
    tinyxml2::XMLElement* Objects = doc.FirstChildElement("RayTracer")->FirstChildElement("Objects")->FirstChildElement("Spheres");
	for(tinyxml2::XMLElement* e = Objects->FirstChildElement("sphere"); e!=NULL; e=e->NextSiblingElement("sphere")){
        if(e->BoolAttribute("draw", true)){
        tinyxml2::XMLElement* sphere_att = e->FirstChildElement("sphere_location");
        Vect sphere_location(sphere_att->FloatAttribute("x", 0), sphere_att->FloatAttribute("y", 0), sphere_att->FloatAttribute("z", 0));
        sphere_att = e->FirstChildElement("sphere_color");
        Color* c= new Color(sphere_att->FloatAttribute("r", 0), sphere_att->FloatAttribute("g", 0), sphere_att->FloatAttribute("b", 0),
                      sphere_att->FloatAttribute("ref", 0), sphere_att->FloatAttribute("transp", 0), sphere_att->FloatAttribute("spec", 0)
                      );
    	Sphere* scene_sphere = new Sphere(sphere_location, e->FloatAttribute("radius"), c, e->FloatAttribute("coef"));
        scene_objects.push_back(dynamic_cast<Object*>(scene_sphere));
        }
	}

	//Cylinders
    Objects = doc.FirstChildElement("RayTracer")->FirstChildElement("Objects")->FirstChildElement("Cylinders");
	for(tinyxml2::XMLElement* e = Objects->FirstChildElement("cylinder"); e!=NULL; e=e->NextSiblingElement("cylinder")){
        if(e->BoolAttribute("draw", true)){
        tinyxml2::XMLElement* cylinder_att = e->FirstChildElement("cylinder_location");
        Vect cylinder_location(cylinder_att->FloatAttribute("x", 0), cylinder_att->FloatAttribute("y", 0), cylinder_att->FloatAttribute("z", 0));
        cylinder_att = e->FirstChildElement("cylinder_color");
        Color* c= new Color(cylinder_att->FloatAttribute("r", 0), cylinder_att->FloatAttribute("g", 0), cylinder_att->FloatAttribute("b", 0),
                      cylinder_att->FloatAttribute("ref", 0), cylinder_att->FloatAttribute("transp", 0), cylinder_att->FloatAttribute("spec", 0)
                      );
        cylinder_att = e->FirstChildElement("cylinder_direction");
        Vect cylinder_direction(cylinder_att->FloatAttribute("x", 0), cylinder_att->FloatAttribute("y", 0), cylinder_att->FloatAttribute("z", 0));
    	Cylinder* scene_cylinder = new Cylinder(cylinder_location, cylinder_direction, e->FloatAttribute("radius"), c, e->FloatAttribute("coef"));
        scene_objects.push_back(dynamic_cast<Object*>(scene_cylinder));
        }
	}

	//Planes
    Objects = doc.FirstChildElement("RayTracer")->FirstChildElement("Objects")->FirstChildElement("Planes");
	for(tinyxml2::XMLElement* e = Objects->FirstChildElement("plane"); e!=NULL; e=e->NextSiblingElement("plane")){
        if(e->BoolAttribute("draw", true)){
        tinyxml2::XMLElement* plane_att= e->FirstChildElement("plane_normal");
        Vect plane_normal(plane_att->FloatAttribute("x", 0), plane_att->FloatAttribute("y", 0), plane_att->FloatAttribute("z", 0));
        plane_att = e->FirstChildElement("plane_color");
        Color* c= new Color(plane_att->FloatAttribute("r", 0), plane_att->FloatAttribute("g", 0), plane_att->FloatAttribute("b", 0),
                      plane_att->FloatAttribute("ref", 0), plane_att->FloatAttribute("transp", 0), plane_att->FloatAttribute("spec", 0)
                      );
        Plane* scene_plane = new Plane(plane_normal, e->FloatAttribute("pos"), c);
        scene_objects.push_back(dynamic_cast<Object*>(scene_plane));
        }
	}

    //Table
    tinyxml2::XMLElement* table_att = doc.FirstChildElement("RayTracer")->FirstChildElement("Objects")->FirstChildElement("Table");
    if(table_att->BoolAttribute("draw",false)){
    table_att = doc.FirstChildElement("RayTracer")->FirstChildElement("Objects")->FirstChildElement("Table")->FirstChildElement("table_color");
    Color* table_c= new Color(table_att->FloatAttribute("r", 0), table_att->FloatAttribute("g", 0), table_att->FloatAttribute("b", 0),
                      table_att->FloatAttribute("ref", 0), table_att->FloatAttribute("transp", 0), table_att->FloatAttribute("spec", 0)
                      );
    table_att = table_att->NextSiblingElement("table_p1");
    Vect t1 (table_att->FloatAttribute("x", 0), table_att->FloatAttribute("y", 0), table_att->FloatAttribute("z", 0));
    table_att = table_att->NextSiblingElement("table_p2");
    Vect t2 (table_att->FloatAttribute("x", 0), table_att->FloatAttribute("y", 0), table_att->FloatAttribute("z", 0));
    table_att = table_att->NextSiblingElement("table_material");
    double coef = table_att->FloatAttribute("coef", 1);
	makeTable(t1,t2,table_c,scene_objects, coef);
    }


/*
    -----------------
    |   LIGHTS      |
    -----------------

*/

    vector<Light*> light_sources;

    tinyxml2::XMLElement* Lights = doc.FirstChildElement("RayTracer")->FirstChildElement("Lights");
	for(tinyxml2::XMLElement* e = Lights->FirstChildElement("light"); e!=NULL; e=e->NextSiblingElement("light")){
        if(e->BoolAttribute("on", true)){
        tinyxml2::XMLElement* light_att= e->FirstChildElement("light_position");
        Vect light_position(light_att->FloatAttribute("x", 0), light_att->FloatAttribute("y", 0), light_att->FloatAttribute("z", 0));
        light_att = e->FirstChildElement("light_color");
        Color* c= new Color(light_att->FloatAttribute("r", 0), light_att->FloatAttribute("g", 0), light_att->FloatAttribute("b", 0),
                      light_att->FloatAttribute("ref", 0), light_att->FloatAttribute("transp", 0), light_att->FloatAttribute("spec", 0)
                      );
    	Light* scene_light = new Light(light_position, c);
        light_sources.push_back(scene_light);
        }
	}

	int dpi = resolution->IntAttribute("dpi",72);
	int width = resolution->IntAttribute("width",640);
	int height = resolution->IntAttribute("height",480);
	int n = width*height;

    //profundidad del antialiasing
	int aadepth = 2;
	double aspectratio = (double)width/(double)height;

	int pixel, aa_index;
	double xamnt, yamnt;

    //variables de MPI
    int process_id, nproc, process_group, ngroups, rank_in_group;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id); /* obtener id del proceso */
	MPI_Comm_size(MPI_COMM_WORLD, &nproc); /* obtener número de procesos */
    ngroups = nproc / 4; //cada grupo tiene 4 procesos
    process_group = process_id / 4;
    rank_in_group = process_id % 4;

    int destination = process_group*4;
    double* pixels_aux = NULL;
    if(rank_in_group == 0) {
        pixels_aux = new double[n/ngroups*5];
    }
    int index_pixels_aux = 0;

    RGBType *pixels, *pixels2, *pixels3;
    if(process_id == 0) {
        pixels = new RGBType[n];
        pixels2 = new RGBType[n];
        pixels3 = new RGBType[n];
    }

	for (int y = process_group; y < height; y+=ngroups) {
        double *temp_vars;
        temp_vars = new double[width*5];
        for (int x = 0; x < width; x++) {
			pixel = y*width + x;

            int aax, aay;
            switch(rank_in_group) {
                case 0:
                    aax = 0; aay = 0;
                    break;
                case 1:
                    aax = 0; aay = 1;
                    break;
                case 2:
                    aax = 1; aay = 0;
                    break;
                case 3:
                    aax = 1; aay = 1;
                    break;
            }
            
            // Rayo de la camara al pixel
            if (width > height) {
                // Imagen ancha
                xamnt = ((x + (double)aax /((double)aadepth - 1)) / width)*aspectratio - (((width - height) /(double) height) / 2);
                yamnt = ((height - y) + (double)aax / ((double) aadepth - 1)) / height;
            }
            else if (height > width) {
                // Imagen alta
                xamnt = (x + (double)aax / (aadepth - 1)) / width;
                yamnt = (((height - y) + (double)aax / (aadepth - 1)) / height) / aspectratio - (((height - width) / width) / 2);
            }
            else {
                // Imagen cuadrada
                xamnt = (x + (double)aax / (aadepth - 1)) / width;
                yamnt = ((height - y) + (double)aax / (aadepth - 1)) / height;
            }

            //camera ray
            Vect cam_ray_origin = scene_cam->getCameraPosition();
            Vect cam_ray_direction = camdir.vectAdd(camright.vectMult(xamnt - 0.5).vectAdd(camdown.vectMult(yamnt - 0.5))).normalize();

            Ray* cam_ray = new Ray(cam_ray_origin, cam_ray_direction);
            
            Color* intersection_color = trace(cam_ray,scene_objects,light_sources,1,1);
            delete cam_ray;
            temp_vars[x*5+0] = intersection_color->getColorRed();
            temp_vars[x*5+1] = intersection_color->getColorGreen();
            temp_vars[x*5+2] = intersection_color->getColorBlue();
            temp_vars[x*5+3] = intersection_color->getColorRefl();
            temp_vars[x*5+4] = intersection_color->getColorTransp();

            delete intersection_color;
		}

        if(rank_in_group == 0) {
            double *temp_vars1, *temp_vars2, *temp_vars3;
            temp_vars1 = new double[width*5];
            temp_vars2 = new double[width*5];
            temp_vars3 = new double[width*5];
            MPI_Recv(temp_vars1, width*5, MPI_DOUBLE, process_group*4+1, y+1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(temp_vars2, width*5, MPI_DOUBLE, process_group*4+2, y+2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(temp_vars3, width*5, MPI_DOUBLE, process_group*4+3, y+3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            for (int x = 0; x < width; x++) {
                for(int i = 0; i < 5; i++){
                    pixels_aux[index_pixels_aux*width*5+x*5+i] = (temp_vars[x*5+i]+temp_vars1[x*5+i]+temp_vars2[x*5+i]+temp_vars3[x*5+i]) / 4.0;
                }
            }
            index_pixels_aux++;
            delete [] temp_vars1;
            delete [] temp_vars2;
            delete [] temp_vars3;
        }
        else{
            int tag = y + rank_in_group;
            MPI_Send(temp_vars, width*5, MPI_DOUBLE, destination, tag, MPI_COMM_WORLD);
        }
        delete [] temp_vars;
	}

    int color = (rank_in_group == 0) ? color = 1 : color = 0;
    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, process_id, &new_comm);

    if(rank_in_group == 0 && process_id != 0) {
        MPI_Gather(pixels_aux, n/ngroups*5, MPI_DOUBLE, NULL, n/ngroups*5, MPI_DOUBLE, 0, new_comm);
        delete [] pixels_aux;
    }
    
    double *recvbuf;
    if(process_id == 0) {
        recvbuf = new double[n*5];
        MPI_Gather(pixels_aux, n/ngroups*5, MPI_DOUBLE, recvbuf, n/ngroups*5, MPI_DOUBLE, 0, new_comm);
        delete [] pixels_aux;
        for(int i = 0; i < ngroups; i++) {
            for(int j = 0; j < n/ngroups; j++) {
                pixel = (j/width*ngroups)*width + i*width + j%width;
                int pixel_recvbuf = i*(n/ngroups)+j;
                pixels[pixel].r = recvbuf[pixel_recvbuf*5+0];
                pixels[pixel].g = recvbuf[pixel_recvbuf*5+1];
                pixels[pixel].b = recvbuf[pixel_recvbuf*5+2];

                pixels2[pixel].r = 1 *recvbuf[pixel_recvbuf*5+3];
                pixels2[pixel].g = 1 *recvbuf[pixel_recvbuf*5+3];
                pixels2[pixel].b = 1 *recvbuf[pixel_recvbuf*5+3];

                pixels3[pixel].r = 1 *recvbuf[pixel_recvbuf*5+4];
                pixels3[pixel].g = 1 *recvbuf[pixel_recvbuf*5+4];
                pixels3[pixel].b = 1 *recvbuf[pixel_recvbuf*5+4];
            }
        }
        delete [] recvbuf;

        savebmp("test.bmp",width,height,dpi,pixels);
        savebmp("testRef.bmp",width,height,dpi,pixels2);
        savebmp("testTransp.bmp",width,height,dpi,pixels3);

        delete [] pixels;
        delete [] pixels2;
        delete [] pixels3;
    }

    MPI_Finalize();
	return 0;
}