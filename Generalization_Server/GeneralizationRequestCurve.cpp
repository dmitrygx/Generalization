#include "GeneralizationRequestCurve.h"


void GeneralizationRequestCurve::addMemberFunction(pair<State_t, Event_t> forPair, callBackFunction methodName)
{
	matrix[forPair] = methodName;
}

GeneralizationRequestCurve::GeneralizationRequestCurve()
{
	state = State_t::UNITIALIZED;

	Initialize();
}

GeneralizationRequestCurve::GeneralizationRequestCurve(
	double_t C_, uint32_t Np_, uint32_t Ns_,
	double_t f_, uint32_t Ninit_) :
	GeneralizationCurve(C_, Np_, Ns_, f_, Ninit_)
{
	state = State_t::UNITIALIZED;

	Initialize();
}

GeneralizationRequestCurve::GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve)
{
	state = State_t::UNITIALIZED;
	Curve = newCurve;
	countOfPoints = newCountOfPoints;

	Initialize();
}

GeneralizationRequestCurve::GeneralizationRequestCurve(uint32_t newCountOfPoints, curve *newCurve,
	double_t C_, uint32_t Np_, uint32_t Ns_,
	double_t f_, uint32_t Ninit_) :
	GeneralizationCurve(C_, Np_, Ns_, f_, Ninit_)
{
	state = State_t::UNITIALIZED;
	Curve = newCurve;
	countOfPoints = newCountOfPoints;

	Initialize();
}

void GeneralizationRequestCurve::Initialize()
{
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

		state = State_t::INITIALIZED;
	}
	);

	pair<State_t, Event_t> pair_Init_Adduct = make_pair(INITIALIZED, ADDUCTION);
	addMemberFunction(
		pair_Init_Adduct,
		[&] {
		if (!Curve || countOfPoints == 0)
		{
			std::cout << "Error: Count of points = " << countOfPoints << endl;
			return;
		}
		this->Adduction();

		state = State_t::ADDUCTED;
	}
	);

	pair<State_t, Event_t> pair_Adduct_Segm = make_pair(ADDUCTED, SEGMENTATION);
	addMemberFunction(
		pair_Adduct_Segm,
		[&] {
		if (!Curve || countOfPoints == 0)
		{
			std::cout << "Error: Count of points = " << countOfPoints << endl;
			return;
		}
		this->Segmentation();

		state = State_t::SEGMENTED;
	}
	);

	pair<State_t, Event_t> pair_Segm_Simpl = make_pair(SEGMENTED, SIMPLIFICATION);
	addMemberFunction(
		pair_Segm_Simpl,
		[&] {
		if (!Curve || countOfPoints == 0)
		{
			std::cout << "Error: Count of points = " << countOfPoints << endl;
			return;
		}
		this->Simplification();

		state = State_t::SIMPLIFIED;
	}
	);
}

void GeneralizationRequestCurve::SetCurve(uint32_t newCountOfPoints, curve *newCurve)
{
	Curve = newCurve;
	countOfPoints = newCountOfPoints;
}

void GeneralizationRequestCurve::SetDBInfo(string dbCode, long dbNumber)
{
	Code = dbCode;
	Number = dbNumber;
}

string GeneralizationRequestCurve::GetDBCode()
{
	return Code;
}

long GeneralizationRequestCurve::GetDBNumber()
{
	return Number;
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
