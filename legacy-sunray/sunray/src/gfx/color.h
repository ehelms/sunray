#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>
#include <cmath>

/*The algorithms for translating between colorspaces are taken from
	"Introduction to Computer Graphics" by Foley, Van Dam, et al.
	1997, pg. 415-416
*/
//Color data structure
typedef struct{
	float red;
	float green;
	float blue;
} Color;
//static void print(Color &c);
//returns maximum value of r,g,b
float max_val(float r,float g,float b);
//returns minimum values of r,g,b
float min_val(float r,float g,float b);
//convert and RGB color to HSV
void RGBtoHSV( float r, float g, float b, float *h, float *s, float *v );
//convert RGB color to HSV
void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
void HSVtoRGB( unsigned char &r, unsigned char &g, unsigned char &b, float h, float s, float v );
void HSVtoRGB(Color &color, float h, float s, float v );
Color HSVtoRGB(float h, float s, float v );
const Color RED={1.0,0,0};
const Color DARK_RED={0.75,0.0,0};
const Color GREEN={0,1.0,0};
const Color DARK_GREEN={0.0,0.75,0};
const Color BLUE={0,0,1.0};
const Color DARK_BLUE={0,0.0,0.75};
const Color YELLOW={1.0,1.0,0};
const Color DARK_YELLOW={0.75,0.75,0};
const Color PURPLE={1.0,0.0,1.0};
const Color DARK_PURPLE={0.5,0.0,0.5};
const Color TEAL={0,1.0,1.0};
const Color BLACK={0,0,0};
const Color WHITE={1,1,1};
const Color GRAY={0.5,0.5,0.5};
const Color DARK_GRAY={0.25,0.25,0.25};
const Color LIGHT_GRAY={0.75,0.75,0.75};
#endif // _COLOR_H_

