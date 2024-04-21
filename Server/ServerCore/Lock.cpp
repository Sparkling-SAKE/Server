#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	uint16 lockThreadId = (_flag.load() & WRITE_THREAD_MASK) >> 16;

	if (LThreadId == lockThreadId)
	{
		_flag.fetch_add(1);
		return;
	}

	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			uint32 expected = (_flag.load() & READ_COUNT_MASK);
			if (_flag.compare_exchange_strong(expected, expected + 1))
			{
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			ERR("ReadLock timeout!!!");
			CRASH("READ_LOCK_TIMEOUT");
		}

		this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if ((_flag.fetch_sub(1) & READ_COUNT_MASK) == 0)
	{
		ERR("ReadUnLock Error!!!");
		CRASH("MULTIPLE_UNLOCK");
	}
}

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	uint16 lockThreadId = (_flag.load() & WRITE_THREAD_MASK) >> 16;
	if (lockThreadId == LThreadId)
	{
		_writeCount++;
		return;
	}

	uint32 expected = EMPTY_FLAG;
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	uint64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; ++spinCount)
		{
			if (_flag.compare_exchange_strong(expected, desired) == true)
			{
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
		{
			ERR("WriteLock timeout!!!");
			CRASH("WRITE_LOCK_TIMEOUT");
		}

		this_thread::yield();
	}
}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if ((_flag.load() & READ_COUNT_MASK) != 0)
	{
		ERR("ReadLock is not unlocked!!!");
		CRASH("INVALID_UNLOCK");
	}

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
	{
		_flag.store(EMPTY_FLAG);
	}
}
