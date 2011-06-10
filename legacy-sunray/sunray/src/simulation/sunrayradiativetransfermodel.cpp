#include "sunrayradiativetransfermodel.h"
#include <iostream>

namespace Simulation
{

	SunRayRadiativeTransferModel::SunRayRadiativeTransferModel(Parameters p)
		: RadiativeTransferModel(), inputs(p)
	{
		/*
		beamSource = beamCoefficients = NULL;
		diffuseSource = diffuseCoefficients = NULL;
		*/
	}
	
	SunRayRadiativeTransferModel::~SunRayRadiativeTransferModel()
	{
	}

	double SunRayRadiativeTransferModel::getSunDistanceScaleFactor ( double dayAngle )
	{
		double factor = 0;
		sun_distance_scale_factor__(&dayAngle, &factor);
		return factor;
	}

	void SunRayRadiativeTransferModel::performIrradianceCalculation ( )
	{
		if ( ! verifyInputs() ) {
			//throw SimulationInvalidInputException();
		}

		determineCoefficients(inputs.getElevation(), inputs.getOzone(), 
				inputs.getVisibility(), inputs.getSurfaceAlbedo());
		
		double apsza = getApparentSolarZenithAngle();
		double elev = inputs.getElevation();
		double saz = getSolarAzimuthAngle();
		double reflectance = inputs.getReflectance();
		int fresnelFlag = inputs.getFresnelFlag() ? 1 : 0;
		int gridRes[] = {
			inputs.getPhiResolution(),
			inputs.getRhoResolution()
		};

		double dayAngle = getDayAngle();
		double sunScale = getSunDistanceScaleFactor(dayAngle);
		//double atmpth = getAtmosphericPathLength(apsza, elev);
		double precision = getAnglePrecision(gridRes);

		double *polarAzArray = getPolarAzimuthalArray(gridRes);
		double *sunPolarAzArray = getPolarAzimuthalArray(gridRes); // todo wrong

		double bodyCoordTwist = 0.0;
		double bodyGeoOrient = inputs.getOrientation();

		int iBeamFlag = 0, irBeamFlag = 0;
		int obstruct[181][360];

		unsigned int numPolys = inputs.getPolygonCount();
		// \/ todo this is where it segfaults \/
		double *sortedPolys = inputs.getSortedPolygons();

		double height = 100.0;
		int epsilon = 0;
		double rho = 0.0, rhohi = 0.0, phi = 0.0, phihi = 0.0,
			   atmpth = 0.0, hssbi = 0.0, hssdi = 0.0, hssi = 0.0;
		compute_hsi_field__(&height, &apsza, &saz, &precision,
				sortedPolys, (int*)&numPolys, &bodyCoordTwist, &bodyGeoOrient,
				gridRes, polarAzArray, &elev, 
				beamCoefficients, diffuseCoefficients, &sunScale,
				&epsilon, &rho, &rhohi, &phi, &phihi,
				&atmpth, &hssbi, &hssdi, &hssi
		);

		for ( unsigned int i = 0; i < numPolys; ++i ) {
			polygon_obstruction_mask__(&apsza, &saz, sortedPolys, (int*)&numPolys, (int*)&i, &precision,
					&bodyCoordTwist, &bodyGeoOrient, &obstruct[0][0], &iBeamFlag, &irBeamFlag
			);

			double beamIrrad[27], skyIrrad[27], rBeamIrrad[27], rSkyIrrad[27], grdIrrad[27];
			spectral_irradiance__(&sunScale, &apsza, &saz, &atmpth, &reflectance,
					&fresnelFlag, gridRes, &precision, polarAzArray, sunPolarAzArray,
					&bodyCoordTwist, &bodyGeoOrient, &iBeamFlag, &irBeamFlag, &obstruct[0][0],
					sortedPolys, (int*)&i, &epsilon, &rho, &rhohi, &phi, &phihi,
					&hssi, &hssdi, &hssbi,
					beamCoefficients, diffuseCoefficients,
					// these are the outputs
					beamIrrad, skyIrrad, rBeamIrrad, rSkyIrrad, grdIrrad
			);
		}
	}

	Parameters SunRayRadiativeTransferModel::getParameters ( ) const
	{
		return inputs;
	}

	double *SunRayRadiativeTransferModel::getPolarAzimuthalArray ( int *gridRes )
	{
		unsigned int thetas = 90 / inputs.getThetaResolution();
		unsigned int phis = 360 / inputs.getPhiResolution();
		double polarAzArray[thetas * phis * 2];
	}

	double SunRayRadiativeTransferModel::getAnglePrecision ( int *gridRes )
	{
		double precision = 0.0;
		angleprecision_(gridRes, &precision);
		return precision;
	}

	void SunRayRadiativeTransferModel::determineCoefficients ( double elev, double o3, double vis, double alb )
	{
		getBeamSourceCoefficients();
		getDiffuseSourceCoefficients();
		determine_coefficients__(&beamSource[0][0][0][0][0], &diffuseSource[0][0][0][0][0][0],
				&elev, &o3, &vis, &alb, beamCoefficients, diffuseCoefficients);
	}

	void SunRayRadiativeTransferModel::getBeamSourceCoefficients ( )
	{
		std::string beamCoeffsFile = getBeamCoefficientsFile();
		read_coeffs_beam__(const_cast<char *>(beamCoeffsFile.c_str()), &beamSource[0][0][0][0][0]);
	}

	void SunRayRadiativeTransferModel::getDiffuseSourceCoefficients ( )
	{
		std::string diffuseCoeffsFile = getDiffuseCoefficientsFile();
		read_coeffs_diffuse__(const_cast<char *>(diffuseCoeffsFile.c_str()), &diffuseSource[0][0][0][0][0][0]);
	}

	std::string SunRayRadiativeTransferModel::getBeamCoefficientsFile ( )
	{
		return "/home/josiah/workspace/sunray/trunk/src/simulation/resources/coeff_b_pbio.txt";
	}

	std::string SunRayRadiativeTransferModel::getDiffuseCoefficientsFile ( )
	{
		return "/home/josiah/workspace/sunray/trunk/src/simulation/resources/coeff_d_pbio.txt";
	}

	bool SunRayRadiativeTransferModel::verifyInputs ( )
	{
		return false;
	}
}

