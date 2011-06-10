#ifndef HttpPullData_H_
#define HttpPullData_H_
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <curl/curl.h>
#include <libxml/HTMLparser.h>
#include "../StringTools.h"
using namespace std;
#ifdef _MSC_VER
#define COMPARE(a, b) (!stricmp((a), (b)))
#else
#define COMPARE(a, b) (!strcasecmp((a), (b)))
#endif

enum PARSE_ACTION {GET_OPTIONS,GET_OZONE,GET_LOCATION,GET_AEROSOL_UV,GET_AEROSOL_VISIBLE};

struct Context{
	Context(): addOption(false),addTableText(false),addBoldText(false){ }

	bool addOption;
	bool addTableText;
	bool addBoldText;
	vector<string> options;
	vector<string> boldText;
	vector<string> tableText;
};

struct LocationInfo{
	double latitude;
	double longitude;
	double elevation;
	string ecoregion;
};

struct AerosolDepth{
	AerosolDepth(){
		size=7;
		freqs[0]=332;
		freqs[1]=368;
		freqs[2]=415;
		freqs[3]=500;
		freqs[4]=610;
		freqs[5]=665;
		freqs[6]=860;
		for(int i=0;i<size;i++)depthsAM[i]=depthsPM[i]=-1;
	}
	int size;
	double freqs[7];
	double depthsAM[7];
	double depthsPM[7];
};

static const int POST_LEN=512;
static char errorBuffer[CURL_ERROR_SIZE];

class HttpPullData{
private:
	static char* OZONE_POST_URL;
	static char* OZONE_POST_STR;
	static char* OZONE_GET_STR;
	static char* LOC_POST_URL;
	static char* LOC_POST_STR;
	static char* LOC_GET_STR;
	static char* AEROSOL_POST_URL;
	static char* AEROSOL_POST_STR;
	static char* AEROSOL_GET_STR;
	int buffer_offset;
	LocationInfo info;
	AerosolDepth aerosols;
	vector<string> options;
	double ozone;
	
	string buffer;
	static htmlSAXHandler saxHandler;
	CURL *curl;
	static int writer(char *data, size_t size, size_t nmemb,std::string *writerData){
	  if (writerData == NULL)return 0;
	  writerData->append(data, size*nmemb);
	  return size * nmemb;
	}
	static char* getKeyValue(const xmlChar **attributes,const char* key){
  	  for(int i=0;attributes[i]!=NULL;i++){
  		if(COMPARE((char *)attributes[i],key)){
  			return (char *)attributes[i+1];
  		}
  	  }
  	  return NULL;
    }

	CURLcode retrieveOzone(const char* townName,const char* startYear,const char* startMonth,const char* startDay);
	CURLcode retrieveAerosol(const char* townName,const char* startYear,const char* startMonth,const char* startDay,PARSE_ACTION action);
	CURLcode retrieveLoc(const char* townName);
	CURLcode initLocServer();
	CURLcode initOzoneServer();
	CURLcode initAerosolServer();
	void parseHtml(const std::string &html,int pos,PARSE_ACTION action);
	static void StartElement(void *voidContext,const xmlChar *name,const xmlChar **attributes){
	  Context *context = (Context *)voidContext;
	  //cout<<"<"<<name<<">"<<endl;
	  if (COMPARE((char *)name, "SELECT")&&COMPARE(getKeyValue(attributes,"NAME"),"townName")){
	  	context->addOption=true;
	  }
	  if(COMPARE((char *)name, "OPTION")&&context->addOption){
	  	char *c=getKeyValue(attributes,"VALUE");
	    if(c){
	    	string str(c);
	    	context->options.push_back(str); 
	    }
	  }
	  if(COMPARE((char *)name, "TD")){
	  	context->addTableText=true;
	  }
	  if(COMPARE((char *)name,"B")){
	  	context->addBoldText=true;
	  }
	}
	static void EndElement(void *voidContext,const xmlChar *name){
	  Context *context = (Context *)voidContext;
	  //cout<<"</"<<name<<">"<<endl;
	  if(COMPARE((char *)name, "SELECT")&&context->addOption){
	  	context->addOption=false;
	  }
	  if(COMPARE((char *)name, "TD")){
	  	context->addTableText=false;
	  }
	  if(COMPARE((char *)name,"B")){
	  	context->addBoldText=false;
	  }
	}
	static void handleCharacters(Context *context,const xmlChar *chars,int length){
	  string tmp((char *)chars,length);
	  if(context->addTableText){
	  	context->tableText.push_back(tmp);
	  }
	  if(context->addBoldText){
	    context->boldText.push_back(tmp);
	  }
	}
	static void Characters(void *voidContext,const xmlChar *chars,int length){
	  Context *context = (Context *)voidContext;
	  handleCharacters(context, chars, length);
	}
	static void cdata(void *voidContext,const xmlChar *chars,int length){
	  Context *context = (Context *)voidContext;
	  handleCharacters(context, chars, length);
	}

public:

	AerosolDepth getAerosols(int loc,int month,int day,int year);
	LocationInfo getLocationInfo(int loc);
	const vector<string>& getLocations(){return options;}
	double getOzone(int loc,int month,int day,int year);
	HttpPullData();
	virtual ~HttpPullData();
};
#endif /*HttpPullData_H_*/
