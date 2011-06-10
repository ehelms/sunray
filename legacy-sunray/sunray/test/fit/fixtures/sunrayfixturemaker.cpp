#include "sunrayfixturemaker.h"

int main ( int argc, char *argv[] ) {
	FixtureMaker* maker = new SunrayFixtureMaker;
	return FitnesseServer::Main(argc, argv, maker);
}

