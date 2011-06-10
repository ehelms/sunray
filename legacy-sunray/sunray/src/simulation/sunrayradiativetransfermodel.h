#ifndef SUNRAYRADIATIVETRANSFERMODEL_H_
#define SUNRAYRADIATIVETRANSFERMODEL_H_

#include <string>
#include "parameters.h"
#include "suntracker.h"
#include "radiativetransfermodel.h"

namespace Simulation
{
	/*
	extern "C" void g95_runtime_start(int argc, char *argv[]);
	extern "C" void g95_runtime_stop();
	*/
	extern "C" {
		void read_coeffs_beam__ ( char *, double * );
		void read_coeffs_diffuse__ ( char *, double * );
		void compute_hsi_field__ (
				double*, double*, double*, double*,
				double*, int*, double*, double*,
				int*, double*, double*,
				double*, double*, double*,
				int*, double*, double*, double*, double*,
				double*, double*, double*, double*
		);
		void polygon_obstruction_mask__(
				double*, double*, double*, int*, int*, double*,
				double*, double*, int*, int*, int*
		);
		void spectral_irradiance__ (
				double*,double*,double*,double*,double*,
				int*,int*,double*,double*,double*,double*,double*,
				int*,int*,int*,double*,int*,int*,
				double*,double*,double*,double*,
				double*,double*,double*,double*,double*,
				double*,double*,double*,double*,double*
		);
		void sun_distance_scale_factor__ ( double *, double * );
		void angleprecision_ ( int *, double * );

		void determine_coefficients__ (
				double *, double *,
				double *, double *, double *, double *,
				double *, double *
		);
	}

	class SunRayRadiativeTransferModel : public RadiativeTransferModel
	{
		public:
			SunRayRadiativeTransferModel ( Parameters );
			virtual ~SunRayRadiativeTransferModel();

			/**
			 * @param day angle (radians) from SunTracker
			 * @return scale factor
			 * @depend specrad.f
			 */
			double getSunDistanceScaleFactor ( double );
			/**
			 * @param apparent solar zenith angle
			 * @param z ?
			 * @return path length of beam through atmosphere to observer
			 * @depend specrad.f
			 */
			double getAtmosphericPathLength ( double, double );

			/**
			 * @return precision
			 */
			double getAnglePrecision ( int * );

			/**
			 * @param array [ thetaRes, phiRes ]
			 * @return array of (polar,azimuthal) angles
			 * @depend specrad.f
			 */
			double * getPolarAzimuthalArray ( int * );


		protected:
			virtual Parameters getParameters ( ) const;
			virtual void performIrradianceCalculation ( );
			/**
			 * Determines beam and diffuse coefficients for given parameters.
			 * @param elevation
			 * @param ozone
			 * @param visibility
			 * @param surface albedo
			 */
			void determineCoefficients ( double, double, double, double );

			virtual void getBeamSourceCoefficients ( );
			virtual void getDiffuseSourceCoefficients ( );

			std::string getBeamCoefficientsFile ( );
			std::string getDiffuseCoefficientsFile ( );

		private:
			bool verifyInputs ( );

			double beamSource[27][2][3][3][4], beamCoefficients[108];
			double diffuseSource[27][2][3][3][3][13], diffuseCoefficients[351];
			Parameters &inputs;
	};

}

#endif /*SUNRAYRADIATIVETRANSFERMODEL_H_*/
