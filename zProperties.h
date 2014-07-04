#ifndef _zProperties_h_
#define _zProperties_h_

#include <iostream>
#include <string>
#include <algorithm>

class zProperties
{
	public:
		
		const std::string& GetProperty(const std::string& key)
		{
			return properties[key];
		}

		void SetProperty(const std::string& key, const std::string& value)
		{
			properties[key] = value;
		}

		std::string& operator[](const std::string& key)
		{
			return properties[key];
		}

		bool find(const std::string& key) const
		{
			property_hashtype::const_iterator it;
			it = properties.find(key);
			if (it == properties.end())
				return false;
			else 
				return true;
		}

		void dump(std::ostream& out)
		{
			property_hashtype::const_iterator it;
			for (it = properties.begin(); it != properties.end(); ++it)
			{
				out << it->first << " = " << it->second << std::endl;
			}
		}

		unsigned int ParseCmdLine(const std::string& cmd_line);
		unsigned int ParseCmdLine(const char* cmd_line);

	protected:

		struct key_hash : public std::unary_function<const std::string, const std::string, bool>
		{
			bool operator()(const std::string& s1, const std::string& s2) const
			{
				return strcasecmp(s1.c_str(), s2.c_str()) == 0;
			}
		};

		typedef __gun_cxx::hash_map<std::string, std::string. key_hash, key_equal> property_hashtype;

		property_hashtype properties;
};

#endif  // _zProperties_h_
