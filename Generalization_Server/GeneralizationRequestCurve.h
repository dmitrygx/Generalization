#pragma once

#include "GeneralizationCurve.h"
#include <map>
#include <vector>
#include <utility>
#include <functional>
#include <iostream>

using namespace std;

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
public:
	GeneralizationRequestCurve();
	GeneralizationRequestCurve::GeneralizationRequestCurve(
		double_t C_ = 0.5, uint32_t Np_ = 500, uint32_t Ns_ = 50,
		double_t f_ = 5, uint32_t Ninit_ = 1000);
	GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve);
	GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve, 
		double_t C_ = 0.5, uint32_t Np_ = 500, uint32_t Ns_ = 50,
		double_t f_ = 5, uint32_t Ninit_ = 1000);
	void SetCurve(uint32_t newCountOfPoints, curve *newCurve);
	void SetDBInfo(string dbCode, long dbNumber);
	string GetDBCode();
	long GetDBNumber();
	size_t DispatchEvent(Event_t newEvent);
	virtual ~GeneralizationRequestCurve();
	void SetVerbose(bool value)
	{
		verbose = value;
	}
	bool GetVerbose(void)
	{
		return verbose;
	}
};

