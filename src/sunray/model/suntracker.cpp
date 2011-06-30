#include "suntracker.h"

namespace Simulation
{

	static int days[2][12] =
	{
		{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
		{ 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
	};

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	Suntracker::Suntracker()
	{

		rDayAngle = 0;
		rDeclination = 0;
		EQT = 0;
		day = 0;
		month = 0;
		year = 100;
		LongitudeDir = 0;
		LatitudeDir = 0;
		Longitude = 0;
		Latitude = 0;
		altitude = 0;
		azimuth = 0;


		solflux = 0; 
		rlat = 0;  //the latitude
		rlong = 0; //the longitdude


		nrmbflux = 0.0; 

		currTST = 0;
		tsr = 0; 
		tss = 0; 
		asr = 0; 
		ass = 0;


		pi = acos(-1.0);
		atmtra = 0.65;		//standard transmission value


		for (int i = 0; i < 1441; i++){
			alt[i] = 0; 
			apalt[i] = 0; 
			az[i] = 0;
			incidentang[i] = 0;
			diffuse[i] = 0;
			horbflux[i] = 0;
			beam[i] = 0;
		}

	}

	Suntracker::~Suntracker()
	{

	}

	void Suntracker::setupSuntracker(int hr, int min, const Parameters &UI)
	{

		day = UI.getDay();
		month = UI.getMonth();
		year = UI.getYear();
		Longitude = UI.getLongitude();
		Latitude = UI.getLatitude();
		LongitudeDir = UI.getLongitudeDir();
		LatitudeDir = UI.getLatitudeDir();
		hour = hr;
		minute = min;

		//Converts longitude to correct degree from GUI
		if (LongitudeDir == 1) {		// if it's west instead of east
			Longitude *= -1;				// make it negative
		}
		if (LatitudeDir == 1) {		// if it's south instead of north
			Latitude *= -1;
		}

		DayAngle();
		Declination();
		EQTime();
		TrueSolarTime();
		CalcuateAltitudeAzimuth();
		solarDist();
		terrestrialFlux();

		//solaralt();

		//GetAltAz(rDeclination,rlat, 100,  altit, azim);

		//solaraz();  	

		// here we have the normal beam flux NRMBFLUX 
		// and the horizontal beam flux HORBFLUX
		//terrestrialFlux();

	}

	void Suntracker::DayAngle()
	{
		int leap;

		leap = !(year % 4) && (year % 100) || !(year % 400);

		for (int i = 0; i < month - 1; i++){
			day += days[leap][i];
		}

		if (leap) { 
			year = 366;
		} else { 
			year = 365;
		}

		rDayAngle = (2.0 * pi * (double)(day - 1)) / (double)year;
	}

	void Suntracker::Declination()
	{
		rDeclination = 0.006918 +
			(-0.399912 * cos(rDayAngle)) +
			(0.070257 * sin(rDayAngle)) +
			(-0.006758 * cos(2 * rDayAngle)) +
			(0.000907 * sin(2 * rDayAngle)) +
			(-0.002697 * cos(3 * rDayAngle)) +
			(0.001480 * sin(3 * rDayAngle));
	}

	void Suntracker::EQTime()
	{
		EQT = 229.18 * (0.000075 +
				0.001868 * cos(rDayAngle) +
				-0.032077 * sin(rDayAngle) +
				-0.014615 * cos(2 * rDayAngle) +
				-0.040890 * sin(2 * rDayAngle));
	}

	void Suntracker::TrueSolarTime()
	{
		double d;
		double stdmer;

		if (Longitude == 0.0) stdmer = 0.0;
		else if (Longitude < 0.0) stdmer = -(fabs(Longitude) - fmod(fabs(Longitude), 15.0));
		else stdmer = Longitude + (15.0 - fmod(Longitude, 15.0));

		d = Longitude - stdmer;
		minute = minute + (int)(4 * d + ((d<0) ? -0.5 : 0.5)) + (int)(EQT + ((EQT > 0) ? 0.5 : -0.5));

		if (minute >= 120) { hour += 2; minute -= 120; }
		else if (minute >= 60) { hour += 1; minute -= 60; }
		else if (minute < -60) { hour -= 2; minute += 120; }
		else if (minute < 0) { hour -= 1; minute += 60; }

		if (hour > 23) hour -= 24;
		else if (hour < 0) hour += 24;

		currTST = hour * 60 + minute - 1;
	}

	void Suntracker::CalcuateAltitudeAzimuth()
	{
		double d;
		double zenith, rzenith;
		double truealt;


		double rLatitude = (Latitude / 180.0) * pi;

		d = pi - ((pi / 180.0) * 15.0 * ( (double)currTST / 60.0));
		d = sin(rDeclination) * sin(rLatitude) +
			cos(rDeclination) * cos(rLatitude) * cos(d);
		if (fabs(d) > 1.0) d = ((d<0) ? -1.0 : 1.0);
		truealt = asin(d);


		rzenith = pi / 2.0 - asin(d); if (rzenith < 0) rzenith = 0;
		zenith = rzenith * 180.0 / pi;
		if (zenith <= 90.83 && zenith > 0) {
			d = 21.2 * exp(pow(zenith / 90.83, 8.8));
			d = 1.0 + 0.000394965 * pow(zenith / 90.83, d);
			d = d * sin(rzenith) / 1.00029;
			if (fabs(d) > 1.0) d = ((d<0) ? -1.0 : 1.0);
			rzenith = asin(d);
		}

		altitude = 180 / pi * (pi / 2 - rzenith);

		if (truealt == pi / 2.0) azimuth = 90.0;
		else if (truealt == -pi / 2.0) azimuth = 270.0;
		else {
			d = (sin(truealt) * sin(rLatitude) - sin(rDeclination)) / (cos(truealt) * cos(rLatitude));
			if (fabs(d) > 1.0) d = ((d<0) ? -1.0 : 1.0);
			d = 180.0 / pi * ((currTST>720) ? -acos(d) : acos(d));
			azimuth = 180.0 - d;
		}
	}

	void Suntracker::solarDist () //double rDayAngle)
	{
		//     INPUT  DAYANGLE [RADIANS]
		//     RETURNS  SOLAR FLUX TO EARTH [W/M**2]
		double meandist, term1, term2, term3, term4, term5, solcon, ecf, sundist;



		//     MEANDIST IS IN METERS
		meandist = 1.4959789e11;

		//     COMPUTE ECCENTRICITY CORRECTION FACTOR
		term1 = 1.00011;
		term2 = cos(rDayAngle)		 * 0.034221;
		term3 = sin(rDayAngle)		 * 0.00128;
		term4 = cos(rDayAngle * 2.0) * 0.000719;
		term5 = sin(rDayAngle * 2.0) * 0.000077;
		ecf = term1 + term2 + term3 + term4 + term5;

		//     COMPUTE SUN-EARTH DISTANCE
		sundist = sqrt(pow(meandist, 2.0) / ecf);

		//     COMPUTE SOLAR FLUX AT THE ATMOSPHERE
		solcon = 1367.0; 
		solflux = ( solcon * pow((meandist / sundist),2.0) );
	}

	void Suntracker::terrestrialFlux()
	{

		//     INPUT EXTRATERRESTRIAL SOLAR FLUX: SOLFLUX [W/M**2] 
		//     INPUT ARRAY OF APPARENT SOLAR ALTITUDES: APALT   
		//     RETURNS ARRAY OF NORMAL INCIDENCE BEAM FLUX:  NRMBFLUX [W/M**2] 
		//     RETURNS ARRAY OF HORIZONTAL SURFACE BEAM FLUX: HORBFLUX [W/M**2]


		//     CLOUDLESS SKY TRANSMISSIVITY [0.5=HAZY, 0.8=EXCEPTIONALLY CLEAR] 
		double atmpth;



		//     INITIALIZE ARRAYS
		nrmbflux = 0.;
		//horbflux[tim+1] = 0.;


		atmpth = 1.0 / sin((pi / 180.0) * (90.0 - altitude));
		nrmbflux = solflux * pow(atmtra, atmpth);
		//horbflux[tim+1] = nrmbflux[tim+1] * sin(pi / 180.0 * apalt[tim+1]);


	}

	double Suntracker::GetDayAngle()
	{
		return rDayAngle;
	}

	double Suntracker::GetDeclination()
	{
		return rDeclination;
	}

	double Suntracker::GetEQTime()
	{
		return EQT;
	}

	int Suntracker::GetTrueSolarTime()
	{
		return currTST;
	}

	double Suntracker::GetAltitude()
	{
		return altitude;
	}

	double Suntracker::GetAzimuth()
	{
		return azimuth;
	}

	double Suntracker::getBeam(double rSlope, double rAspect)
	{

		double tempBeam = 0;
		double rIncidentAngle = incident(rSlope, rAspect);

		tempBeam =  nrmbflux * cos(rIncidentAngle);  
		if(tempBeam < 0.0) 
			tempBeam = 0.0;

		return tempBeam;

	}

	double Suntracker::incident(double rSlope, double rAspect)
	{ 	
		double rpsi;
		double ca, cd, cl, sa, cs, sd, sl, ss, arg;
		double psi;
		double rzprime;

		double rLatitude = Latitude * ( pi/180.0);


		sa = sin(rAspect);
		sd = sin(rDeclination);
		sl = sin(rLatitude);
		ss = sin(rSlope);
		ca = cos(rAspect);
		cd = cos(rDeclination);
		cl = cos(rLatitude);
		cs = cos(rSlope);


		rzprime = pi / 180.0 * (altitude);
		psi = 180.0 - azimuth;
		rpsi = pi / 180.0 * psi;
		arg = cs * cos(rzprime) + ss * sin(rzprime) * cos(rpsi - rAspect);
		if (fabs(arg) > 1.0) {
			arg = ((arg<0) ? -1.0 : 1.0);
		}

		//cout << "Incident Angle = " << 180.0 / pi * acos(arg) << endl;

		return acos(arg);

	}



	double Suntracker::getDiffuse(double rSlope, double rAspect){

		double theta, thetaz, param_c,sky, rIncidentAngle;

		// get incident for a specific time (current TST)  
		rIncidentAngle = incident( rSlope, rAspect);


		theta = rIncidentAngle;

		//     THETAZ IS SOLAR ZENITH ANGLE 
		thetaz = pi / 180.0 * (90.0 - altitude);
		param_c = sin(rDayAngle + 1.49618) * - 0.041117 + 0.09037;
		sky = param_c * nrmbflux;

		return .5 * sky *  (cos(rSlope) + 1.0) * ( pow(sin(rSlope / 2.0),3) + 1.0) * 
			(1.0 + pow(cos(theta),2) * pow(sin(thetaz),3));

	}


	/*
	   double Suntracker::beam_flux(double rIncidentAngle)
	   {
	   double tempBeam = 0;

	   tempBeam =  nrmbflux[currTST+1] * cos(rIncidentAngle);  
	   if(tempBeam < 0.0) 
	   tempBeam = 0.0;
	   return tempBeam;

	   }
	   */

	/*
	   void Suntracker::setupSuntracker(double lat, double lon, int day, int month, int year, double atrans )
	   {

	   atmtra = atrans;



	//rDayAngle = GetDayAngle(day,month,year);

	//rDeclination = GetDeclination(rDayAngle);
	//EQT = GetEQTime(rDayAngle);

	//solaralt();

	//GetAltAz(rDeclination,rlat, 100,  altit, azim);

	//solaraz();   

	// begin subroutines to calculate beam and diffuse flux onto a surface  
	//solarDist();


	// here we have the normal beam flux NRMBFLUX 
	// and the horizontal beam flux HORBFLUX
	//terrestrialFlux();

	return;

	}
	*/

	/*
	   double Suntracker::GetDiffuseOld(struct tm timeofday, double slope,
	   double incidentang, double altitude)
	   {
	   double normalflux = GetNormalFlux(timeofday,altitude);

	   if ((slope >= pi/2) || (altitude <= 0)) return 0.0;

	   incidentang *= pi/180.0;
	   slope *= pi/180.0;
	   altitude *= pi/180.0;

	   int day = timeofday.tm_mday, month = timeofday.tm_mon, year = timeofday.tm_year+1900;
	   double dayangle = GetDayAngle(day, month, year);
	   double thetaz = incidentang * (pi/2 - altitude);
	   double sky = (-0.041117 * sin(dayangle + 1.49618) + 0.09037) * normalflux;

	   sky *= 0.5 * (1.0 + cos(slope)) *
	   pow((1.0 + sin(slope/2.0)), 3.0) *
	   (pow((1.0 + cos(incidentang)), 2.0) + pow(sin(thetaz), 3.0));
	   return sky;
	   }
	   */

	/*
	   double Suntracker::GetNormalFlux(struct tm timeofday, double altitude)
	   {
	   if (altitude < 0.0) return 0.0;
	   if (altitude == 0.0) altitude = 0.00001;

	   altitude *= pi/180.0;

	   const double meandist = 1.4959789E+11;
	   const double atmtrn = 0.65;

	   int day = timeofday.tm_mday, month = timeofday.tm_mon, year = timeofday.tm_year+1900;
	   double dayangle = GetDayAngle(day, month, year);
	   double ecf, sundist, solarflux;

	   ecf = 1.000110 +
	   (0.034221 * cos(dayangle)) +
	   (0.001280 * sin(dayangle)) +
	   (0.000719 * cos(2 * dayangle)) +
	   (0.000077 * sin(2 * dayangle));
	   sundist = sqrt(pow(meandist, 2.0) / ecf);
	   solarflux = 1367.0 * pow(meandist/sundist, 2.0);
	   solarflux *= pow(atmtrn, 1.0/sin(altitude));

	   return solarflux;
	   }
	   */


	/*
	   void Suntracker::setCurrTime(int seconds){

	   currTST = getTST(seconds/60);

	   }
	   */

	/*
	   void Suntracker::solaralt()
	   { 

	   double argument, tsunrise, tsunset, hrang;
	   int minperday, timasrmin, timassmin, timtsrmin, timtssmin, 
	   tsrminute, tssminute;

	// FLAGS FOR NOSUNRISE,NOSUNSET <==> CONTINUOUS POLAR DAY OR NIGHT
	int nosr = 0, noss = 0; 

	double daylen;
	int miltsr, miltss, tim;
	double rzenith, rzprime;
	int tsrhour, tsshour;

	//rlat  = (pi/180.0)*lat;  //the latitude
	//rlong = (pi/180.0)*lon; //the longitdude

	if (rlat == pi / 2.0) {
	if (rDeclination > 0) {
	daylen = 24;
	}
	else if (rDeclination == 0) {
	daylen = 12;
	}
	else if (rDeclination < 0) {
	daylen = 0;
	}
	}
	else if (rlat == -pi / 2.0) {
	if (rDeclination > 0) {
	daylen = 0;
	}
	else if (rDeclination == 0) {
	daylen = 12.0;
	}
	else if (rDeclination < 0) {
	daylen = 24.0;
	}
	}
	else {
	argument = -tan(rlat) * tan(rDeclination);
	if (fabs(argument) > 1.0) {
	argument = ((argument<0) ? -1.0 : 1.0);
	}
	daylen = (2.0/15.0)*(180.0/pi) * acos(argument);
	}

	tsunrise = 12.0 - (daylen / 2.0);
	tsunset =  12.0 + (daylen / 2.0);

	minperday = round(daylen * 60.0);

	//     COMPUTE TRUE SUNRISE AND SUNSET IN MILITARY TIME 

	tsrhour = (int) tsunrise;
	tsrminute = round ((tsunrise - (double) tsrhour) * 60.0);
	if (tsrminute == 60) {
	++tsrhour;
	tsrminute = 0;
	}
	miltsr = tsrhour * 100 + tsrminute;
	tsshour = (int) tsunset;
	tssminute = round ((tsunset - (double) tsshour) * 60.0);
	if (tssminute == 60) {
	++tsshour;
	tssminute = 0;
	}
	miltss = tsshour * 100 + tssminute;
	tsr = miltsr;
	tss = miltss;

	//     COMPUTE TIME OF TRUE SUNRISE AND SUNSET (CUMULATIVE MINUTES) 
	//     HOUR ANGLE (HRANG) RANGE [+pi,-pi) OVER A 24-HOUR DAY 

	timtsrmin = 720 - round((double) minperday / 2.0);
	timtssmin = 720 + round((double) minperday / 2.0);
	if (timtsrmin < 0) {
	timtsrmin = 0;
	}
	if (timtssmin > 1439) {
	timtssmin = 1439;
	}

	for (tim = 0; tim <= 1439; ++tim) {
	hrang = pi - ((pi / 180.0) * 15.0 * ((double) tim / 60.0));
	argument = (sin(rDeclination) * sin(rlat)) + (cos(rDeclination) * cos(rlat) * cos(hrang));
	if (fabs(argument) > 1.0) {
	argument = ((argument<0) ? -1.0 : 1.0);
	}
	alt[tim+1] = (180.0 / pi) * asin(argument);

	//      COMPUTE APPARENT ALTITUDE (ALTITUDE CORRECTED FOR REFRACTION) 

	rzenith = (pi / 2.0) - asin(argument);
	rzenith = (0.0 < rzenith)? rzenith: 0.0;
	refract(rzenith, rzprime);
	apalt[tim +1] = (180.0 / pi) * ((pi / 2.0) - rzprime);
	}
	//     DETERMINE APPARENT SUNRISE AND SUNSET TIMES DUE TO  REFRACTION 
	//     EXCLUDE CASES OF POLAR CONTINUOUS DAY AND POLAR CONTINUOUS NIGHT 

	nosr = 0;
	noss = 0;
	if (apalt[721] > 0.0) {
	//      AT SOLAR NOON THE SUN IS ABOVE THE HORIZON 
	if (apalt[1] > 0.0) {
	nosr = 1;
	}
	if (apalt[1440] > 0.0) {
	noss = 1;
	}
	} else {
	//      AT SOLAR NOON THE SUN IS BELOW THE HORIZON 
	nosr = 1;
	noss = 1;
	}

	int temp =0;

	if (apalt[timtsrmin + 1] > 0.0) {
	for (tim = timtsrmin; tim >= 0; --tim) {
	if (apalt[tim + 1] <= 0.0) {
	temp = tim;
	tim=-1;
	}
	}
	}
	timasrmin = (temp < 0)? 0 : temp;

	temp = 1439;

	if (apalt[timtssmin + 1] > 0.) {
	for (tim = timtssmin; tim <= 1439; ++tim) {
	if (apalt[tim + 1] <= 0.) {
	temp = tim;
	tim = 1440;
	}

	}
	}

	timassmin = (temp > 1439) ? 1439 : temp ;

	asr = miltime(timasrmin);
	ass = miltime(timassmin);

	return;

	} 
	*/

	void Suntracker::solaraz(){ //double rDeclination,double alt[],double az[]) {
		double ralt, arg;
		int tim;
		double psi;

		/*     INPUT  DECLENATION (RADIANS) */
		/*      ARRAY OF SOLAR ALTITUDES (DEGREES) */
		/*      APPARENT SUNRISE AND SUNSET TIMES (MILITARY), */
		/*     RETURNS ARRAY OF SOLAR AZIMUTHS (DEGREES) */
		/*     SOLAR AZIMUTH ANGLE IS CONSTANT AT POLES */

		if (rlat == pi / 2.0) {
			for (int i = 0; i <= 1440; ++i)
				az[i] = 180.0;
		}
		else if (rlat == -pi / 2.0) {
			for (int i = 0; i <= 1440; ++i)
				az[i] = 0.0;
		}

		/*     COMPUTE SOLAR AZIMUTH FROM APPARENT SUNRISE TO APPARENT SUNSET */
		/*     compute solar azimuth from 0000 to 2359 for program showsun only */
		/*     SOLAR AZIMUTH IS A FUNCTION OF TRUE SOLAR ALTITUDE (ALT) */
		else {
			for (tim = 0; tim <= 1439; ++tim) {
				ralt = pi / 180.0 * alt[tim + 1];
				/*      TEST: IF SOLAR ALTITUDE AT ZENITH OR NADIR: AZIMUTH UNDEFINED */
				if (fabs(ralt) == pi / 2.0) {
					/*       TEST FOR SOLAR ZENITH; ASSIGN AZIMUTH = EAST */
					if (ralt == pi / 2.0) {
						az[tim + 1] = 90.0;
					}
					/*       TEST FOR SOLAR NADIR;  ASSIGN AZIMUTH = WEST */
					if (ralt == -pi / 2.0) {
						az[tim + 1] = 270.0;
					}
				}
				else {
					arg = (sin(ralt) * sin(rlat) - sin(rDeclination)) / (cos(ralt) * cos(rlat));
					if (fabs(arg) >= 1.0) {
						arg = ((arg<0) ? -1.0 : 1.0);
					}

					if (tim <= 720) {
						/*       TIME OF DAY IS A.M. */
						psi = 180.0 / pi * acos(arg);
					} else {
						/*       TIME OF DAY IS P.M. */
						psi = -(180.0 / pi) * acos(arg);
					}

					/*      AZIMUTH ANGLE (PSI) RANGE [+180, -180) */
					/*      AZIMUTH ANGLE (AZ) RANGE [0, +360) */

					az[tim + 1] = 180.0 - psi;
				}
			}
		}
	}

	void Suntracker::refract (double rzenith, double & rzprime) {
		double argument, e, munot, factor, zenith;

		/*     Numerical algorithm from J.J.Streicher */
		/*     INPUT ZENITH ANGLE (RZENITH)[RADIANS] */
		/*     RETURNS CORRECTED ZENITH ANGLE (RZPRIME)[RADIANS] */
		/*     ROUTINE ACCOUNTS FOR ATMOSPHERIC REFRACTION IN OBSERVED ZENITH */

		zenith = rzenith * (180.0 / pi);

		/*     REGRESSION EQUATION FOR ZENITH VALID OVER RANGE [0, 90.83] */
		/*     APPARENT SUNRISE/SUNSET OCCUR AT ZENITH = 90.83 */
		/*     90.83 = 90.57 (REFR OF SUN CENTER) + 0.26 (SEMI-DIAM OF SUN) */
		/* NOTE: APALTITUDE COMPUTATIONS REFERENCE SUN CENTER FOR ALT >> HORIZON */
		/* NOTE: FOR APALT NEAR THE HORIZON, REF POINT MIGRATES TO TOP OF SUN */
		/* NOTE: TO MAINTAIN APALT REF TO CENTER, MUST CHANGE CODE AS FOLLOWS - */
		/* NOTE: SUB REFRACT - PARAMETERIZE RZPRIME WRT 90.57 DEGREES */
		/* NOTE: SUB SOLARALT - COMPUTE ASR, ASS BY COMPARING APALT WITH (-0.26), */
		/* NOTE:                NOT (0.0) */

		if (zenith > 90.83) {
			rzprime = rzenith;
		}
		else if (zenith == 0.0) {
			rzprime = rzenith;
		}
		else {
			munot = 1.00029;
			e = exp(pow((zenith / 90.83), 8.8)) * 21.2;
			factor = pow((zenith / 90.83), e) * 3.94965e-4 + 1.0;
			argument = (factor * sin(rzenith)) / munot;
			if (fabs(argument) > 1.0) {
				argument = ((argument<0) ? -1.0 : 1.0);
			}
			rzprime = asin(argument);
		}
	}

	/*
	   int Suntracker::getTST(int lst)
	   {

	   return cumtime( trusoltim(miltime(lst), EQT ) );

	   }
	   */

	int Suntracker::miltime(int ctime)
	{
		int hour, minute;

		/*     INPUT CUMULATIVE TIME [0,1439] [MINUTES] */
		/*     RETURNS MILITARY TIME OF DAY IN I4.4 FORMAT [0000,2359] */
		/*     NOTE: THIS FUNCTION DOES NOT INCREMENT DATE */

		hour = (int) (ctime / 60.0);
		minute = ctime - hour * 60;
		if (minute == 60) {
			++hour;
			minute = 0;
		}
		if (hour >= 24) {
			hour -= 24;
		}
		return hour * 100 + minute;
	}
	//***********************************************************************  
	//*******************************CUMTIME 11FEB92*************************
	int Suntracker::cumtime(int mtime){

		int hour, minute;

		/*     INPUT MILITARY TIME OF DAY IN I4.4 FORMAT [0000,2359] */
		/*     RETURNS CUMULATIVE TIME [0,1439] [MINUTES] */

		hour = (int) (mtime / 100);
		minute = mtime - hour * 100;
		return minute + hour * 60;
	}
	//***********************************************************************

	//*********************************************************************** 
	/*******************************beam_flux 31JAN92************************
	  void Suntracker::beam_flux(){
	  double tempBeam = 0;

	  for (int tim = 0; tim <= 1439; ++tim) {
	  tempBeam =  nrmbflux[tim+1] * cos((pi/180.0) * incidentang[tim+1]);  
	  if(tempBeam < 0.0) 
	  tempBeam = 0.0;
	  beam[tim+1] = tempBeam;


	  }     
	  }
	  */

	//*********************************************************************** 
	//*******************************INCIDENT 31JAN92************************


	/*********************************************************************** 
	***************************SKY DIFFUSE FLUX 13APR00******************** 
	void Suntracker::sky_diffuse(double rslope){	//double rslope, double incidentang[], double apalt[], double nrmbflux[], 
	//double rDayAngle, double diffuse[]) {

	//     INPUT SURFACE SLOPE (DEGREES): RANGE [0=HORIZONTAL, 90=VERTICAL]
	//     INPUT INCIDENTANGLE (DEGREES): ARRAY OF BEAM-TO-SURF NORMAL ANGLES
	//     INPUT APALT (DEGREES):  ARRAY OF SOLAR ALTITUDE ANGLES
	//     INPUT NRMBFLUX (W/M**2): ARRAY OF BEAM FLUX ON SURF NORMAL TO BEAM
	//     INPUT DAYANGLE (RADIANS)
	//     OUTPUT DIFFUSE (W/M**2): ARRAY OF SKY-DIFFUSE FLUX ONTO SLOPE SURF     

	double theta, thetaz, param_c,sky;
	int tim;

	for (tim = 0; tim <= 1439; ++tim) {
	theta = pi / 180.0 * incidentang[tim+1];

	*     THETAZ IS SOLAR ZENITH ANGLE 
	thetaz = pi / 180.0 * (90.0 - apalt[tim+1]);
	param_c = sin(rDayAngle + 1.49618) * - 0.041117 + 0.09037;
	sky = param_c * nrmbflux[tim+1];

	diffuse[tim+1] = .5 * sky *  (cos(rslope) + 1.0) * ( pow(sin(rslope / 2.0),3) + 1.0) * 
	(1.0 + pow(cos(theta),2) * pow(sin(thetaz),3));

	}
	}



	   double Suntracker::d_mod(double x, double y)
	   {
	   double quotient;
	   if( (quotient = x / y) >= 0)
	   quotient = floor(quotient);
	   else
	   quotient = -floor(-quotient);
	   return(x - (y) * quotient );

	   }

	   int Suntracker::round(double num){
	   int temp = (int)num;

	   num -=temp;
	   num *= 10;
	   if (num >= 5)
	   temp ++;
	   return temp;

	   }
	   */

}
