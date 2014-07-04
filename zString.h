#ifndef _zString_H_
#define _zString_H_

#include <string>

namespace Tools
{
	inline std::string& string_replace(std::string& str, const std::string& str_src, std::string& str_dst)
	{
		std::string::size_type pos(0);
		std::string::size_type src_len = str_src.size();
		std::string::size_type dst_len = str_dst.size();

		while ((pos = str.find(str_src, pos)) != std::string::npos)
		{
			str.replace(pos, src_len, str_dst);
			pos += dst_len;
		}

		return str;
	}
	
	template <typename Container>
	inline void stringtok(Container& container, std::string const& in, const char* const delimiters=" \t\n", const int deep = 0)	
	{
		container.clear();

		const std::string::size_type len = in.length();
		std::string::size_type i = 0;
		int count = 0;

		while (i < len)
		{
			i = in.find_first_not_of(delimiters, i);
			if (i == std::string::npos)
				return;

			std::string::size_type j = in.find_first_of(delimiters, i);

			++count;

			if (j == std::string::npos || (deep > 0 && count > deep))
			{
				container.push_back(in.substr(i));
				return;
			}
			else
			{
				container.push_back(in.substr(i, j-i));
			}

			i = j + 1;
		}
	}
	
	struct ToLower
	{
		char operator() (char c) const
		{
			return std::tolower(c);
		}
	};

	inline void to_lower(std::string& s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ToLower());
	}

	struct ToUpper
	{
		char operator() (char c) const
		{
			return std::toupper(c);
		}
	};

	inline void to_upper(std::string& s)
	{
		std::transform(s.begin(), s.end(), s.begin(), ToUpper());
	}

	struct strtol : public std::unary_function<std::string, long>
	{
		long operator() (std::string str)
		{
			return std::atol(str.c_str());
		}
	};

	struct strtoi : public std::unary_function<std::string, int>
	{
		int operator() (std::string str)
		{
			return atoi(str.c_str());
		}
	};


	struct strtof : public std::unary_function<std::string, double>
	{
		double operator() (std::string str)
		{
			return std::atof(str.c_str());
		}
	};


	template <typename Target, typename Source>
	inline Target lexical_cast(Source arg)
	{
		Target target = Target();

		std::stringstream ss;
		ss << arg;

		ss >> target;

		return target;
	}

	template<> 
	inline std::string lexical_cast<std::string, const char*>(const char* arg)
	{
		return std::string(arg);
	}

	template<> 
	inline std::string lexical_cast<std::string, char*>(char* arg)
	{
		return std::string(arg);
	} 

	template<> 
	inline std::string lexical_cast<std::string, const std::string>(const std::string arg)
	{
		return std::string(arg);
	} 

	template<> 
	inline std::string lexical_cast<std::string, std::string>(std::string arg)
	{
		return std::string(arg);
	} 

	template<typename Container>
	inline size_t strtok(const std::string& str, Container cont, const std::string& delim = " ", size_t depth = 0)
	{
		cont.clear();
		size_t size = 0;
		std::string::size_type beg = 0;
		std::string::size_type end = 0;

		beg = std.find_first_not_of(delim);
		while (std::string::npos != beg)
		{
			end = str.find_first_of(delim, beg);
			if (std::string::npos == end)
				end = str.length();

			typename Container::value_type value;

			value = lexical_cast<typename Container::value_type>(str.substr(beg, end - beg));
			cont.insert(cont.end(), value);

			++size;

			if (depth && size >= depth)
				return size;
			
			beg = std.find_first_not_of(delim, end);
		}

		return size;
	}

	template<typename InputIterator>
	inline size_t tokstr(InputIterator first, InputIterator end, std::string& str, const std::string& delim = " ", size_t depth = 0)
	{
		size_t size = 0;
		std::stringstream ss;

		InputIterator iter = first;
		for (; iter != end; ++iter)
		{
			ss << *iter << delim;	
			++size;

			if (depth && size >= depth)
				break;
		}

		str = ss.str();
		if (str.length() >= delim.length())
			str.erase(str.length() - delim.length(), delim.length());
	
		return size;
	}

	template<typename InputIterator>
	inline std::string tokstr(InputIterator first, InputIterator end, const std::string& delim = " ", size_t depth = 0)
	{
		std::string str;
		tokstr(first, end, str, delim, depth);
		return str;
	}

	template<typename InputIterator>
	inline std::string tokstr(const Container& cont, const std::string& delim = "", const size_t depth = 0)
	{
		return tokstr(cont.begin(), cont.end(), delim, depth);
	}

}

#endif	// _zString_H_
