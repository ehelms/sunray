#include "HttpPullData.h"

int main() {

	HttpPullData httpPull;

	const vector<string>& locations=httpPull.getLocations();

	//display list of locations from the website
	for(int i=0;i<locations.size();i++){
    	cout<<i<<")"<<locations[i]<<endl;
    }
	if(locations.size()==0)return 0;
    //pull the ozone level for the given location index from the website.
    //getOzone returns total column ozone in Dobsian Units (DU)
    //If the ozone is unavailable, -1 is returned
	httpPull.getOzone(0,8,5,2005);
	httpPull.getOzone(0,5,6,2005);
	httpPull.getAerosols(0,5,1,2006);

	httpPull.getLocationInfo(0);
	return 0;
}
