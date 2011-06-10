#include "logger.h"

std::ofstream Logger::logFile("../log/sunray.log");

void Logger::debug ( std::string msg )
{
	write(msg, "debug");
}

void Logger::warn ( std::string msg )
{
	write(msg, "warn");
}

void Logger::error ( std::string msg )
{
	write(msg, "error");
}

void Logger::write ( std::string msg, std::string type )
{
	// create time string
	std::time_t tstamp = std::time(0);
	std::tm *tstruct = std::localtime(&tstamp);
	char *tformat = "%X %d-%m-%Y";
	char tlen = 32;
	char time[tlen];
	strftime(time, tlen, tformat, tstruct);
	
	logFile << time << " [" << type << "] " << msg << std::endl;
}

