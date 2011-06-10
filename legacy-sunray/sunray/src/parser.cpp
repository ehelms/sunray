
#include "parser.h"

#include <boost/spirit/core.hpp>
#include <boost/spirit/file_iterator.hpp>


bool Parser::parseLine ( std::string line ) const
{
	parse_info<> info = parse(line.begin(), line.end(), getSyntax());
	return ( info.full == 1 );
}

bool Parser::parseFile ( std::string filename ) const
{
	file_iterator<char> first(filename);
	if ( !first ) {
		std::cerr << "Unable to open configuration file!" << std::endl;
		return false;
	}
	file_iterator<char> last = first.make_end();

	parse_info<file_iterator<char> > info = parse(first, last, getSyntax());
	return ( info.full == 1 );
}


#endif // _PARSER_H_

