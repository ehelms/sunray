
#ifndef _PARSER_H_
#define _PARSER_H_

#include <string>


template <typename GrammarT>
class Parser
{
	public:
		// TODO the grammar needs to be constrained to subclasses of
		// boost::spirit::grammar
		Parser ( ) { }
		virtual ~Parser ( ) { }

		/**
		 * Parses a string.
		 *
		 * @param string line to parse
		 * @return bool whether line was fully parsed
		 */
		bool parseLine ( std::string ) const;

		/**
		 * Parses a full file.
		 *
		 * @param string filename to parse
		 * @return bool whether the file was fully parsed
		 */
		bool parseFile ( std::string ) const;

	protected:
		/**
		 * Gets the grammar used for parsing. Its type is determined
		 * by the template argument provided for class construction.
		 *
		 * @return boost::spirit::grammar grammar
		 */
		const GrammarT getGrammar ( ) const {
			return grammar;
		}

	private:
		GrammarT grammar;

};


#endif // _PARSER_H_

