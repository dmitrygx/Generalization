#pragma once

#include "GeneralizationCurve.h"
#include "GeneralizationLogging.h"
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <iostream>
#include <boost/timer/timer.hpp>
#include <ctime>
#include <iomanip>

#define InitTimerGetter(name)		\
double Get##name##Timer(void)		\
{					\
	return timer.name##_timer;	\
}

#define StartTime(time)							\
	cpu_timer (time)

#define PauseTime(time)							\
	(time).stop()

#define ResumeTime(time)						\
	(time).resume()

#define StopTime(time, timer, type)					\
do {									\
	(time).stop();							\
	(timer).type##_timer =						\
		((double)(time).elapsed().wall) / 1000000000.0;		\
} while (0)


#define VerboseTime(timer, type)					\
if (GeneralizationLogging::GetVerbose())				\
{									\
	std::printf("%f sec \n", (timer).type##_timer);			\
}

#define PrintTimer(name, time)						\
	GeneralizationLogging::fout << "\t"name": " <<			\
	std::setprecision(7) << this->Get##time##Timer() << std::endl;

#define VerboseTimeResults(timer)							\
if (GeneralizationLogging::GetOutputResults())						\
{											\
	GeneralizationLogging::fout << "Curve " << Code << ":" << Number << std::endl;	\
	PrintTimer("Adduction", adduction);						\
	PrintTimer("Segmentation", segmentation);					\
	PrintTimer("Simplification", simplification);					\
	PrintTimer("Smoothing", smoothing);						\
	PrintTimer("Total", total);							\
}

using namespace std;
using namespace boost::timer;


typedef enum State
{
	UNITIALIZED	= -1,
	INITIALIZED	= 0,
	ADDUCTED	= 1,
	SEGMENTED	= 2,
	SIMPLIFIED	= 3,
	SMOOTHED	= 4,
} State_t;

typedef enum Event
{
	INITIALIZE	= 0,
	ADDUCTION	= 1,
	SEGMENTATION	= 2,
	SIMPLIFICATION	= 3,
	SMOOTHING	= 4,
} Event_t;

struct GeneralizationTimer
{
	double adduction_timer;
	double segmentation_timer;
	double simplification_timer;
	double smoothing_timer;

	double total_timer;
	cpu_timer *total_time;
};

class GeneralizationRequestCurve :
	public GeneralizationCurve
{
private:
	using callBackFunction = std::function<void()>;
	uint32_t countOfPoints;
	curve *Curve;
	
	string Code;
	long Number;

	State_t state;
	Event_t event;
	map<pair<State_t, Event_t>, callBackFunction> matrix;
	bool verbose;

	void addMemberFunction(pair<State_t, Event_t> forPair, callBackFunction methodName);
	void Initialize();

	GeneralizationTimer timer;
public:
	GeneralizationRequestCurve();
	GeneralizationRequestCurve(
		double_t C_, uint32_t Np_, uint32_t Ns_,
		double_t f_, uint32_t Ninit_, int parallelism);
	GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve,
		double_t C_, uint32_t Np_, uint32_t Ns_,
		double_t f_, uint32_t Ninit_, int parallelism);
	GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve);
	void SetCurve(uint32_t newCountOfPoints, curve *newCurve);
	void SetDBInfo(string dbCode, long dbNumber);
	string GetDBCode();
	long GetDBNumber();
	size_t DispatchEvent(Event_t newEvent);
	virtual ~GeneralizationRequestCurve();
	InitTimerGetter(adduction);
	InitTimerGetter(segmentation);
	InitTimerGetter(simplification);
	InitTimerGetter(smoothing);
	InitTimerGetter(total);

	void OutputResults(void)
	{
		VerboseTimeResults(timer);
	}
};

