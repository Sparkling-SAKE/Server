#pragma once

#define OUT

/*---------------
	   Log
---------------*/
#define LOG(fmt,...) printf("[LOG] %s:%d " fmt "\n", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define ERR(fmt,...) printf("\033[0;31m[ERR] %s:%d " fmt "\n\033[0m", __FUNCTION__, __LINE__, __VA_ARGS__ )
#define WAR(fmt,...) printf("\033[0;33m[WAR] %s:%d " fmt "\n\033[0m", __FUNCTION__, __LINE__, __VA_ARGS__ )


/*---------------
	  Crash
---------------*/

#define CRASH(cause)						\
{											\
	uint32* crash = nullptr;				\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xDEADBEEF;					\
}

#define ASSERT_CRASH(expr)			\
{									\
	if (!(expr))					\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}