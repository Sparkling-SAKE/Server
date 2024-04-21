#pragma once

#define OUT

/*---------------
	  Lock
---------------*/

#define USE_MANY_LOCKS(count)	Lock _locks[count];
#define USE_LOCK				USE_MANY_LOCKS(1)
#define	READ_LOCK_IDX(idx)		ReadLockGuard readLockGuard_##idx(_locks[idx], typeid(this).name());
#define READ_LOCK				READ_LOCK_IDX(0)
#define	WRITE_LOCK_IDX(idx)		WriteLockGuard writeLockGuard_##idx(_locks[idx], typeid(this).name());
#define WRITE_LOCK				WRITE_LOCK_IDX(0)


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