#pragma once

#include <Windows.h>

class Mutex {
private:
	bool isFree;
public:
	void lock() {
		while (!isFree) Sleep(1);
		isFree = 0;
	}
	bool tryLock() {
		if (isFree) {
			isFree = 0;
			return 1;
		}
		else {
			return 0;
		}
	}
	void unlock() {
		isFree = 1;
	}
	Mutex() {
		isFree = 1;
	}
};