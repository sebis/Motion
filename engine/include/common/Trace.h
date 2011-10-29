#ifndef COMMON_TRACE
#define COMMON_TRACE

#include <cstdarg>
#include <cstdio>

namespace Trace
{
#if defined DEBUG || _DEBUG
#define debug info
#else
#define debug 0 && info
#endif

	inline void error(const char * format, ...)
	{
		char buffer[4096];
		va_list args;

		va_start (args, format);
		vsnprintf (buffer, 4096, format, args);
		va_end (args);

		fprintf(stderr, "[ERROR] %s", buffer);
	}

	inline void warning(const char * format, ...)
	{
		char buffer[4096];
		va_list args;

		va_start (args, format);
		vsnprintf (buffer, 4096, format, args);
		va_end (args);

		fprintf(stdout, "[WARNING] %s", buffer);
	}

	inline void info(const char * format, ...)
	{
		char buffer[4096];
		va_list args;

		va_start (args, format);
		vsnprintf (buffer, 4096, format, args);
		va_end (args);

		fprintf(stdout, "[INFO] %s", buffer);
	}
}

#endif /* COMMON_TRACE */
