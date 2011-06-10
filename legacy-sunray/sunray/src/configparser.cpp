
#include <iostream>
#include <map>
#include <boost/spirit/core.hpp>

using namespace boost::spirit;


bool ConfigParser::parseLine ( std::string )
{
	return false;
}

bool ConfigFileParser::parseFile ( std::string )
{
	file_iterator<char> first(filename);
	if ( !first ) {
		std::cerr << "Unable to open configuration file!" << std::endl;
		return;
	}
	file_iterator<char> last = first.make_end();

	parse_info<file_iterator<char> > info = parse(first, last, getSyntax());

	bool retr = false;
	if ( info.full == 1 ) {
		retr = true;
	}
	return retr;
}


int main ( int argc, char *argv[] )
{
	std::string filename(argv[1]);
	std::cout << "using filename " << filename << std::endl;
	ConfigParser parser(filename);

	return 0;
}

