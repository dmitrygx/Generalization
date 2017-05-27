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
	bool use_mkl;
	bool parallelism_enabled;
	double_t C;
	uint32_t Ninit;
	uint32_t Np;
	uint32_t Ns;
	double_t f;
	double_t M;
	uint32_t CountPoints;
	curve *Points;
	std::vector<double_t> Distance;
	double_t AverageDistance;
	double_t Radius;
	curve *AdductionPoints; // TODO
	uint32_t AdductionCount;

	uint32_t CountOfSegments;
	std::vector<uint32_t> SegmentCountPoints;

	std::vector<uint32_t> LocalMax;
	std::vector<double_t> Integral�haract;

	std::vector<curve> AdductionPointsInSegment; // TODO
	std::vector<uint32_t> CountOfAdductionPointsInSegment;
	uint32_t ResultSegmentCount;

	std::vector<double_t> AngularCoeffRegresLine;


	curve **PointsAfterSimplification;
	std::vector<uint32_t> CountOfPointsAfterSimplification;
	uint32_t TotalCountOfPointsAfterSimplification;

	curve **PointsAfterSmoothing;
	std::vector<uint32_t> CountOfPointsAfterSmoothing;
	uint32_t TotalCountOfPointsAfterSmoothing;

	curve* CurveDup(curve *fromCurve);
	double_t ComputeDistances();
	double_t ComputeAvarageDistance();
	double_t ComputeRadius(double_t averageDistance);
	bool BelongPoints(point point1, point point2, point current);
	point* CheckInterSection(point point1, point point2,
		double_t radius, point pointCircle);
	double_t func1(point *p1, point *p2, point *pnt);
	bool IntersectionLineAndSquare(point *p1, point *p2, std::vector<point> *points);
	uint32_t OldComputeQuadrics(uint32_t CurrentSegment, double_t dist);
	uint32_t ComputeQuadrics(uint32_t CurrentSegment, double_t dist);
	void CopyArraysOfPoints(point *FromArray, point *ToArray, uint32_t Length,
		uint32_t StartIndexFrom, uint32_t StartIndexTo);
	double_t ComputeDistBetweenPoints(point *A, point *B);
	double_t ComputeP(double_t DistAB, double_t DistBC, double_t DistAC);
	double_t ComputeS(double_t p, double_t DistAB, double_t DistBC, double_t DistAC);
	double_t ComputeDistBetweenPointAndLine(point *A, point *B, point *C);
	curve* SimplificationOfCurve(curve *initialPoints, uint32_t len,
		double_t H, uint32_t &CountOfNewPoints);
	curve* SimplificationOfSegment(curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints);
public:
	GeneralizationCurve();
	GeneralizationCurve(double_t C_ = 0.5, uint32_t Np_ = 500,
		uint32_t Ns_ = 50, double_t f_ = 5, uint32_t Ninit_ = 1000,
		int parallelism = 0);

	/* Setters */
	void SetUseMkl(bool use)
	{
		use_mkl = use;
	}
	void SetUseOpenMP(bool use)
	{
		parallelism_enabled = use;
	}
	void SetParamC(double C_)
	{
		C = C_;
	}
	void SetParamNp(uint32_t Np_)
	{
		Np = Np_;
	}
	void SetParamNinit(uint32_t Ninit_)
	{
		Ninit = Ninit_;
	}
	void SetParamf(double f_)
	{
		f = f_;
	}
	void SetParamNs(uint32_t Ns_)
	{
		Ns = Ns_;
	}
	/* ~Setters */

	/* Getters */
	curve* GetSouceCurve();
	curve* GetAdductedCurve(uint32_t &count);
	std::vector<curve> *GetSegmentedCurve(uint32_t &countOfSegm,
					      std::vector<uint32_t> **countOfPointInSegm);
	curve**GetSimplifiedCurve(uint32_t &countOfSimplSegm,
				  std::vector<uint32_t> **countOfPointInSimplSegm);
	curve** GetSmoothedCurve(uint32_t &countOfSmoothSegm,
				 std::vector<uint32_t> **countOfPointInSmoothSegm);
	/* ~Getters */

	void BuildCurve(uint32_t countOfPoints, curve *newCurve);
	void SetValueOfScale(double_t m);
	void SetParallelismMode(bool mode);
	void Adduction();
	void Segmentation();
	void OldSimplification();
	void Simplification();
	void Smoothing();
	virtual ~GeneralizationCurve();
};

