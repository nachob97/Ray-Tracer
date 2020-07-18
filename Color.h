#ifndef _COLOR_H
#define _COLOR_H

class Color {
	double red, green, blue, reflective, transparence, specular;

	public:

	Color ();

	Color (double, double, double, double,double, double);

	// method functions
	double getColorRed() { return red; }
	double getColorGreen() { return green; }
	double getColorBlue() { return blue; }
	double getColorRefl() { return reflective; }
	double getColorTransp() { return transparence; }
	double getColorSpec(){return specular;}

	void setColorRed(double redValue) { red = redValue; }
	void setColorGreen(double greenValue) { green = greenValue; }
	void setColorBlue(double blueValue) { blue = blueValue; }
	void setColorRefl(double reflValue) { reflective = reflValue; }
	void setColorTransp(double transpValue) { transparence = transpValue; }

	double brightness() {
		return(red + green + blue)/3;
	}

	Color* colorScalar(double scalar) {
		return new Color (red*scalar, green*scalar, blue*scalar, reflective,transparence, specular);
	}

	Color* colorAdd(Color* color) {
	    red += color->getColorRed();
	    green += color->getColorGreen();
	    blue += color->getColorBlue();
		return  this;
	}

	Color* colorMultiply(Color* color) {
	    red *= color->getColorRed();
	    green *= color->getColorGreen();
	    blue *= color->getColorBlue();
		return this;
	}

	Color* colorAverage(Color* color) {
	    red = (red + color->getColorRed())/2;
	    green = (green + color->getColorGreen())/2;
	    blue =(blue + color->getColorBlue())/2;
		return this;
	}

	Color* clip() {
		double alllight = red + green + blue;
		double excesslight = alllight - 3;
		if (excesslight > 0) {
			red = red + excesslight*(red/alllight);
			green = green + excesslight*(green/alllight);
			blue = blue + excesslight*(blue/alllight);
		}
		if (red > 1) {red = 1;}
		if (green > 1) {green = 1;}
		if (blue > 1) {blue = 1;}
		if (red < 0) {red = 0;}
		if (green < 0) {green = 0;}
		if (blue < 0) {blue = 0;}

		return new Color (red, green, blue, reflective,transparence, specular);
	}
};

Color::Color () {
	red = 0.5;
	green = 0.5;
	blue = 0.5;
}

Color::Color (double r, double g, double b, double refl,double transp, double spec) {
	red = r;
	green = g;
	blue = b;
	reflective = refl;
	transparence = transp;
	specular = spec;
}

#endif
