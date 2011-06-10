#include "color.h"
//RGB format uses normalized values for red, green, and blue components
//HSV format uses normalized values for saturation and value. Hue ranges between [0,360]
//Standard colors and their values 

/*
static void print(Color &c){
	std::cout<<"("<<c.red<<","<<c.green<<","<<c.blue<<")"<<std::endl;
}
*/
//returns maximum value of r,g,b
float max_val(float r,float g,float b){
  if(r>g&&r>b)return r;else if(g>b&&g>r)return g; else return b;
}
//returns minimum values of r,g,b
float min_val(float r,float g,float b){
  if(r<g&&r<b)return r;else if(g<b&&g<r)return g; else return b;
}
//convert and RGB color to HSV
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v )
{
	float min, max, delta;

	min = min_val( r, g, b );
	max = max_val( r, g, b );
	*v = max;				// v

	delta = max - min;

	if( max != 0 )
		*s = delta / max;		// s
	else {
		// r = g = b = 0		// s = 0, v is undefined
		*s = 0;
		*h = -1;
		return;
	}

	if( r == max )
		*h = ( g - b ) / delta;		// between yellow & magenta
	else if( g == max )
		*h = 2 + ( b - r ) / delta;	// between cyan & yellow
	else
		*h = 4 + ( r - g ) / delta;	// between magenta & cyan

	*h *= 60;				// degrees
	if( *h < 0 )
		*h += 360;

}
//convert RGB color to HSV
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v )
{
	int i;
	float f, p, q, t;

	if( s == 0 ) {
		// achromatic (grey)
		*r = *g = *b = v;
		return;
	}

	h /= 60;			// sector 0 to 5
	i = (int)std::floor( h );
	f = h - i;			// factorial part of h
	p = v * ( 1.0f - s );
	q = v * ( 1.0f - s * f );
	t = v * ( 1.0f - s * ( 1.0f - f ) );

	switch( i ) {
		case 0:
			*r = v;
			*g = t;
			*b = p;
			break;
		case 1:
			*r = q;
			*g = v;
			*b = p;
			break;
		case 2:
			*r = p;
			*g = v;
			*b = t;
			break;
		case 3:
			*r = p;
			*g = q;
			*b = v;
			break;
		case 4:
			*r = t;
			*g = p;
			*b = v;
			break;
		default:		// case 5:
			*r = v;
			*g = p;
			*b = q;
			break;
	}

}
void HSVtoRGB( unsigned char &r, unsigned char &g, unsigned char &b, float h, float s, float v ){
  float red,green,blue;
  HSVtoRGB( &red,&green,&blue,h,s,v);
  r = static_cast<int>(255 * red);
  g = static_cast<int>(255 * green);
  b = static_cast<int>(255 * blue);
}
void HSVtoRGB(Color &color, float h, float s, float v ){
  float red,green,blue;
  HSVtoRGB( &red,&green,&blue,h,s,v);
  color.red=red;
  color.green=green;
  color.blue=blue;
}
Color HSVtoRGB(float h, float s, float v ){
  float red,green,blue;
  HSVtoRGB( &red,&green,&blue,h,s,v);
  Color color={red,green,blue};
	return color;
}
