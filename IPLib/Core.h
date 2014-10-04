#pragma once

#include "windows.h"
#include <CRTDBG.h>
#include "malloc.h"

#include "Time.h"

#include <QTextStream>

#ifdef _DEBUG
#define ASSERT(X)	{if(!(X)) _CrtDbgBreak();}
#define VERIFY(X) ASSERT(X)
#else	// Release
#define ASSERT(X)
#define VERIFY(X) X
#endif


class StringBuilder : public QTextStream
{
public:
	typedef QTextStream base;
	StringBuilder() : base(&mBuffer) {}

	QString toString() const { return mBuffer; }
protected:
	QString mBuffer;
};

class ClockTimer
{
public:
	ClockTimer() : mStart(clock()){}

	__declspec(property(get=getSeconds)) double seconds;
	__declspec(property(get=getMilliSeconds)) double milliSeconds;

	double getSeconds()
	{
		double result = (double)(clock() - mStart)/(double)CLOCKS_PER_SEC;
		mStart = clock();
		return result;
	}

	double getMilliSeconds()
	{
		return getSeconds()*1000;
	}

	void reset()
	{
		mStart = clock();
	}

private:
	clock_t  mStart;
};

