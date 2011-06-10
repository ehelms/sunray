#include "parameters.h"

namespace Simulation
{

	/** Utility class for passing a large number of variables between classes
	*/
	Parameters::Parameters ( int pBeginMinute, int pBeginHour, int pEndMinute, int pEndHour, 
			int pDay, int pMonth, int pYear, double pOzone, double pAOD, std::string pWeighting, double pLatitude, 
			double pLongitude, int pLatitudeDir, int pLongitudeDir, int pOrientation, int pElevation, int pAzimuth, int pZenith, int pTimeResolution)
	{
		setParameters(pBeginMinute, pBeginHour, pEndMinute, pEndHour, 
				pDay, pMonth, pYear, pOzone, pAOD, pWeighting, pLatitude, 
				pLongitude, pLatitudeDir, pLongitudeDir, pOrientation, pElevation, pAzimuth, pZenith, 
				pTimeResolution);
	}

	Parameters::Parameters ( const Parameters &input )
	{
		beginMinute = input.beginMinute;
		beginHour = input.beginHour;
		endMinute = input.endMinute;
		endHour = input.endHour;
		day = input.day;
		month = input.month;
		year = input.year;
		ozone = input.ozone;
		aod = input.aod;
		weightingFunction = input.weightingFunction;
		latitude = input.latitude;
		longitude = input.longitude;
		hemisphereLatitude = input.hemisphereLatitude;
		hemisphereLongitude = input.hemisphereLongitude;
		orientation = input.orientation;
		elevation = input.elevation;
		granularityAzimuth = input.granularityAzimuth;
		granularityZenith = input.granularityZenith;
		granularityTime = input.granularityTime;
	}

	Parameters::Parameters ( )
	{ }

	Parameters::~Parameters ( )
	{ }


	void Parameters::setParameters(int pBeginMinute, int pBeginHour, int pEndMinute, int pEndHour, 
			int pDay, int pMonth, int pYear, double pOzone, double pAOD, std::string pWeighting, double pLatitude, 
			double pLongitude, int pLatitudeDir, int pLongitudeDir, int pOrientation, int pElevation, int pAzimuth, int pZenith, 
			int pTimeResolution){

		beginMinute = pBeginMinute;
		beginHour = pBeginHour;
		endMinute = pEndMinute;
		endHour = pEndHour;
		day = pDay;
		month = pMonth;
		year = pYear;
		ozone = pOzone;
		aod = pAOD;
		weightingFunction = pWeighting;
		latitude = pLatitude;
		longitude = pLongitude;
		hemisphereLatitude = pLatitudeDir;
		hemisphereLongitude = pLongitudeDir;
		orientation = pOrientation;
		elevation = pElevation;
		granularityAzimuth = pAzimuth;
		granularityZenith = pZenith;
		granularityTime = pTimeResolution;

	}


	/** ############################### BLAKE, WHAT DOES THIS DO?????
	 *  @param const Parameters& right
	 *  @return bool
	 */
	bool Parameters::compareTimeTo(const Parameters &right) {
		return(
				(beginMinute == right.beginMinute) &&	// compare time values
				(beginHour == right.beginHour) &&
				(endMinute == right.endMinute) &&
				(endHour == right.endHour) &&
				(day == right.day) &&
				(month == right.month) &&
				(year == right.year) &&
				(hemisphereLatitude == right.hemisphereLatitude) &&		// compare latitude direction
				(hemisphereLongitude == right.hemisphereLongitude) &&		// compare longitude direction
				(orientation == right.orientation) &&		// compare orientation
				(elevation == right.elevation) &&		// compare elevation
				(granularityTime == right.granularityTime) &&	// compare time res
				(latitude == right.latitude) &&				// compare latitutde 
				(longitude == right.longitude)	);		// compare diffuse flag	
	}

	/** ############################# BLAKE, WHAT DOES THIS DO?????
	 *  @param const Parameters &left,const Parameters &right
	 *  @return bool
	 */
	bool operator==(const Parameters &left,const Parameters &right){
		return (
				(left.beginMinute == right.beginMinute) &&	// compare time values
				(left.beginHour == right.beginHour) &&
				(left.endMinute == right.endMinute) &&
				(left.endHour == right.endHour) &&
				(left.day == right.day) &&
				(left.month == right.month) &&
				(left.year == right.year) &&
				(left.weightingFunction == right.weightingFunction) &&			// compare weightingFunction
				(left.hemisphereLatitude == right.hemisphereLatitude) &&		// compare latitude direction
				(left.hemisphereLongitude == right.hemisphereLongitude) &&		// compare longitude direction
				(left.orientation == right.orientation) &&		// compare orientation
				(left.elevation == right.elevation) &&		// compare elevation
				(left.granularityAzimuth == right.granularityAzimuth) &&				// compare granularityAzimuth res
				(left.granularityZenith == right.granularityZenith) &&					// compare granularityZenith res
				(left.granularityTime == right.granularityTime) &&	// compare time res
				(left.ozone == right.ozone) &&					// compare ozone
				(left.aod == right.aod) &&						// compare aod
				(left.latitude == right.latitude) &&				// compare latitutde 
				(left.longitude == right.longitude) 			// compare longitude
		);
	}

	/** Allows a Parameters to be printed to an output stream
	 *  @param ostream& os,const Parameters &right
	 */
	std::ostream& operator<<(std::ostream& os,const Parameters &right){
		os<<"Begin,"<<right.beginHour<<":"<<right.beginMinute<<std::endl;// compare time values
		os<<"End,"<< right.endHour<<":"<<right.endMinute<<std::endl;
		os<<"Date,"<<right.month<<"/"<<right.day<<"/"<<right.year<<std::endl;
		os<<"Location,"<< right.latitude<<" deg "<<((right.hemisphereLatitude==0)?"N ":"S ")<< right.longitude<<" deg "<<((right.hemisphereLongitude==0)?"W":"E")<<std::endl;	// compare latitude direction
		os<<"Orientation,"<< right.orientation<<" deg"<<std::endl;	// compare orientation
		os<<"Elevation,"<< right.elevation<<" ft"<<std::endl;	// compare orientation
		os<<"Azimuth Resolution,"<< right.granularityAzimuth<<" deg"<<std::endl;			// compare azimuth res
		os<<"Zenith Resolution,"<< right.granularityZenith<<" deg"<<std::endl;				// compare zenith res
		os<<"Time Resoultion,"<< right.granularityTime<<" min"<<std::endl;// compare time re
		os<<"Ozone,"<< right.ozone<<" DU"<<std::endl;				// compare ozone
		os<<"AOD,"<< right.aod<<" km"<<std::endl;					// compare aod
		return os;	
	}

	/** Compares this Parameters to another
	 * @param const Parameters &left,const Parameters &right
	 * @return bool
	 */
	bool operator!=(const Parameters &left,const Parameters &right){
		return !(left==right);
	}


	/** Returns variable
	 *  @return 
	 */
	double Parameters::getOzone ( ) const {
		return ozone;
	}

	/** Returns variable
	 *  @return 
	 */
	double Parameters::getAOD ( ) const {
		return aod;
	}

	/** Returns variable
	 *  @return 
	 */
	std::string Parameters::getSpectralWeightingFunction ( ) const {
		return weightingFunction;
	}

	/** Returns variable
	 *  @return 
	 */
	double Parameters::getLatitude ( ) const {
		return latitude;
	}

	/** Returns variable
	 *  @return 
	 */
	double Parameters::getLongitude ( ) const {
		return longitude;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getLatitudeDir ( ) const {
		return hemisphereLatitude;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getLongitudeDir ( ) const {
		return hemisphereLongitude;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getOrientation ( ) const {
		return orientation;
	}
	
	int Parameters::getElevation ( ) const {
		return elevation;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getAzimuthResolution ( ) const {
		return granularityAzimuth;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getZenithResolution ( ) const {
		return granularityZenith;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getTimeResolution ( ) const {
		return granularityTime;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getBeginMinute ( ) const {
		return beginMinute;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getBeginHour ( ) const {
		return beginHour;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getEndHour ( ) const {
		return endHour;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getEndMinute ( ) const {
		return endMinute;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getDay ( ) const {
		return day;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getMonth ( ) const {
		return month;
	}

	/** Returns variable
	 *  @return 
	 */
	int Parameters::getYear ( ) const {
		return year;
	}

	int Parameters::getThetaResolution ( ) const {
		return 1;
	}
	int Parameters::getPhiResolution ( ) const {
		return 1;
	}
	int Parameters::getRhoResolution ( ) const {
		return 1;
	}

	double Parameters::getVisibility ( ) const
	{
		return 1;
	}
	double Parameters::getSurfaceAlbedo ( ) const
	{
		return 1;
	}
	double Parameters::getReflectance ( ) const
	{
		return 1;
	}
	bool Parameters::getFresnelFlag ( ) const
	{
		return true;
	}

	unsigned int Parameters::getPolygonCount ( ) const
	{
		return 100;
	}
	double *Parameters::getSortedPolygons ( ) const
	{
		return NULL;
	}
}
