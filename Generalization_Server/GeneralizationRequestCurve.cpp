#include "GeneralizationRequestCurve.h"


void GeneralizationRequestCurve::addMemberFunction(pair<State_t, Event_t> forPair, callBackFunction methodName)
{
	matrix[forPair] = methodName;
}

GeneralizationRequestCurve::GeneralizationRequestCurve()
{
	state = State_t::UNITIALIZED;

	pair<State_t, Event_t> pair_Unit_Init = make_pair(UNITIALIZED, INITIALIZE);
	addMemberFunction(
		pair_Unit_Init,
		[&] {
		if (!Curve || countOfPoints == 0)
		{
			std::cout << "Error: Count of points = " << countOfPoints << endl;
			return;
		}
		this->BuildCurve(countOfPoints, Curve);
	}
	);
}

GeneralizationRequestCurve::GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve)
{
	state = State_t::UNITIALIZED;
	Curve = newCurve;
	countOfPoints = newCountOfPoints;

	pair<State_t, Event_t> pair_Unit_Init = make_pair(UNITIALIZED, INITIALIZE);
	addMemberFunction(
		pair_Unit_Init,
		[&] {
			if (!Curve || countOfPoints == 0)
			{
				std::cout << "Error: Count of points = " << countOfPoints << endl;
				return;
			}
			this->BuildCurve(countOfPoints, Curve);
		}
	);
}

void GeneralizationRequestCurve::SetCurve(uint32_t newCountOfPoints, curve *newCurve)
{
	Curve = newCurve;
	countOfPoints = newCountOfPoints;
}

size_t GeneralizationRequestCurve::DispatchEvent(Event_t newEvent)
{
	pair<State_t, Event_t> reqPair = make_pair(state, newEvent);

	if (!matrix.at(reqPair))
	{
		return -1;
	}
	else
	{
		matrix[reqPair]();
		return 0;
	}
}


GeneralizationRequestCurve::~GeneralizationRequestCurve()
{
}
