
#ifndef _CONFIG_PARSER_H_
#define _CONFIG_PARSER_H_

#include <string>

#include "parser.h"
#include "configsyntax.h"

typedef Parser<ConfigSyntax> ConfigParser;

template <typename ValueT>
class ConfigProperty
{
	public:
		ConfigProperty ( std::string k ) : key(k) { }
		ConfigProperty ( std::string k, ValueT val ) : key(k), value(val) { }
		virtual ~ConfigProperty ( ) { }

		void setValue ( ValueT val ) { value = val; }
		ValueT getValue ( ) { return value; }

	private:
		std::string key;
		ValueT value;
};

class ConfigScope
{
	public:
		ConfigScope ( std::string s ) : name(s) { }
		virtual ~ConfigScope ( ) { }

		std::string getName ( ) { return name; }
		std::string getProperty ( std::string key ) {
			return properties[key].getValue();
		}
		void addProperty ( ConfigProperty );

	protected:
		std::map<std::string, ConfigProperty> properties;

	private:
		std::string name;

};


class ConfigEnvironment
{
	public:
		ConfigProperty hasProperty ( std::string );
		ConfigProperty getProperty ( std::string );
		ConfigProperty hasProperty ( std::string, std::string );
		ConfigProperty getProperty ( std::string, std::string );

		void addConfigScope ( std::string );
		void addProperty ( std::string );
		void addProperty ( std::string, std::string );

	private:
		std::map<std::string, ConfigScope> globalScopes;
		std::map<std::string, ConfigProperty> globalProperties;

};

#endif // _CONFIG_PARSER_H_

