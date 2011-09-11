#ifndef COMMON_UTILS
#define COMMON_UTILS

#include <string>
#include <fstream>

namespace Utils
{
	inline std::string readfile(const std::string filename)
	{
		std::ifstream ifs(filename.c_str());
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		return str;
	}
}

#endif /* COMMON_UTILS */