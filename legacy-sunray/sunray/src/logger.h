#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <string>
#include <fstream>

/**
 * Provides logging functionality by writing to a log file using 
 * one of three tags: debug, warn, or error
 */
class Logger
{
	public:
		void static debug ( std::string );
		void static warn ( std::string );
		void static error ( std::string );

		static std::ofstream logFile;

	private:
		void static write ( std::string, std::string );

};

#endif // _LOGGER_H_

