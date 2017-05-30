#pragma once

#include <string>
#include <list>
#include <vector>
#include <utility>
#include <map>
#include <functional>
#include <atomic>
#include <algorithm>

#include "common.h"

#include "mkl.h"

#define X(point) ((point).first)
#define Y(point) ((point).second)

typedef enum MathType {
	StdMath		= 0,
	IntelMklMath	= 1,
	MaxMath		= 2,
} MathType;

class GeneralizationCurve
{
private:
	bool parallelism_enabled;
	double_t C;
	uint32_t Ninit;
	uint32_t Np;
	uint32_t Ns;
	double_t f;
	double_t M;

	MathType math_type = StdMath;

	std::function<double(void)> ComputeDistances[MaxMath];
	std::function<point*(point point1, point point2,
		double_t radius, point pointCircle)> CheckInterSection[2];
	std::function<void(void)> SegmentationInner[MaxMath];
	std::function<uint32_t(uint32_t CurrentSegment, double_t dist)> ComputeQuadrics[MaxMath];
	std::function<double_t(point *A, point *B)> ComputeDistBetweenPoints[MaxMath];
	std::function<void(void)> SimplificationInner[MaxMath];
	std::function<curve*(curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment,
		uint32_t i, uint32_t &CountOfPoints)> SimplificationOfSegment[MaxMath];

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
	std::vector<double_t> Integral—haract;

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

	double InitialSinuosityCoef;
	double ResultSinuosityCoef;

	std::vector<std::map<uint32_t, bool>> GridMath;
	MKL_LONG dimension[2] = { 10000, 10000 };
	bool *GridMkl;

	curve* CurveDup(curve *fromCurve);
	
	double_t ComputeDistancesMath(void);
	double_t ComputeDistancesMkl(void);

	double_t ComputeAvarageDistance();
	double_t ComputeRadius(double_t averageDistance);
	bool BelongPoints(point point1, point point2, point current);
	
	
	point* CheckInterSectionMath(point point1, point point2,
		double_t radius, point pointCircle);
	point* CheckInterSectionMkl(point point1, point point2,
		double_t radius, point pointCircle);

	void SegmentationMath();
	void SegmentationMkl();

	double_t func1(point *p1, point *p2, point *pnt);
	bool IntersectionLineAndSquare(point *p1, point *p2, std::vector<point> *points);

	uint32_t ComputeQuadricsMath(uint32_t CurrentSegment, double_t dist);
	uint32_t ComputeQuadricsMkl(uint32_t CurrentSegment, double_t dist);

	void CopyArraysOfPoints(point *FromArray, point *ToArray, uint32_t Length,
		uint32_t StartIndexFrom, uint32_t StartIndexTo);

	double_t ComputeDistBetweenPointsMath(point *A, point *B);
	double_t ComputeDistBetweenPointsMkl(point *A, point *B);

	double_t ComputeP(double_t DistAB, double_t DistBC, double_t DistAC);
	double_t ComputeS(double_t p, double_t DistAB, double_t DistBC, double_t DistAC);
	double_t ComputeDistBetweenPointAndLine(point *A, point *B, point *C);
	curve* SimplificationOfCurve(curve *initialPoints, uint32_t len,
		double_t H, uint32_t &CountOfNewPoints);

	curve* SimplificationOfSegmentMath(curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints);
	curve* SimplificationOfSegmentMkl(curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints);

	void SimplificationMath(void);
	void SimplificationMkl(void);

	void InitializeClassMembers(void);


	/* Utility */
	void UtilInitializeClassMembers();
		/* Distances */
	double_t UtilComputeDistancesMath(curve* Obj, uint32_t countOfPoints,
					  std::vector<double> *distances);
	double_t UtilComputeDistancesMkl(curve* Obj, uint32_t countOfPoints,
					 std::vector<double> *distances);
	std::function<double_t(curve* Obj, uint32_t countOfPoints,
		std::vector<double> *distances)> UtilComputeDistances[MaxMath];
		/* ~Distances */
		/* Avg Distances */
	double_t UtilComputeAvarageDistance(curve* Obj,
					    uint32_t countOfPoints,
					    std::vector<double> *distances);
		/* ~Avg Distances */
		/* Radius */
	double_t UtilComputeRadius(double coeff,
				   curve* Obj,
				   uint32_t countOfPoints,
				   std::vector<double> *distances);
		/* ~Radius */
		/* Intersection - (It utilizes already implemented algorithms,
		 * because they're suitable enough) */
	std::function<point*(point point1, point point2,
		double_t radius, point pointCircle)> UtilCheckInterSection[MaxMath];
		/* ~Intersection */

	/* ~Utility */
public:
	GeneralizationCurve();
	GeneralizationCurve(double_t C_ = 0.5, uint32_t Np_ = 500,
		uint32_t Ns_ = 50, double_t f_ = 5, uint32_t Ninit_ = 1000,
		int parallelism = 0);

	/* Setters */
	void SetUseMkl(bool use)
	{
		math_type = use ? IntelMklMath : StdMath;
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
	void SetParamM(double M_)
	{
		M = M_;
	}
	/* ~Setters */

	/* Getters */
	curve* GetSouceCurve();
	curve* GetAdductedCurve(uint32_t &count);
	std::vector<curve> *GetSegmentedCurve(uint32_t &countOfSegm,
					      uint32_t &countOfInitSegm,
					      std::vector<uint32_t> **countOfPointInSegm);
	curve**GetSimplifiedCurve(uint32_t &countOfSimplSegm, uint32_t &totalCountOfPoints,
				  std::vector<uint32_t> **countOfPointInSimplSegm);
	curve** GetSmoothedCurve(uint32_t &countOfSmoothSegm, uint32_t &countOfSmoothPoints,
				 std::vector<uint32_t> **countOfPointInSmoothSegm);
	/* ~Getters */

	void BuildCurve(uint32_t countOfPoints, curve *newCurve);
	void SetValueOfScale(double_t m);
	void SetParallelismMode(bool mode);
	void Adduction();
	void Segmentation();
	void Simplification();
	void Smoothing();

	curve* GetResultCurve()
	{
		curve *resultCurve = new curve;
		uint32_t iter = 0;
		X(*resultCurve).resize(TotalCountOfPointsAfterSmoothing);
		Y(*resultCurve).resize(TotalCountOfPointsAfterSmoothing);
		
		for (uint32_t i = 0; i < ResultSegmentCount; i++)
		{
			for (uint32_t j = 0; j < CountOfPointsAfterSmoothing[i]; j++)
			{
				X(*resultCurve)[iter] = X(*PointsAfterSmoothing[i])[j];
				Y(*resultCurve)[iter] = Y(*PointsAfterSmoothing[i])[j];
				iter++;
			}
		}
		return resultCurve;
	}

	double_t ComputeSinuosityCoeff(curve *Obj,
				       uint32_t countOfPoints,
				       uint32_t numOfSegm);

	virtual ~GeneralizationCurve();
};

