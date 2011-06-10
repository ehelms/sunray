
#ifndef _CONFIG_SYNTAX_H_
#define _CONFIG_SYNTAX_H_

//#define BOOST_SPIRIT_DEBUG

#include <string>
#include <iostream>
#include <boost/spirit/core.hpp>
#include <boost/spirit/iterator/file_iterator.hpp>


using namespace boost::spirit;

namespace
{
	void addSection ( file_iterator<char> first, file_iterator<char> last ) {
		// for now, we're just ignoring section headers, there is no modular configuration
	}

	void handleKey ( file_iterator<char> first, file_iterator<char> last ) {
		std::string name(first, last);
	}

	void handleValue ( file_iterator<char> first, file_iterator<char> last ) {
		std::string value(first, last);
	}
}

class ConfigSyntax : public grammar<ConfigSyntax>
{

	public:
		template <typename ScannerT>
		struct definition
		{
			definition ( ConfigSyntax const &self ) {

				// we're going to ignore blank lines and comments
				empty = *blank_p >> eol_p;
				comment = *blank_p >> ch_p('#') >> *(anychar_p - eol_p) >> eol_p;

				// an identifier has constrants, the value can be anything printable
				ident = ( alpha_p | ch_p('_') ) >> *( alpha_p | ch_p('_') );
				value = *( alnum_p | blank_p | punct_p );
				
				// each section has several 'content' lines
				sectionHeader = *blank_p 
						>> ch_p('[') >> *blank_p >> ident[&addSection] >> *blank_p >> ch_p(']') 
						>> *blank_p >> eol_p;
				sectionContents = *blank_p
						>> ident[&handleKey] >> *blank_p >> ch_p('=') >> *blank_p >> value[&handleValue] 
						>> *blank_p >> eol_p;
				section = sectionHeader >> *( sectionContents | empty | comment );

				// the configuration file is made up of zero or more sections
				config = *( section | empty | comment ) >> end_p;

#if 0
				BOOST_SPIRIT_DEBUG_NODE(config);
				BOOST_SPIRIT_DEBUG_NODE(empty);
				BOOST_SPIRIT_DEBUG_NODE(comment);
				BOOST_SPIRIT_DEBUG_NODE(commentEnd);
				BOOST_SPIRIT_DEBUG_NODE(section);
				BOOST_SPIRIT_DEBUG_NODE(sectionHeader);
				BOOST_SPIRIT_DEBUG_NODE(sectionContents);
#endif 
			}

			rule<ScannerT> config, empty, commentStart, commentEnd, comment, section, 
					sectionHeader, sectionContents, ident, value;
			rule<ScannerT> const &start ( ) const { return config; }
		};

};


#endif // _CONFIG_SYNTAX_H_

