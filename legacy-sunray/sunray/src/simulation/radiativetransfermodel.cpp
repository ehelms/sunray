#include "radiativetransfermodel.h"

#include <string>
#include <fstream>

#include "simulation.h"
#include "logger.h"

namespace Simulation
{
	int RadiativeTransferModel::initialize ( )
	{
		TimeStepCounter = (int)ceil(((endHour*60 + endMinute) - (beginHour*60 + beginMinute))/(double)TimeResolution);
		return (TimeStepCounter+1);
	}

	/**
	 * Calculates the coordinates of the sun in model space. 
	 * 
	 * @return GFX::vector3d cartesian coordinates of the sun
	 */
	GFX::vector3d RadiativeTransferModel::getBeamDirection ( )
	{
		double sun_az_model;

		GFX::vector3d vec;

		sun_az_model = (360.0 - saz) + getParameters().getOrientation();

		double pi = acos(-1.0);
		double rapsza = (pi/180.0)*apsza;
		double r_sun_az_model = (pi/180.0)*sun_az_model;

		vec.x = sin(rapsza)*cos(r_sun_az_model);
		vec.y = sin(rapsza)*sin(r_sun_az_model);
		vec.z = cos(rapsza);

		return vec;
	}

	/**
	 * Calculates whether another timestep is needed and returns true or false.
	 * If a new timestep is needed, the angles of the sun are recomputed
	 * and the regression parameters are interpolated.
	 *
	 * @return bool true for another time step availible
	 */
	bool RadiativeTransferModel::getNextTimeStep ( )
	{
		if ( TimeStepCounter <0 ) {
			return false;
		} else {
			int minutes=beginHour*60 + beginMinute+TimeResolution*TimeStepCounter;
			int hour=(int)floor(minutes/60.0);
			int minute=(int)minutes%60;

			st.setupSuntracker(hour, minute, getParameters());
			rdayangle = st.GetDayAngle();
			saz = st.GetAzimuth();
			apsza = 90. - st.GetAltitude();

			//Setup from Fortran
			/*
			sundistancescalefactor_( &rdayangle, &sundist_scale );
			pathlength_( &apsza, &atmpth );
			interpolateregressionparams_( &apsza, &o3, &aod, &o3min, &o3max, &aodmin, &aodmax, 
					direct_parameter_db, diffuse_parameter_db, 
					direct_coeff, diffuse_coeff);
					*/

			TimeStepCounter--;
			return true;
		}
	}

	RadiativeTransferModel::RadiativeTransferModel ( )
	{
		/*
		//Constant values used in regression equations
		aodmax = 25;
		aodmin = 5;
		o3min = 200;
		o3max = 300;

		//Variables to be set by GUI in setInputParameters
		body_geo_orient = 0;
		rdayangle = 0;
		apsza = 0;
		saz = 0;
		o3 = 200;
		aod = 5;
		sundist_scale = 0;
		atmpth = 0;
		beginHour = 0;
		endHour = 0;
		beginMinute = 0;
		endMinute = 0;
		day = 0;
		month = 0;
		year = 100;
		Latitude = 0;
		Longitude = 0;
		TimeStepCounter = 0;
		TimeResolution = 0;
		diffuseBroadband = 0;

		pi = acos(-1.0);

		//Default Resolution Values
		phi_grid_res = 30;
		theta_grid_res = 15;

		//Creates an array of the particular wavelengths 
		//being used in the direct and diffuse irradiance calculations
		for(int bandx = 0; bandx < 23; bandx++){
			band[bandx] = 295.0 + 5.0*((double)(bandx - 1)) ;
		}

		band[23] = 450.0;
		band[24] = 550.0;
		band[25] = 650.0;
		band[26] = 750.0;


		for(int bandx = 0; bandx < 27; bandx++){
			ssa[bandx] = (1 - ((log10(band[bandx]) - 2)/(3-2)) ) * 1.0 
				+ ((log10(band[bandx]) - 2)/(3-2)  ) * 0.6;
		}

		//Suntracker inialization
		st = NULL;
		//UserInput inialization
		UI = NULL;
		*/
	}

	RadiativeTransferModel::~RadiativeTransferModel ( )
	{
		/*
		if ( st ) {
			delete st;
		}

		std::cout << "Broadband Direct = " << directBroadband << std::endl;;
		std::cout << "Broadband Diffuse = " << diffuseBroadband << std::endl;;
		*/
	}

//
//	void RadiativeTransferModel::setInputParameters ( Parameters *input )
//	{
//
//		UI = input;
//
//		//Values set by GUI
//		o3 = (double)UI->getOzone();
//		aod = (double)UI->getAOD();
//		body_geo_orient = (double)UI->getOrientation(); 
//		phi_grid_res = UI->getAzimuthResolution();
//		theta_grid_res = UI->getZenithResolution();
//		beginHour = UI->getBeginHour();
//		endHour = UI->getEndHour();
//		beginMinute =  UI->getBeginMinute();
//		endMinute = UI->getEndMinute();
//
//		
//		   day = UI->getDay();
//		   /*   month = UI->getMonth();
//		   year = UI->getYear();
//		   Longitude = UI->getLongitude();
//		   Latitude = UI->getLatitude();
//		   LongitudeDir = UI->getLongitudeDir();
//		   LatitudeDir = UI->getLatitudeDir();
//		   */
//		TimeResolution = UI->getTimeResolution();
//
//		//Sets the counter for the number of time steps to be taken
//		initialize();
//
//		//Setup Spectral Weighting Function Information
//		std::string wf = UI->getSpectralWeightingFunction();
//		std::string SWF_File;
//		if ( wf == "DNA" ) {
//			SWF_File = "swf_DNA.txt";
//		} else if ( wf == "ERYTHEMA" ) {
//			SWF_File = "swf_erythema.txt";
//		} else if ( wf == "PHYTOPLANKTON" ) {
//			SWF_File = "swf_Phytoplankton.txt";
//		} else if ( wf == "PLANTGROWTH" ) {
//			SWF_File = "swf_PlantGrowth.txt";
//		} else if ( wf == "SKINCANCER" ) {
//			SWF_File = "swf_skincancer.txt";
//		} else if ( wf == "MELANOMA" ) {
//			SWF_File = "swf_melanoma.txt";
//		} else if ( wf == "CATARACT" ) {
//			SWF_File = "swf_cataract.txt";
//		} else if ( wf == "IMMUNOSUPPRESSION" ) {
//			SWF_File = "swf_immunosuppression.txt";
//		} else {
//			SWF_File = "swf_None.txt";
//		}
//
//		//Opens input file to read in spectral weighting function
//		std::ifstream in((RESOURCE_PATH + SWF_File).c_str());
//		if ( in ) {
//			char band[10];
//			for ( int i = 0; i < 27; i++ ) {
//				in >> band;
//				in >> swf[i];
//			}
//			in.close();
//		} else {
//			//Logger::error("Cannot Open File SWF");
//		}
//
//		//Suntracker set-up
//		st = new Suntracker;
//
//		//Setup the pregression databases and formulate coefficients
//		readRegressionParameters();
//	}
//
//
//	/**
//	 * Calculates the irradiance at a point due to the reflective properties
//	 * of a nearby surface. Right now this means the ground/water/ice upon
//	 * which the model sits. This could be extended to include surrounding
//	 * objects
//	 *
//	 * @param point the cartesian coordinates of the point
//	 * @param normal the point normal
//	 * @param ground the polar reflective ground
//	 * @return SunPacket<27> a sunpacket containing the reflective irradiance
//	 */
//	std::map<const unsigned int, double> RadiativeTransferModel::getReflectiveIrradiance ( const GFX::point3d &point, const GFX::vector3d &normal, const PolarGround &ground )
//	{
//		std::map<const unsigned int, double> reflection;
//		for ( int i = 0; i < 27; ++i ) {
//			reflection[i] = 1337;
//		}
//		return reflection;
//	}
//
//	/**
//	 * Calculates Direct Irradiance by being passed a point and its normal 
//	 * The normal coordinates are then converted to polar coordinates
//	 * from cartesian and then passed along with the necessary
//	 * parameters to the Fortran subroutine.  The returned array is then
//	 * convolved by the selected spectral weighting function. 
//	 * The direct_irradiance array is then passed and set in
//	 * a SunPacket.
//	 * 
//	 * @param point the cartesian coordinates of the point
//	 * @param normal the cartesian coordinates of the point normal
//	 * @return SunPacket<27> a sunpacket containing the direct irradiance
//	 */
//	std::map<const unsigned int, double> RadiativeTransferModel::getDirectIrradiance ( const GFX::point3d &point, const GFX::vector3d &normal ) 
//	{
//		double slope = acos(normal.z);	
//		double aspect = 0;
//
//		if ( normal.x > 0 && normal.y  >= 0 ) {
//			aspect = atan(normal.y/normal.x);
//		} else if ( normal.x < 0 ) {
//			aspect = pi + atan(normal.y/normal.x);
//		} else if ( normal.x > 0 && normal.y < 0 ) {
//			aspect = 2*pi + atan(normal.y/normal.x);
//		} else if ( normal.x == 0 && normal.y > 0 ) {
//			aspect = pi/2;
//		} else if ( normal.x == 0 && normal.y < 0 ) {
//			aspect = (3*pi)/2;
//		} else {
//			aspect = 0;
//		}
//
//		aspect = (2*pi - (aspect - body_geo_orient*pi/180.0));
//		if( aspect > 2*pi) aspect = aspect - 2*pi;
//
//		for(int i = 0; i < 27; i++){
//			direct_irradiance[i] = 0;
//		}
//
//		directBroadband += st->getBeam(slope, aspect);
//
//		polydirectirradiance_(direct_coeff, &apsza, &saz, &slope, &aspect, &atmpth, &sundist_scale, band, ssa, direct_irradiance);
//
//		std::map<const unsigned int, double> irrad;
//		for ( int i = 0; i < 27; i++ ) {
//			direct_irradiance[i] *= swf[i];
//			irrad[i] = direct_irradiance[i];
//		}
//
//		return irrad;
//	}
//
//
//
//	/**
//	 * Calculates Diffuse Irradiance by being passed a point and its normal, 
//	 * and the coordinates of the diffuse ray.
//	 * The normal and diffuse ray coordinates are then converted to polar coordinates
//	 * from cartesian and then passed along with the necessary
//	 * parameters to the Fortran subroutine.  The returned array is then
//	 * convolved by the selected spectral weighting function. 
//	 * The diffuse_irradiance array is then passed and set in
//	 * a SunPacket.
//	 * 
//	 * @param point the cartesian coordinates of the point
//	 * @param normal the cartesian coordinates of the point normal
//	 * @param direction the cartesian coordinates of the diffuse ray
//	 * @return SunPacket<27> a sunpacket containing the diffuse irradiance
//	 */
//	std::map<const unsigned int, double> RadiativeTransferModel::getDiffuseIrradiance ( const GFX::point3d &point, const GFX::vector3d &normal, const GFX::vector3d &direction )
//	{
//
//		int grid_res[2];
//		grid_res[0] = theta_grid_res;
//		grid_res[1] = phi_grid_res;
//
//		//Convert ray direction to polar
//		double ray_slope = acos(direction.z);	
//		double ray_aspect = 0;
//
//		if ( direction.x > 0 && direction.y  >= 0 ) {
//			ray_aspect = atan(direction.y/direction.x);
//		} else if ( direction.x < 0 ) {
//			ray_aspect = pi + atan(direction.y/direction.x);
//		} else if ( direction.x > 0 && direction.y < 0 ) {
//			ray_aspect = 2*pi + atan(direction.y/direction.x);
//		} else if ( direction.x == 0 && direction.y > 0 ) {
//			ray_aspect = pi/2;
//		} else if ( direction.x == 0 && direction.y < 0 ) {
//			ray_aspect = (3*pi)/2;
//		} else {
//			ray_aspect = 0;
//		}
//
//		//Convert normal to polar
//		double slope = acos(normal.z);	
//		double aspect = 0;
//
//		if ( normal.x > 0 && normal.y  >= 0 ) {
//			aspect = atan(normal.y/normal.x);
//		} else if ( normal.x < 0 ) {
//			aspect = pi + atan(normal.y/normal.x);
//		} else if ( normal.x > 0 && normal.y < 0 ) {
//			aspect = 2*pi + atan(normal.y/normal.x);
//		} else if ( normal.x == 0 && normal.y > 0 ) {
//			aspect = pi/2;
//		} else if ( normal.x == 0 && normal.y < 0 ) {
//			aspect = (3*pi)/2;
//		} else {
//			aspect = 0;
//		}
//
//		//ray_slope = ray_slope + slope;
//		//ray_aspect = ray_aspect + aspect;
//
//
//		for ( int i = 0; i < 27; i++ ) {
//			diffuse_irradiance[i] = 1;
//		}
//
//		diffuseBroadband += st->getDiffuse(slope, aspect);
//
//		polydiffuseirradiance_(diffuse_coeff, &apsza, &saz, &body_geo_orient, grid_res, &sundist_scale, &ray_slope, &ray_aspect, &slope, &aspect, diffuse_irradiance) ;
//
//
//		std::map<const unsigned int, double> irrad;
//		for ( int i = 0; i < 27; i++ ) {
//			diffuse_irradiance[i] *= swf[i];
//			irrad[i] = diffuse_irradiance[i];
//		}
//
//		return irrad;
//	}
//
//
//	/**
//	 * Reads in the direct and diffuse coefficients stored in 
//	 * in directparams.txt and diffuseparams.txt.  The values
//	 * are stored in the diffuse and direct coefficient arrays.
//	 */
//	void RadiativeTransferModel::readRegressionParameters ( )
//	{
//		char line[100];
//
//		double apsza,band,o3,aod;
//
//		//Opens input file to read in direct irradiance parameters
//		std::string directParams = RESOURCE_PATH + "directparams.txt";
//		std::ifstream in(directParams.c_str());
//		if ( !in ) {
//			//Logger::error("Cannot Open File Direct Params");
//			//Logger::error(directParams);
//			return;
//		}
//
//		//Reads in non-data at the beginning of the file to be thrown away
//		for ( int i = 0; i < 7; i++ ) {
//			in >> line;
//		}
//
//		//Creates an array from the file data that stores the direct parameters for the different wavelengths
//		for ( int o3x = 0; o3x < 2; o3x++ ) {
//			for ( int aodx = 0; aodx < 2; aodx++ ) {
//				for ( int bandx = 0; bandx < 27; bandx++ ) {
//
//					in >> band;
//					in >> o3;
//					in >> aod;
//					in >> direct_parameter_db[0][aodx][o3x][bandx];
//					in >> direct_parameter_db[1][aodx][o3x][bandx];
//					in >> direct_parameter_db[2][aodx][o3x][bandx];
//					in >> direct_parameter_db[3][aodx][o3x][bandx];
//				}     
//			}
//		}
//		in.close();
//
//
//		//Opens input file to read in diffuse irradiance parameters
//		std::string diffuseParams = RESOURCE_PATH + "diffuseparams.txt";
//		std::ifstream in1(diffuseParams.c_str());
//		if ( !in1 ) {
//			//Logger::error("Cannot Open File Diffuse Params");
//			//Logger::error(diffuseParams);
//			return;
//		}
//
//		//Reads in non-data at the beginning of the file to be thrown away
//		for ( int i = 0; i < 10; i++ ) {
//			in1 >> line;
//		}
//
//		//Creates an array from the file data that stores the direct parameters for the different wavelengths   
//		for ( int o3x = 0; o3x < 2; o3x++ ) {
//			for ( int aodx = 0; aodx < 2; aodx++ ) {
//				for ( int apszax = 0; apszax < 12; apszax++ ) {
//					for ( int bandx = 0; bandx < 27; bandx++ ) {
//
//						in1 >> apsza;
//						in1 >> band;
//						in1 >> o3;
//						in1 >> aod;
//						in1 >> diffuse_parameter_db[0][aodx][o3x][bandx][apszax];
//						in1 >> diffuse_parameter_db[1][aodx][o3x][bandx][apszax];
//						in1 >> diffuse_parameter_db[2][aodx][o3x][bandx][apszax];
//						in1 >> diffuse_parameter_db[3][aodx][o3x][bandx][apszax];
//						in1 >> diffuse_parameter_db[4][aodx][o3x][bandx][apszax];
//						in1 >> diffuse_parameter_db[5][aodx][o3x][bandx][apszax];
//					}
//				}     
//			}
//		}
//		in1.close();
//
//		return;
//	}
//
//
//	/**
//	 * Reads in the spectral weighting function in question stored in 
//	 * in swf.txt.  
//	 */
//	void RadiativeTransferModel::spectralWeightingFunctions ( )
//	{
//		char band[10];
//
//		//Opens Spectral Weighting Function file 
//		std::ifstream in("swf.txt");
//		if ( !in ) {
//			//Logger::error("Cannot Open File");
//			return;
//		}
//
//		for ( int bandx = 0; bandx < 27; bandx++ ) {
//			in >> band;
//			in >> swf[bandx];
//		}
//
//		in.close();
//
//		return;          
//	}
//
//
//
}

