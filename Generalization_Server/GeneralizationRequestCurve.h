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
	UNITIALIZED = -1,
	INITIALIZED = 0,
} State_t;

typedef enum Event
{
	INITIALIZE = 0,
} Event_t;

class GeneralizationRequestCurve :
	public GeneralizationCurve
{
private:
	using callBackFunction = std::function<void()>;
	uint32_t countOfPoints;
	curve *Curve;

	State_t state;
	Event_t event;
	map<pair<State_t, Event_t>, callBackFunction> matrix;

	void addMemberFunction(pair<State_t, Event_t> forPair, callBackFunction methodName);
public:
	GeneralizationRequestCurve();
	GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve);
	void GeneralizationRequestCurve::SetCurve(uint32_t newCountOfPoints, curve *newCurve);
	size_t DispatchEvent(Event_t newEvent);
	virtual ~GeneralizationRequestCurve();
};

