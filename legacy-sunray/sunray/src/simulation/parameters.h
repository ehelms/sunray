#ifndef _PARAMETERS_H_
#define _PARAMETERS_H_

#include <ctime>
#include <iostream>
#include <string>

#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>

#include "../logger.h"

namespace Simulation
{

	class AbstractParameters
	{
		public:
			AbstractParameters ( ) { }
			~AbstractParameters ( ) { }
	};


	class Parameters
	{
		public:
			Parameters ( int, int, int, int, int, int, int, double, double, std::string, double, double, int, int, int, int, int, int, int );
			Parameters ( );
			Parameters ( const Parameters & );
			~Parameters ( );

			void setParameters( int, int, int, int, int, int, int, double, double, std::string, double, double, int, int, int, int, int, int, int );

		private:
			unsigned int beginMinute, beginHour, endMinute, endHour;
			unsigned int day, month, year;
			double ozone, aod;
			std::string weightingFunction;
			double latitude, longitude;
			unsigned int orientation, elevation;
			unsigned char hemisphereLatitude, hemisphereLongitude;
			unsigned int granularityAzimuth, granularityZenith, granularityTime;

		public:
			friend bool operator==(const Parameters &left,const Parameters &input);
			friend bool operator!=(const Parameters &left,const Parameters &input);

			friend std::ostream& operator<< ( std::ostream& os, const Parameters &right );

			bool compareTimeTo ( const Parameters &right );

			int getBeginMinute ( ) const;
			int getBeginHour ( ) const;
			int getEndMinute ( ) const;
			int getEndHour ( ) const;
			int getDay ( ) const;
			int getMonth ( ) const;
			int getYear ( ) const;

			double getAOD ( ) const;
			std::string getSpectralWeightingFunction ( ) const;

			double getLatitude ( ) const;
			double getLongitude ( ) const;
			int getLatitudeDir ( ) const;
			int getLongitudeDir ( ) const;
			int getOrientation ( ) const;

			int getElevation ( ) const;
			double getOzone ( ) const;
			double getVisibility ( ) const;
			double getSurfaceAlbedo ( ) const;

			double getReflectance ( ) const;
			bool getFresnelFlag ( ) const;

			int getThetaResolution ( ) const;
			int getPhiResolution ( ) const;
			int getRhoResolution ( ) const;
			int getAzimuthResolution ( ) const;
			int getZenithResolution ( ) const;
			int getTimeResolution ( ) const;

			unsigned int getPolygonCount ( ) const;
			double *getSortedPolygons ( ) const;
	};

}

#endif // _PARAMETERS_H_

