#include "HttpPullData.h"
char* HttpPullData::OZONE_POST_URL="http://uvb.nrel.colostate.edu/UVB/servlet/ProductsServlet";
char* HttpPullData::OZONE_POST_STR="cmd=exePlot&townName=%s&startYear=%s&startMonth=%s&startDay=%s&plotLength=1&theStat=This+request+could+take+up+to+thirty+seconds.+Please+be+patient.";
char* HttpPullData::OZONE_GET_STR="cmd=Ozone";
char* HttpPullData::LOC_POST_URL="http://uvb.nrel.colostate.edu/UVB/servlet/SiteInformationServlet";
char* HttpPullData::LOC_POST_STR="cmd=exeSite&townName=%s&theStat=++Working.+This+request+could+take+up+to+thirty+seconds.+Please+be+patient.";
char* HttpPullData::LOC_GET_STR="cmd=getSites";
char* HttpPullData::AEROSOL_POST_URL="http://uvb.nrel.colostate.edu/UVB/servlet/ProductsServlet";
char* HttpPullData::AEROSOL_POST_STR="cmd=exePlot&townName=%s&insType=%s&startYear=%s&startMonth=%s&startDay=%s&plotLength=1&dayPart=both&theStat=++Working.+This+request+could+take+up+to+thirty+seconds.+Please+be+patient.";
char* HttpPullData::AEROSOL_GET_STR="cmd=AvgODs";
htmlSAXHandler HttpPullData::saxHandler={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,StartElement,EndElement,NULL,Characters,NULL,NULL,NULL,NULL,NULL,NULL,NULL,cdata,NULL};

HttpPullData::HttpPullData():buffer_offset(0){
  CURLcode res;
  curl = curl_easy_init();
  if(curl) {
  	curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
  	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
  	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
  	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = initLocServer();
    if (res != CURLE_OK) {
      fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    }
    
    parseHtml(buffer,0,GET_OPTIONS);
  }
}
double HttpPullData::getOzone(int loc,int month,int day,int year){
	
	retrieveOzone(options[loc].c_str(),StringTools::itos(year,10).c_str(),StringTools::itos(month,10).c_str(),StringTools::itos(day,10).c_str());
    if(ozone!=-1){
    	cout<<"Total column ozone for "<<options[loc]<<" on "<<month<<"/"<<day<<"/"<<year<<" OZONE="<<ozone<<" DU"<<endl;
    } else {
   		cout<<"Total column ozone for "<<options[loc]<<" on "<<month<<"/"<<day<<"/"<<year<<" NOT AVAILABLE"<<endl;
    }
    return ozone;
}
LocationInfo HttpPullData::getLocationInfo(int loc){
	LocationInfo in;
	info=in;
	retrieveLoc(options[loc].c_str());
    cout<<"Location info for "<<options[loc]<<" ECOREGION "<<info.ecoregion<<" LATITUDE "<<info.latitude<<" LONGITUDE "<<info.longitude<<" ELEVATION "<<info.elevation<<" FT"<<endl;
    return info;
}
AerosolDepth HttpPullData::getAerosols(int loc,int month,int day,int year){
	AerosolDepth a;
	aerosols=a;
	retrieveAerosol(options[loc].c_str(),StringTools::itos(year,10).c_str(),StringTools::itos(month,10).c_str(),StringTools::itos(day,10).c_str(),GET_AEROSOL_VISIBLE);
	retrieveAerosol(options[loc].c_str(),StringTools::itos(year,10).c_str(),StringTools::itos(month,10).c_str(),StringTools::itos(day,10).c_str(),GET_AEROSOL_UV);
   	cout<<"Aerosol Optical Depth for "<<options[loc]<<" on "<<month<<"/"<<day<<"/"<<year<<":"<<endl;
	for(int i=0;i<aerosols.size;i++){
		cout<<"\t"<<aerosols.freqs[i]<<" nm | AM Optical Depth="<<aerosols.depthsAM[i]<<"\t| PM Optical Depth="<<aerosols.depthsPM[i]<<endl;
	}
    //cout<<"LOCATION INFO FOR "<<options[loc]<<" ECOREGION "<<info.ecoregion<<" LATITUDE "<<info.latitude<<" LONGITUDE "<<info.longitude<<" ELEVATION "<<info.elevation<<" FT"<<endl;
    return aerosols;
}

	CURLcode HttpPullData::retrieveOzone(const char* townName,const char* startYear,const char* startMonth,const char* startDay){
		char* escTownName=(char *)curl_escape(townName,0);
		char* escStartYear=(char *)curl_escape(startYear,0);
		char* escStartMonth=(char *)curl_escape(startMonth,0);
		char* escStartDay=(char *)curl_escape(startDay,0);
		char postThis[POST_LEN];
		buffer.clear();
		initOzoneServer();
		buffer_offset=buffer.length();
		buffer.clear();
		sprintf(postThis,OZONE_POST_STR,escTownName,escStartYear,escStartMonth,escStartDay);
	    curl_easy_setopt(curl, CURLOPT_URL, OZONE_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postThis);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postThis));
	    CURLcode res=curl_easy_perform(curl);
	    if (res != CURLE_OK) {
      		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    		return res;
    	}
	    int pos=buffer.find("</HTML>")+strlen("</HTML>");
	    buffer="<HTML><BODY>"+buffer.substr(pos,buffer.length());
	    parseHtml(buffer,0,GET_OZONE);
	    return res;
	}
	CURLcode HttpPullData::retrieveAerosol(const char* townName,const char* startYear,const char* startMonth,const char* startDay,PARSE_ACTION action){
		char* escTownName=(char *)curl_escape(townName,0);
		char* escStartYear=(char *)curl_escape(startYear,0);
		char* escStartMonth=(char *)curl_escape(startMonth,0);
		char* escStartDay=(char *)curl_escape(startDay,0);
		const char* freqRange= (action==GET_AEROSOL_UV)?"UVRSR":"MFRSR";
		char postThis[POST_LEN];
		buffer.clear();
		initAerosolServer();
		buffer.clear();
		sprintf(postThis,AEROSOL_POST_STR,escTownName,freqRange,escStartYear,escStartMonth,escStartDay);
	    curl_easy_setopt(curl, CURLOPT_URL, AEROSOL_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postThis);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postThis));
	    CURLcode res=curl_easy_perform(curl);
	    if (res != CURLE_OK) {
      		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    		return res;
    	}
	    int pos=buffer.find("</HTML>")+strlen("</HTML>");
	    buffer="<HTML><BODY>"+buffer.substr(pos,buffer.length());
	    parseHtml(buffer,0,action);
	    return res;
	}
	CURLcode HttpPullData::retrieveLoc(const char* townName){
		char* escTownName=(char *)curl_escape(townName,0);
		char postThis[POST_LEN];
		buffer.clear();
		initLocServer();
		buffer_offset=0;
		buffer.clear();
		sprintf(postThis,LOC_POST_STR,escTownName);
	    curl_easy_setopt(curl, CURLOPT_URL, LOC_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postThis);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(postThis));
	    CURLcode res=curl_easy_perform(curl);
	   	if (res != CURLE_OK) {
      		fprintf(stderr, "Curl perform failed: %s\n", curl_easy_strerror(res));
    		return res;
    	}
    	parseHtml(buffer,0,GET_LOCATION);
	    return res;
	}
	
	CURLcode HttpPullData::initLocServer(){
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	    curl_easy_setopt(curl, CURLOPT_URL, LOC_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, LOC_GET_STR);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(LOC_GET_STR));
	    return curl_easy_perform(curl);
	}
	CURLcode HttpPullData::initOzoneServer(){
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	    curl_easy_setopt(curl, CURLOPT_URL, OZONE_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, OZONE_GET_STR);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(OZONE_GET_STR));
	    return curl_easy_perform(curl);
	}
	CURLcode HttpPullData::initAerosolServer(){
		curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
	    curl_easy_setopt(curl, CURLOPT_URL, AEROSOL_POST_URL);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, AEROSOL_GET_STR);
	    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(AEROSOL_GET_STR));
	    return curl_easy_perform(curl);
	}

	void HttpPullData::parseHtml(const std::string &html,int pos,PARSE_ACTION action){
	  htmlParserCtxtPtr ctxt;
	  Context context;
	  ctxt = htmlCreatePushParserCtxt(&saxHandler, &context, "", 0, "",XML_CHAR_ENCODING_NONE);
	  htmlParseChunk(ctxt,html.c_str()+pos,max((unsigned long)0,html.size()-pos), 0);
	  htmlParseChunk(ctxt, "", 0, 1);
	  htmlFreeParserCtxt(ctxt);
	  switch(action){
	  	case GET_OPTIONS:
	  		options=context.options;
	  		break;
	 	case GET_OZONE:
	 		ozone=-1;
	  		for(unsigned int i=0;i<context.tableText.size();i++){
	  			if(context.tableText[i].find(" DU",0)>=0){
	  				ozone=atof(StringTools::stripString(context.tableText[i]," DU").c_str());
	  				break;
	  			}
	  		}	 	
	  		break;
	  	case GET_LOCATION:
	  		for(int i=0;i<context.boldText.size();i++){
	  			if(context.boldText[i]=="Ecoregion "){
	  				info.ecoregion=context.boldText[i+1];
	  			} else if(context.boldText[i]=="Latitude"){
	  	  			info.latitude=atof(StringTools::stripString(context.boldText[i+1]," N").c_str());
	  			} else if(context.boldText[i]=="Longitude"){
	  				if((int)context.boldText[i+1].find(" W")>=0){
	  					info.longitude=atof(StringTools::stripString(context.boldText[i+1]," W").c_str());
	  				} else if((int)context.boldText[i+1].find(" E")>=0){
	  					info.longitude=-atof(StringTools::stripString(context.boldText[i+1]," E").c_str());
	  				}
	  			} else if(context.boldText[i]=="Elevation"){
	  				info.elevation=atof(context.boldText[i+1].c_str());
	  			}
	  		}
	  	case GET_AEROSOL_VISIBLE:
	  		for(int i=0;i<context.tableText.size();i++){
	  			if(context.tableText[i]==" Morning  "){
	  				aerosols.depthsAM[2]=StringTools::stof(context.tableText[i+1]);
	  				aerosols.depthsAM[3]=StringTools::stof(context.tableText[i+2]);
	  				aerosols.depthsAM[4]=StringTools::stof(context.tableText[i+3]);
	  				aerosols.depthsAM[5]=StringTools::stof(context.tableText[i+4]);
	  				aerosols.depthsAM[6]=StringTools::stof(context.tableText[i+5]);
	  			}
	  			if(context.tableText[i]==" Afternoon  "){
	  				aerosols.depthsPM[2]=StringTools::stof(context.tableText[i+1]);
	  				aerosols.depthsPM[3]=StringTools::stof(context.tableText[i+2]);
	  				aerosols.depthsPM[4]=StringTools::stof(context.tableText[i+3]);
	  				aerosols.depthsPM[5]=StringTools::stof(context.tableText[i+4]);
	  				aerosols.depthsPM[6]=StringTools::stof(context.tableText[i+5]);
	  			}
	  			
			}
			break;
	  	case GET_AEROSOL_UV:
	  		for(int i=0;i<context.tableText.size();i++){
	  			if(context.tableText[i]==" Morning  "){
	  				aerosols.depthsAM[0]=StringTools::stof(context.tableText[i+1]);
	  				aerosols.depthsAM[1]=StringTools::stof(context.tableText[i+2]);
	  			}
	  			if(context.tableText[i]==" Afternoon  "){
	  				aerosols.depthsPM[0]=StringTools::stof(context.tableText[i+1]);
	  				aerosols.depthsPM[1]=StringTools::stof(context.tableText[i+2]);
	  			}
	  			
			}
			break;
		default:
	  		break;
	  }
	}
HttpPullData::~HttpPullData(){
	if(curl)curl_easy_cleanup(curl);
}

