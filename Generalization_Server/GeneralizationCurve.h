#pragma once

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <map>

#include "common.h"

#define X(point) ((point).first)
#define Y(point) ((point).second)

class GeneralizationCurve
{
private:
	double_t C;
	uint32_t Ninit;
	uint32_t Np;
	uint32_t Ns;
	double_t f;
	uint32_t CountPoints;
	curve Point;
	std::vector<double_t> Distance;
	double_t AverageDistance;
	double_t Radius;
	curve AdductionPoints;
	uint32_t AdductionCount;
	double_t Radius;
public:
	GeneralizationCurve();
	GeneralizationCurve::GeneralizationCurve(double_t C_, uint32_t Np_,
		uint32_t Ns_, double_t f_, uint32_t Ninit_);
	void BuildCurve(uint32_t _countOfPoints);
	double_t ComputeDistances();
	double_t ComputeAvarageDistance();
	double_t ComputeRadius(double_t averageDistance);
	bool BelongPoints(point point1, point point2, point current);
	point* CheckInterSection(point point1, point point2,
		double_t radius, point pointCircle);
	void Adduction();
	virtual ~GeneralizationCurve();
};

