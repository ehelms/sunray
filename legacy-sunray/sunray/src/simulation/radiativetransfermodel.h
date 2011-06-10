#ifndef _RADIATIVE_TRANSFER_MODEL_H_
#define _RADIATIVE_TRANSFER_MODEL_H_

#include <map>
#include "parameters.h"
#include "suntracker.h"
// TODO this class shouldn't need to know about this stuff
#include "../gfx/primitives.h"
#include "../SunPacket.h"
#include "../scenario/polarground.h"

namespace Simulation
{

	// Fortran Prototypes
	/*
	extern "C" { 
		void pathlength_( double* apsza, double* atmpth );
		void sundistancescalefactor_( double* rdayangle, double* sundist_scale );
		void interpolateregressionparams_( double* apsza, double* o3, double* aod, double* o3min, double* o3max, double* aodmin, double* aodmax, double direct_parameter_db[4][2][2][27], double diffuse_parameter_db[6][2][2][27][12], double direct_coeff[4][27], double diffuse_coeff[6][27]);
		void polydiffuseirradiance_(double diffuse_coeff[6][27], double* apsza, double* saz, double* body_geo_orient, int grid_res[2], double* sundist_scale, double* ray_slope, double* ray_aspect, double* slope, double* aspect, double diffuse_irradiance[27]);
		void polydirectirradiance_( double direct_coeff[4][27], double* apsza, double* saz, double* slope, double* aspect, double* atmpth, double* sundist_scale, double band[27], double ssa[27], double direct_irradiance[27] );
		void polytotalirradiance_( double direct_irradiance[27], double diffuse_irradiance[27], double total_irradiance[27] ); 
		void polybioeffectiveirradiance_( double swf[27], double total_irradiance[27], double* bio_effective_irradiance );
		void polybeamincidentangle_( double* slope, double* aspect, double* apsza, double* saz, double* incident_angle);
	}
	*/

	//Main Solar Radiation Class used for initializing and setting up variables with values from GUI
	//Makes calls to the Fortran Prototypes provided in the Solar Radiation Model code
	class RadiativeTransferModel
	{
		public:
			RadiativeTransferModel ( );
			virtual ~RadiativeTransferModel ( );

			int initialize ( );
			GFX::vector3d getBeamDirection ( );
			bool getNextTimeStep ( );

		protected:
			virtual Parameters getParameters ( ) const = 0;

			Suntracker getSunTracker ( ) { return st; }
			double getApparentSolarZenithAngle ( ) { return apsza; }
			double getSolarAzimuthAngle ( ) { return saz; }
			double getDayAngle ( ) { return rdayangle; }

		private:
			Suntracker st;

			int beginHour;
			int endHour;
			int beginMinute;
			int endMinute;
			int TimeStepCounter;
			int TimeResolution;

			double rdayangle;
			double apsza;
			double saz;
			/*
			std::map<const unsigned int, double> getDirectIrradiance ( const GFX::point3d &, const GFX::vector3d & );
			std::map<const unsigned int, double> getDiffuseIrradiance ( const GFX::point3d &, const GFX::vector3d &, const GFX::vector3d & );
			std::map<const unsigned int, double> getReflectiveIrradiance ( const GFX::point3d &, const GFX::vector3d &, const PolarGround & );
			void setInputParameters ( Parameters * );

		protected:
			void readRegressionParameters ( );
			void spectralWeightingFunctions ( );

		private:
			Suntracker *st;
			Parameters *UI;

			//Variables to be given a set value by the GUI
			double aodmax;
			double aodmin;
			double body_geo_orient;
			double o3min;
			double o3max;
			double sundist_scale;
			double atmpth;
			double o3;
			double aod;
			double band[27];
			double ssa[27];
			double swf[27];
			double direct_irradiance[27];
			double diffuse_irradiance[27];
			double total_irradiance[27];
			double direct_coeff[4][27];
			double diffuse_coeff[6][27];

			//variables for suntracker calculation of Azimuth and Zenith sun angles
			int day;
			int month;
			int year;
			int LongitudeDir;
			int LatitudeDir;
			double Latitude;
			double Longitude;
			double directBroadband;
			double diffuseBroadband;


			enum SWF { NONE, ERYTHEMA, DNA, SKINCANCER, MELANOMA, CATARACT, IMMUNOSUPPRESSION, PLANTGROWTH, PHYTOPLANKTON };

			double pi;

			//Regression Parameter databases
			//Pre-calculated values that are read-in from a text file
			double direct_parameter_db[4][2][2][27];
			double diffuse_parameter_db[6][2][2][27][12];

			int phi_grid_res;
			int theta_grid_res;
			*/

	};

}

#endif // _RADIATIVE_TRANSFER_MODEL_H_
