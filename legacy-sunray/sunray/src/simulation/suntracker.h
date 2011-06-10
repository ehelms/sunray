// suntracker.h: interface for the suntracker class.
//
//////////////////////////////////////////////////////////////////////
#include <cstdio>
#include <string>
#include <cmath>
#include <ctime>
#include <iostream>
#include "parameters.h"


#define min_(x,y) ((x)<(y) ? (x) : (y))
#define max_(x,y) ((x)>(y) ? (x) : (y))

#ifndef SUNTRACKER_H_
#define SUNTRACKER_H_

namespace Simulation
{

	class Suntracker  
	{
		public:
			Suntracker();
			virtual ~Suntracker();
				
			double GetDayAngle();
			double GetDeclination();
			double GetEQTime();
			int GetTrueSolarTime();
			double GetAltitude();
			double GetAzimuth();
			double GetNormalFlux(struct tm timeofday, double altitude);
			double GetDiffuseOld(struct tm timeofday, double slope, double incidentang, double altitude);

			int getTST(int lst);
			void setCurrTime(int seconds);

			double getMaxFlux();
			double getBeam(double rSlope, double rAspect);
			double getDiffuse(double rSlope, double rAspect);

			void setupSuntracker ( int hour, int min, const Parameters &UI );								

		protected:

			void output();
			void beam_flux();
			void sky_diffuse(double rslope);
			double stdmeridian();
			void solaralt();		
			void solaraz();			
			void solarDist ();		
			void terrestrialFlux();	
			double incident(double rSlope, double rAspect);
			double beam_flux(double incAng, int tim);
			double d_mod(double x, double y);
			int trusoltim(int lst, double eqt );
			int miltime(int ctime);
			int cumtime(int mtime);
			int round(double num);
			void refract (double rzenith, double & rzprime);


			//My Functions
			void DayAngle();
			void Declination();
			void EQTime();
			void TrueSolarTime();
			void CalcuateAltitudeAzimuth();


			int tsr; 
			int tss; 
			int asr; 
			int ass;

			//My Added Variables
			int day;
			int month;
			int year;
			int hour;
			int minute;
			int LongitudeDir;
			int LatitudeDir;
			double rDayAngle;
			double rDeclination; 
			double EQT;
			double Longitude;
			double Latitude;
			double pi;
			double altitude;
			double azimuth;
			int currTST;

			//OLD
			double solflux; 
			double atmtra;

			double rlat;  //the latitude
			double rlong; //the longitdude


			double alt			[1441]; 
			double apalt		[1441]; 
			double az			[1441];
			double incidentang	[1441];
			double diffuse		[1441];
			double nrmbflux; 
			double horbflux		[1441];
			double beam			[1441];

	};
}

#endif
