#pragma once

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK	= 10000,
		MAX_SPIN_COUNT			= 5000,
		WRITE_THREAD_MASK		= 0xFFFF'0000,
		READ_COUNT_MASK			= 0x0000'FFFF,
		EMPTY_FLAG				= 0x0000'0000
	};

public:
	Lock() = default;
	~Lock() = default;

	void ReadLock(const char* name);
	void ReadUnLock(const char* name);
	void WriteLock(const char* name);
	void WriteUnLock(const char* name);

private:
	Atomic<uint32>	_flag = EMPTY_FLAG;
	uint16			_writeCount = 0;
};
