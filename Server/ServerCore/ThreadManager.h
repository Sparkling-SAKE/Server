#pragma once

#include <thread>
#include <functional>

class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	static void InitTLS();
	static void DestroyTLS();

	void Launch(function<void(void)> callback);
	void Join();

private:
	Mutex			_lock;
	vector<thread>	_threads;
};

