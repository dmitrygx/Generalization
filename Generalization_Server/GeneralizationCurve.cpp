#include "GeneralizationCurve.h"
#include <cassert>
#include <iostream>
#ifdef _OPENMP
# include <omp.h>
#endif

#define MKL_INT size_t
#include "mkl.h"

#define _USE_MATH_DEFINES
#include <math.h>

void GeneralizationCurve::InitializeClassMembers(void)
{
	ComputeDistances[StdMath] = [this](){
		return this->ComputeDistancesMath(); 
	};
	ComputeDistances[IntelMklMath] = [this]() {
		return this->ComputeDistancesMkl();
	};

	CheckInterSection[StdMath] = [this](point point1, point point2,
		double_t radius, point pointCircle) {
		return this->CheckInterSectionMath(point1, point2,
						   radius, pointCircle);
	};
	CheckInterSection[IntelMklMath] = [this](point point1, point point2,
		double_t radius, point pointCircle) {
		return this->CheckInterSectionMkl(point1, point2,
						  radius, pointCircle);
	};

	SegmentationInner[StdMath] = [this]() {
		this->SegmentationMath();
	};
	SegmentationInner[IntelMklMath] = [this]() {
		this->SegmentationMkl();
	};

	ComputeQuadrics[StdMath] = [this](uint32_t CurrentSegment, double_t dist) {
		return this->ComputeQuadricsMath(CurrentSegment, dist);
	};
	ComputeQuadrics[IntelMklMath] = [this](uint32_t CurrentSegment, double_t dist) {
		return this->ComputeQuadricsMkl(CurrentSegment, dist);
	};

	ComputeDistBetweenPoints[StdMath] = [this](point *A, point *B) {
		return this->ComputeDistBetweenPointsMath(A, B);
	};
	ComputeDistBetweenPoints[IntelMklMath] = [this](point *A, point *B) {
		return this->ComputeDistBetweenPointsMkl(A, B);
	};

	SimplificationInner[StdMath] = [this]() {
		this->SimplificationMath();
	};
	SimplificationInner[IntelMklMath] = [this]() {
		this->SimplificationMkl();
	};

	SimplificationOfSegment[StdMath] = [this](curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment,
		uint32_t i, uint32_t &CountOfPoint) {
		return SimplificationOfSegmentMath(AdductionPointsInSegment,
			CountOfAdductionPointsInSegment, i, CountOfPoint);
	};
	SimplificationOfSegment[IntelMklMath] = [this](curve* AdductionPointsInSegment,
		uint32_t *CountOfAdductionPointsInSegment,
		uint32_t i, uint32_t &CountOfPoint) {
		return SimplificationOfSegmentMkl(AdductionPointsInSegment,
			CountOfAdductionPointsInSegment, i, CountOfPoint);
	};
}

void GeneralizationCurve::UtilInitializeClassMembers()
{
	UtilComputeDistances[StdMath] = [this](curve* Obj,
		uint32_t countOfPoints,
		std::vector<double> *distances) {
		return this->UtilComputeDistancesMath(Obj, countOfPoints, distances);
	};
	UtilComputeDistances[IntelMklMath] = [this](curve* Obj,
		uint32_t countOfPoints,
		std::vector<double> *distances) {
		return this->UtilComputeDistancesMkl(Obj, countOfPoints, distances);
	};
}

GeneralizationCurve::GeneralizationCurve()
{
	C = 0.5;
	Np = 500;
	Ns = 50;
	f = 5;
	Ninit = 1000;
	M = 1;
	parallelism_enabled = 0;

	InitializeClassMembers();
	UtilInitializeClassMembers();
}

GeneralizationCurve::GeneralizationCurve(double_t C_, uint32_t Np_,
	uint32_t Ns_, double_t f_, uint32_t Ninit_, int parallelism)
{
	C = C_;
	Np = Np_;
	Ns = Ns_;
	f = f_;
	Ninit = Ninit_;
	M = 1;
	parallelism_enabled = parallelism;

	InitializeClassMembers();
	UtilInitializeClassMembers();
}

curve* GeneralizationCurve::CurveDup(curve *fromCurve)
{
	assert(fromCurve);
	assert(X(*fromCurve).size() == Y(*fromCurve).size());

	// since count of X is equal to coutn of Y, it doesn't matter whose size is choosen
	size_t lengthCurve = X(*fromCurve).size();

	curve *resCurve = new curve;
	X(*resCurve).resize(lengthCurve);
	Y(*resCurve).resize(lengthCurve);

	for (uint32_t i = 0; i < lengthCurve; i++)
	{
		X(*resCurve)[i] = X(*fromCurve)[i];
		Y(*resCurve)[i] = Y(*fromCurve)[i];
	}

	return resCurve;
}
/* Becnhmark-specific helpers */
void GeneralizationCurve::SetAdductionPointsPerSegment(curve* adduction_points,
	uint32_t pps, uint32_t seg_count)
{
	uint32_t iter = 0;

	AdductionPointsInSegment.resize(seg_count);
	CountOfAdductionPointsInSegment.resize(seg_count);
	ResultSegmentCount = seg_count;

	for (uint32_t i = 0; i < ResultSegmentCount; i++)
	{
		X(AdductionPointsInSegment[i]).resize(pps);
		Y(AdductionPointsInSegment[i]).resize(pps);
		CountOfAdductionPointsInSegment[i] = pps;
		for (uint32_t j = 0; j < pps; j++)
		{
			X(AdductionPointsInSegment[i])[j] = X(*adduction_points)[iter];
			Y(AdductionPointsInSegment[i])[j] = Y(*adduction_points)[iter];
			iter++;
		}
	}
}
/* ~Becnhmark-specific helpers */
/* Getters */
curve* GeneralizationCurve::GetSouceCurve()
{
	return Points;
}

curve* GeneralizationCurve::GetAdductedCurve(uint32_t &count)
{
	count = AdductionCount;
	return AdductionPoints;
}

std::vector<curve> *GeneralizationCurve::GetSegmentedCurve(uint32_t &countOfSegm,
							   uint32_t &countOfInitSegm,
							   std::vector<uint32_t> **countOfPointInSegm)
{
	countOfSegm = ResultSegmentCount;
	countOfInitSegm = CountOfSegments;
	*countOfPointInSegm = &CountOfAdductionPointsInSegment;

	return &AdductionPointsInSegment;
}

curve** GeneralizationCurve::GetSimplifiedCurve(uint32_t &countOfSimplSegm, uint32_t &totalCountOfPoints,
						std::vector<uint32_t> **countOfPointInSimplSegm)
{
	countOfSimplSegm = ResultSegmentCount;
	totalCountOfPoints = TotalCountOfPointsAfterSimplification;
	*countOfPointInSimplSegm = &CountOfPointsAfterSimplification;

	return PointsAfterSimplification;
}

curve** GeneralizationCurve::GetSmoothedCurve(uint32_t &countOfSmoothSegm, uint32_t &countOfSmoothPoints,
					      std::vector<uint32_t> **countOfPointInSmoothSegm)
{
	countOfSmoothSegm = ResultSegmentCount;
	countOfSmoothPoints = TotalCountOfPointsAfterSmoothing;
	*countOfPointInSmoothSegm = &CountOfPointsAfterSmoothing;

	return PointsAfterSmoothing;
}
/* ~Getters */

void GeneralizationCurve::BuildCurve(uint32_t countOfPoints, curve *newCurve)
{
	CountPoints = countOfPoints;

	Points = CurveDup(newCurve);

	X(*Points).resize(CountPoints);
	Y(*Points).resize(CountPoints);

	Distance.resize(CountPoints - 1);

	if (math_type == StdMath)
		GridMath.resize(10000);
	else
		GridMkl = (bool *)mkl_calloc(1, dimension[0] * dimension[1] * sizeof(*GridMkl),
					     64);

	AverageDistance = 0;
	Radius = 0;
}

void GeneralizationCurve::InitForBenchmarks()
{
	if (math_type == StdMath)
		GridMath.resize(10000);
	else
		GridMkl = (bool *)mkl_calloc(1, dimension[0] * dimension[1] * sizeof(*GridMkl),
			64);
}

double_t GeneralizationCurve::ComputeDistancesMath(void)
{
	double_t sum = 0;
	for (uint32_t i = 0; i < CountPoints - 1; i++)
	{
		Distance[i] = sqrt(((X(*Points)[i+1] - X(*Points)[i]) * (X(*Points)[i + 1] - X(*Points)[i])) +
			(Y(*Points)[i + 1] - Y(*Points)[i]) * (Y(*Points)[i + 1] - Y(*Points)[i]));

		sum += Distance[i];
	}
	return sum;
}

double_t GeneralizationCurve::ComputeDistancesMkl(void)
{
	double_t sum = 0;
	for (uint32_t i = 0; i < CountPoints - 1; i++)
	{
		double_t PointSubX = (X(*Points)[i + 1] - X(*Points)[i]);
		double_t PointSubY = (Y(*Points)[i + 1] - Y(*Points)[i]);
		double_t PointX2 = PointSubX*PointSubX, PointY2 = PointSubY*PointSubY, Point2Sum;
		Point2Sum = PointX2 + PointY2;
		vmdSqrt(1, &Point2Sum, &Distance[i], 0);

		sum += Distance[i];
	}
	return sum;
}

double_t GeneralizationCurve::UtilComputeDistancesMath(curve* Obj, uint32_t countOfPoints,
						       std::vector<double> *distances)
{
	double_t sum = 0;
	for (uint32_t i = 0; i < countOfPoints - 1; i++)
	{
		(*distances)[i] = sqrt(((X(*Obj)[i + 1] - X(*Obj)[i]) * (X(*Obj)[i + 1] - X(*Obj)[i])) +
			((Y(*Obj)[i + 1] - Y(*Obj)[i]) * (Y(*Obj)[i + 1] - Y(*Obj)[i])));

		sum += (*distances)[i];
	}
	return sum;
}

double_t GeneralizationCurve::UtilComputeDistancesMkl(curve* Obj, uint32_t countOfPoints,
						      std::vector<double> *distances)
{
	double_t sum = 0;
	for (uint32_t i = 0; i < countOfPoints - 1; i++)
	{
		double_t PointSubX = (X(*Obj)[i + 1] - X(*Obj)[i]);
		double_t PointSubY = (Y(*Obj)[i + 1] - Y(*Obj)[i]);
		double_t PointX2 = PointSubX*PointSubX, PointY2 = PointSubY*PointSubY, Point2Sum;
		Point2Sum = PointX2 + PointY2;
		vmdSqrt(1, &Point2Sum, &(*distances)[i], 0);

		sum += (*distances)[i];
	}
	return sum;
}

double_t GeneralizationCurve::ComputeAvarageDistance()
{
	double_t sum = ComputeDistances[math_type]();

	return sum / (CountPoints - 1);
}

double_t GeneralizationCurve::UtilComputeAvarageDistance(curve* Obj,
							 uint32_t countOfPoints,
							 std::vector<double> *distances)
{
	double_t sum = UtilComputeDistances[math_type](Obj, countOfPoints, distances);

	return sum / (countOfPoints - 1);
}

double_t GeneralizationCurve::ComputeRadius(double_t averageDistance)
{
	return C * ComputeAvarageDistance();
}

double_t GeneralizationCurve::UtilComputeRadius(double coeff,
						curve* Obj,
						uint32_t countOfPoints,
						std::vector<double> *distances)
{
	return coeff * UtilComputeAvarageDistance(Obj, countOfPoints, distances);
}

bool GeneralizationCurve::BelongPoints(point point1, point point2, point current)
{
	bool belong = false;
	// 1st Quadrunt
	if (((X(current) >= X(point1)) && (X(current) <= X(point2))) &&
		((Y(current) >= Y(point1)) && (Y(current) <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("1");
		belong = true;
	}
	// 4th Quadrunt
	if (((X(current) >= X(point1)) && (X(current) <= X(point2))) &&
		((Y(current) <= Y(point1)) && (Y(current) >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("4");
		belong = true;
	}
	//3rd Quadrunt
	if (((X(current) <= X(point1)) && (X(current) >= X(point2))) &&
		((Y(current) <= Y(point1)) && (Y(current) >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("3");
		belong = true;
	}
	//2st Quadrunt 
	if (((X(current) <= X(point1)) && (X(current) >= X(point2))) &&
		((Y(current) >= Y(point1)) && (Y(current) <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("2");
		belong = true;
	}

	return belong;
}

point* GeneralizationCurve::CheckInterSectionMath(point point1, point point2,
	double_t radius, point pointCircle)
{
	double_t x1 = X(point1);
	double_t x2 = X(point2);
	double_t y1 = Y(point1);
	double_t y2 = Y(point2);
	double_t k = (x2 != x1) ? (-((y1 - y2) / (x2 - x1))) : 0;
	double_t b = (x2 != x1) ? (-((x1 * y2 - x2 * y1) / (x2 - x1))) : 0;
	double_t D = (((2 * k * b - 2 * X(pointCircle) - 2 * Y(pointCircle) * k) *
		(2 * k * b - 2 * X(pointCircle) - 2 * Y(pointCircle) * k)) -
		(4 + 4 * k * k) * (b * b - radius * radius + 
		X(pointCircle) * X(pointCircle) +
		Y(pointCircle) * Y(pointCircle) -
		2 * Y(pointCircle) * b));
	if (D < 0)
	{
		//System.Windows.Forms.MessageBox.Show("D (" + D.ToString() + ") < 0");
		return nullptr;
	}
	
	double_t X1 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) - sqrt(D)) / (2 + 2 * k * k));
	double_t X2 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) + sqrt(D)) / (2 + 2 * k * k));
	double_t Y1 = k * X1 + b;
	double_t Y2 = k * X2 + b;

	//if (X1 == X2)
	//{
	//	//System.Windows.Forms.MessageBox.Show(" Одна точка пересечения " + X1.ToString() + " " + Y1.ToString());
	//}
	//else
	//{
	//	//System.Windows.Forms.MessageBox.Show(" Две точки пересечения " + X1.ToString() + " " + Y1.ToString() + " и " + X2.ToString() + " " + Y2.ToString());
	//}

	// First root
	bool first = false;
	// 1st Quadrunt
	if (((X1 >= X(point1)) && (X1 <= X(point2))) &&
		((Y1 >= Y(point1)) && (Y1 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("1");
		first = true;
	}
	// 4th Quadrunt
	if (((X1 >= X(point1)) && (X1 <= X(point2))) &&
		((Y1 <= Y(point1)) && (Y1 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("4");
		first = true;
	}
	//3rd Quadrunt
	if (((X1 <= X(point1)) && (X1 >= X(point2))) &&
		((Y1 <= Y(point1)) && (Y1 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("3");
		first = true;
	}
	//2st Quadrunt 
	if (((X1 <= X(point1)) && (X1 >= X(point2))) &&
		((Y1 >= Y(point1)) && (Y1 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("2");
		first = true;
	}

	// Second root
	bool second = false;
	// 1st Quadrunt
	if (((X2 >= X(point1)) && (X2 <= X(point2))) &&
		((Y2 >= Y(point1)) && (Y2 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("1");
		second = true;
	}
	// 4th Quadrunt
	if (((X2 >= X(point1)) && (X2 <= X(point2))) &&
		((Y2 <= Y(point1)) && (Y2 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("4");
		second = true;
	}
	//3rd Quadrunt
	if (((X2 <= X(point1)) && (X2 >= X(point2))) &&
		((Y2 <= Y(point1)) && (Y2 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("3");
		second = true;
	}
	//2st Quadrunt 
	if (((X2 <= X(point1)) && (X2 >= X(point2))) &&
		((Y2 >= Y(point1)) && (Y2 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("2");
		second = true;
	}

	point *res = nullptr;

	if (first)
	{
		res = new point;

		X(*res) = X1;
		Y(*res) = Y1;
	}
	if (second)
	{
		delete res;
		res = new point;

		X(*res) = X2;
		Y(*res) = Y2;
	}
	
	return res;
}

point* GeneralizationCurve::CheckInterSectionMkl(point point1, point point2,
	double_t radius, point pointCircle)
{
	double_t x1 = X(point1);
	double_t x2 = X(point2);
	double_t y1 = Y(point1);
	double_t y2 = Y(point2);
	double_t k = (x2 != x1) ? (-((y1 - y2) / (x2 - x1))) : 0;
	double_t b = (x2 != x1) ? (-((x1 * y2 - x2 * y1) / (x2 - x1))) : 0;
	double_t DArg = (2 * k * b - 2 * X(pointCircle) - 2 * Y(pointCircle) * k);
	double_t DArg2 = DArg * DArg;
	double_t D = (DArg2 -
		(4 + 4 * k * k) * (b * b - radius * radius +
			X(pointCircle) * X(pointCircle) +
			Y(pointCircle) * Y(pointCircle) -
			2 * Y(pointCircle) * b));
	if (D < 0)
	{
		//System.Windows.Forms.MessageBox.Show("D (" + D.ToString() + ") < 0");
		return nullptr;
	}

	double_t Arg;
	vmdSqrt(1, &D, &Arg, 0);
	double_t X1 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) - Arg) / (2 + 2 * k * k));
	double_t X2 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) + Arg) / (2 + 2 * k * k));
	double_t Y1 = k * X1 + b;
	double_t Y2 = k * X2 + b;

	// First root
	bool first = false;
	// 1st Quadrunt
	if (((X1 >= X(point1)) && (X1 <= X(point2))) &&
		((Y1 >= Y(point1)) && (Y1 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("1");
		first = true;
	}
	// 4th Quadrunt
	if (((X1 >= X(point1)) && (X1 <= X(point2))) &&
		((Y1 <= Y(point1)) && (Y1 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("4");
		first = true;
	}
	//3rd Quadrunt
	if (((X1 <= X(point1)) && (X1 >= X(point2))) &&
		((Y1 <= Y(point1)) && (Y1 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("3");
		first = true;
	}
	//2st Quadrunt 
	if (((X1 <= X(point1)) && (X1 >= X(point2))) &&
		((Y1 >= Y(point1)) && (Y1 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("2");
		first = true;
	}

	// Second root
	bool second = false;
	// 1st Quadrunt
	if (((X2 >= X(point1)) && (X2 <= X(point2))) &&
		((Y2 >= Y(point1)) && (Y2 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("1");
		second = true;
	}
	// 4th Quadrunt
	if (((X2 >= X(point1)) && (X2 <= X(point2))) &&
		((Y2 <= Y(point1)) && (Y2 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("4");
		second = true;
	}
	//3rd Quadrunt
	if (((X2 <= X(point1)) && (X2 >= X(point2))) &&
		((Y2 <= Y(point1)) && (Y2 >= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("3");
		second = true;
	}
	//2st Quadrunt 
	if (((X2 <= X(point1)) && (X2 >= X(point2))) &&
		((Y2 >= Y(point1)) && (Y2 <= Y(point2))))
	{
		//System.Windows.Forms.MessageBox.Show("2");
		second = true;
	}

	point *res = nullptr;

	if (first)
	{
		res = new point;

		X(*res) = X1;
		Y(*res) = Y1;
	}
	if (second)
	{
		delete res;
		res = new point;

		X(*res) = X2;
		Y(*res) = Y2;
	}

	return res;
}

void GeneralizationCurve::Adduction()
{
	Radius = ComputeRadius(ComputeAvarageDistance());

	curve AdductionPointsTmp;
	AdductionPointsTmp.first.resize(CountPoints);
	AdductionPointsTmp.second.resize(CountPoints);

	AdductionPoints = new curve;

	X(AdductionPointsTmp)[0] = X(*Points)[0];
	Y(AdductionPointsTmp)[0] = Y(*Points)[0];

	// X(*AdductionPoints)[0] = X(AdductionPointsTmp)[0];
	X(*AdductionPoints).push_back(X(AdductionPointsTmp)[0]);
	// Y(*AdductionPoints)[0] = Y(AdductionPointsTmp)[0];
	Y(*AdductionPoints).push_back(Y(AdductionPointsTmp)[0]);

	uint32_t count = 1;
	uint32_t k = 1;
	uint32_t i = 0;

	while (true)
	{
		point *res;

		point adductionPoint;
		X(adductionPoint) = X(*AdductionPoints)[count - 1];
		Y(adductionPoint) = Y(*AdductionPoints)[count - 1];

		point pnt;
		X(pnt) = X(*Points)[i + k];
		Y(pnt) = Y(*Points)[i + k];

		res = CheckInterSection[math_type](adductionPoint, pnt, Radius, adductionPoint);
		if (nullptr == res)
		{
			k++;
		}
		else
		{
			X(AdductionPointsTmp)[i] = X(*res);
			Y(AdductionPointsTmp)[i] = Y(*res);

			delete res;

			// X(*AdductionPoints)[count] = X(AdductionPointsTmp)[i];
			X(*AdductionPoints).push_back(X(AdductionPointsTmp)[i]);
			// Y(*AdductionPoints)[count] = Y(AdductionPointsTmp)[i];
			Y(*AdductionPoints).push_back(Y(AdductionPointsTmp)[i]);

			count++;
			i = i + k - 1;
			k = 1;
		}

		if (i + k > CountPoints - 1)
		{
			// X(*AdductionPoints)[count] = X(*Points)[CountPoints - 1];
			X(*AdductionPoints).push_back(X(*Points)[CountPoints - 1]);
			// Y(*AdductionPoints)[count] = Y(*Points)[CountPoints - 1];
			Y(*AdductionPoints).push_back(Y(*Points)[CountPoints - 1]);

			count++;
			break;
		}
	}

	AdductionCount = count;
}

void GeneralizationCurve::SegmentationMath()
{
	std::vector<double_t> Dist;
	Dist.resize(AdductionCount - 1);

	for (uint32_t i = 0; i < AdductionCount - 1; i++)
	{
		Dist[i] = ((X(*AdductionPoints)[i + 1] - X(*AdductionPoints)[i]) *
			(X(*AdductionPoints)[i + 1] - X(*AdductionPoints)[i])) +
			((Y(*AdductionPoints)[i + 1] - Y(*AdductionPoints)[i]) *
			(Y(*AdductionPoints)[i + 1] - Y(*AdductionPoints)[i]));
	}
	CountOfSegments = AdductionCount / Ninit;
	bool flag = false;
	if ((AdductionCount % Ninit) != 0)
	{
		flag = true;
		CountOfSegments++;
	}

	curve *InitSegments = new curve[CountOfSegments];
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		X(InitSegments[i]).resize(Ninit);
		Y(InitSegments[i]).resize(Ninit);
	}

	SegmentCountPoints.resize(CountOfSegments);

	uint32_t k = 0;
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		uint32_t N = Ninit;
		if ((flag) && (i == CountOfSegments - 1))
		{
			N = AdductionCount % Ninit;
		}
		for (uint32_t j = 0; j < N; j++)
		{
			X(InitSegments[i])[j] = X(*AdductionPoints)[k];
			Y(InitSegments[i])[j] = Y(*AdductionPoints)[k];
			k++;
		}
		SegmentCountPoints[i] = N;
	}
	k = 0;
	std::vector<std::vector<double_t>> AngleOfRotation;
	AngleOfRotation.resize(CountOfSegments);
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		AngleOfRotation[i].resize(Ninit);
	}

	std::vector<double_t> RotationOfSegment;
	RotationOfSegment.resize(CountOfSegments);
	std::vector<double_t> FullRotationOfSegment;
	FullRotationOfSegment.resize(CountOfSegments);

	IntegralСharact.resize(CountOfSegments);
	LocalMax.resize(CountOfSegments); /* Ei */
	std::fill_n(LocalMax.begin(), CountOfSegments, 0);

	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		uint32_t N = Ninit;
		if ((flag) && (i == CountOfSegments - 1))
		{
			N = AdductionCount % Ninit;
		}
		for (uint32_t j = 1; j < N - 1; j++)
		{
			double_t topLeft = (X(InitSegments[i])[j + 1] - X(InitSegments[i])[j]) *
				(X(InitSegments[i])[j] - X(InitSegments[i])[j - 1]);
			double_t topRight = (Y(InitSegments[i])[j + 1] - Y(InitSegments[i])[j]) *
				(Y(InitSegments[i])[j] - Y(InitSegments[i])[j - 1]);
			double_t botttom = (Dist[k] * Dist[k]);
			k++;
			double_t Value = (topLeft + topRight) / botttom;
			if (Value > 1)
			{
				Value = 1;
			}
			else if (Value < -1)
			{
				Value = -1;
			}
			AngleOfRotation[i][j] = acos(Value);
			RotationOfSegment[i] += AngleOfRotation[i][j];
			if ((AngleOfRotation[i][j] > AngleOfRotation[i][j - 1]) &&
				(AngleOfRotation[i][j] > AngleOfRotation[i][j + 1]))
			{
				LocalMax[i]++;
			}
		}
		FullRotationOfSegment[i] = RotationOfSegment[i] / (2 * M_PI); /* Wi */
		IntegralСharact[i] = FullRotationOfSegment[i] + f * LocalMax[i]; /* Mi */
	}

	std::vector<uint32_t> MinSegmentCountPoint;
	MinSegmentCountPoint.resize(CountOfSegments);
	uint32_t MinCountSegment = 0;
	bool ShouldUnionSegment = false;

	for (uint32_t i = 0; i < CountOfSegments; ++i)
	{
		if (SegmentCountPoints[i] < Np)
		{
			MinSegmentCountPoint[MinCountSegment++] = i;
		}
	}
	if ((CountOfSegments > Ns) && (MinCountSegment != 0))
	{
		ShouldUnionSegment = true;
	}

	AdductionPointsInSegment.resize(CountOfSegments);
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		X(AdductionPointsInSegment[i]).resize(AdductionCount);
		Y(AdductionPointsInSegment[i]).resize(AdductionCount);
	}

	CountOfAdductionPointsInSegment.resize(CountOfSegments);
	std::fill_n(CountOfAdductionPointsInSegment.begin(), CountOfSegments, 0);
	ResultSegmentCount = 0;
	while (ShouldUnionSegment)
	{
		for (uint32_t i = 0; i < CountOfSegments; i += 2)
		{
			if (i + 1 != CountOfSegments)
			{
				if ((IntegralСharact[i + 1] - IntegralСharact[i] >= -10) && (IntegralСharact[i + 1] - IntegralСharact[i] <= 10))
				{
					for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					for (uint32_t j = 0; j < SegmentCountPoints[i + 1]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i + 1])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i + 1])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
				}
				else
				{
					for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
					for (uint32_t j = 0; j < SegmentCountPoints[i + 1]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i + 1])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i + 1])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
				}
			}
		}
		break;
	}

	if (!ShouldUnionSegment)
	{
		for (uint32_t i = 0; i < CountOfSegments; ++i)
		{
			for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
			{
				X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
					X(InitSegments[i])[j];
				Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
					Y(InitSegments[i])[j];
				CountOfAdductionPointsInSegment[ResultSegmentCount]++;
			}
			ResultSegmentCount++;
		}
	}

	delete[] InitSegments;
}

void GeneralizationCurve::SegmentationMkl()
{
	std::vector<double_t> Dist;
	Dist.resize(AdductionCount - 1);

	for (uint32_t i = 0; i < AdductionCount - 1; i++)
	{
		double_t DistPointSub[2]{
			(X(*AdductionPoints)[i + 1] - X(*AdductionPoints)[i]),
			(Y(*AdductionPoints)[i + 1] - Y(*AdductionPoints)[i])
		};
		double_t DistPointSub2[2], DistArg;
		vmdPowx(2, DistPointSub, 2, DistPointSub2, 0);
		DistArg = DistPointSub2[0] + DistPointSub2[1];

		vmdSqrt(1, &DistArg, &Dist[i], 0);
	}
	CountOfSegments = AdductionCount / Ninit;
	bool flag = false;
	if ((AdductionCount % Ninit) != 0)
	{
		flag = true;
		CountOfSegments++;
	}

	curve *InitSegments = new curve[CountOfSegments];
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		X(InitSegments[i]).resize(Ninit);
		Y(InitSegments[i]).resize(Ninit);
	}

	SegmentCountPoints.resize(CountOfSegments);
	std::vector<std::vector<double_t>> AngleOfRotation;
	AngleOfRotation.resize(CountOfSegments);

	uint32_t k = 0, m = 0;

	std::vector<double_t> RotationOfSegment;
	RotationOfSegment.resize(CountOfSegments);
	std::vector<double_t> FullRotationOfSegment;
	FullRotationOfSegment.resize(CountOfSegments);

	IntegralСharact.resize(CountOfSegments);
	LocalMax.resize(CountOfSegments); /* Ei */
	std::fill_n(LocalMax.begin(), CountOfSegments, 0);

	uint32_t *N = (uint32_t *)mkl_calloc(CountOfSegments, sizeof(*N), 64);
	std::fill_n(N, CountOfSegments - 1, Ninit);
	N[CountOfSegments - 1] = (flag) ? AdductionCount % Ninit : N[CountOfSegments - 1];

//#pragma omp parallel for if ((parallelism_enabled) && (CountOfSegments >= 4))
	for (int32_t i = 0; i < CountOfSegments; i++)
	{
		uint32_t Npoints = Ninit;
		if ((flag) && (i == CountOfSegments - 1))
		{
			Npoints = AdductionCount % Ninit;
		}
		for (uint32_t j = 0; j < Npoints; j++)
		{
			X(InitSegments[i])[j] = X(*AdductionPoints)[m];
			Y(InitSegments[i])[j] = Y(*AdductionPoints)[m];
			m++;
		}
		SegmentCountPoints[i] = Npoints;
		AngleOfRotation[i].resize(Ninit);

		for (uint32_t j = 1; j < N[i] - 1; j++)
		{
			double_t topLeft = (X(InitSegments[i])[j + 1] - X(InitSegments[i])[j]) *
				(X(InitSegments[i])[j] - X(InitSegments[i])[j - 1]);
			double_t topRight = (Y(InitSegments[i])[j + 1] - Y(InitSegments[i])[j]) *
				(Y(InitSegments[i])[j] - Y(InitSegments[i])[j - 1]);
			double_t bottom = Dist[k] * Dist[k];
			k++;
			double_t Value = (topLeft + topRight) / bottom;
			if (Value > 1)
			{
				Value = 1;
			}
			else if (Value < -1)
			{
				Value = -1;
			}
			vmdAcos(1, &Value, &AngleOfRotation[i][j], 0);
			RotationOfSegment[i] += AngleOfRotation[i][j];

			if ((AngleOfRotation[i][j] > AngleOfRotation[i][j - 1]) &&
			    (AngleOfRotation[i][j] > AngleOfRotation[i][j + 1]))
				LocalMax[i]++;
		}
		FullRotationOfSegment[i] = RotationOfSegment[i] / (2 * M_PI); /* Wi */
		IntegralСharact[i] = FullRotationOfSegment[i] + f * LocalMax[i]; /* Mi */
	}

	mkl_free(N);

	std::vector<uint32_t> MinSegmentCountPoint;
	MinSegmentCountPoint.resize(CountOfSegments);
	uint32_t MinCountSegment = 0;
	bool ShouldUnionSegment = false;

	for (uint32_t i = 0; i < CountOfSegments; ++i)
	{
		if (SegmentCountPoints[i] < Np)
		{
			MinSegmentCountPoint[MinCountSegment++] = i;
		}
	}
	if ((CountOfSegments > Ns) && (MinCountSegment != 0))
	{
		ShouldUnionSegment = true;
	}

	AdductionPointsInSegment.resize(CountOfSegments);
	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		X(AdductionPointsInSegment[i]).resize(AdductionCount);
		Y(AdductionPointsInSegment[i]).resize(AdductionCount);
	}

	CountOfAdductionPointsInSegment.resize(CountOfSegments);
	std::fill_n(CountOfAdductionPointsInSegment.begin(), CountOfSegments, 0);
	ResultSegmentCount = 0;

	while (ShouldUnionSegment)
	{
		for (uint32_t i = 0; i < CountOfSegments; i += 2)
		{
			if (i + 1 != CountOfSegments)
			{
				if ((IntegralСharact[i + 1] - IntegralСharact[i] >= -10) &&
				    (IntegralСharact[i + 1] - IntegralСharact[i] <= 10))
				{
					for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					for (uint32_t j = 0; j < SegmentCountPoints[i + 1]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i + 1])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i + 1])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
				}
				else
				{
					for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
					for (uint32_t j = 0; j < SegmentCountPoints[i + 1]; ++j)
					{
						X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							X(InitSegments[i + 1])[j];
						Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
							Y(InitSegments[i + 1])[j];
						CountOfAdductionPointsInSegment[ResultSegmentCount]++;
					}
					ResultSegmentCount++;
				}
			}
		}
		break;
	}

	if (!ShouldUnionSegment)
	{
		for (uint32_t i = 0; i < CountOfSegments; ++i)
		{
			for (uint32_t j = 0; j < SegmentCountPoints[i]; ++j)
			{
				X(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
					X(InitSegments[i])[j];
				Y(AdductionPointsInSegment[ResultSegmentCount])[CountOfAdductionPointsInSegment[ResultSegmentCount]] =
					Y(InitSegments[i])[j];
				CountOfAdductionPointsInSegment[ResultSegmentCount]++;
			}
			ResultSegmentCount++;
		}
	}

	delete[] InitSegments;
}

void GeneralizationCurve::Segmentation()
{
	return SegmentationInner[math_type]();
}

double_t GeneralizationCurve::func1(point *p1, point *p2, point *pnt)
{
	return ((Y(*p1) - Y(*p2)) * X(*pnt) + (X(*p1) - X(*p2)) * Y(*pnt) + (X(*p1) * Y(*p2) - X(*p2) * Y(*p1)));
}

bool GeneralizationCurve::IntersectionLineAndSquare(point *p1, point *p2, std::vector<point> *points)
{
	if (func1(p1, p2, &(*points)[0]) * func1(p1, p2, &(*points)[1]) >= 0)
	{
		return true;
	}
	else if (func1(p1, p2, &(*points)[1]) * func1(p1, p2, &(*points)[2]) >= 0)
	{
		return true;
	}
	else if (func1(p1, p2, &(*points)[2]) * func1(p1, p2, &(*points)[3]) >= 0)
	{
		return true;
	}
	else if (func1(p1, p2, &(*points)[3]) * func1(p1, p2, &(*points)[0]) >= 0)
	{
		return true;
	}
	return false;
}

#define Grid(i,j) GridMkl[(i)* dimension[1] + (j)]

uint32_t GeneralizationCurve::ComputeQuadricsMkl(uint32_t CurrentSegment, double_t dist)
{
	uint32_t CountOfSquare = 0;
	// std::vector<std::vector<bool>> Grid;
	for (uint32_t k = 0; k < CountOfAdductionPointsInSegment[CurrentSegment] - 1; ++k)
	{
		double_t A = Y(AdductionPointsInSegment[CurrentSegment])[k] - Y(AdductionPointsInSegment[CurrentSegment])[k + 1];
		double_t B = X(AdductionPointsInSegment[CurrentSegment])[k + 1] - X(AdductionPointsInSegment[CurrentSegment])[k];
		double_t C = X(AdductionPointsInSegment[CurrentSegment])[k] * Y(AdductionPointsInSegment[CurrentSegment])[k + 1] -
			X(AdductionPointsInSegment[CurrentSegment])[k + 1] * Y(AdductionPointsInSegment[CurrentSegment])[k];
		double_t DistArg = A * A + B * B;
		double_t Dist;
		vmdSqrt(1, &DistArg, &Dist, 0);
		Dist /= dist;
	
		uint32_t Left1 = (uint32_t)(X(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1;
		uint32_t Left2 = (uint32_t)(X(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1;
		uint32_t Right1 = (uint32_t)(Y(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1;
		uint32_t Right2 = (uint32_t)(Y(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1;

		for (uint32_t i = Left2; i < Left1; ++i)
		{
			for (uint32_t j = Right2; j < Right1; ++j)
			{
				std::vector<point> points;
				points.resize(4);
				X(points[0]) = i; Y(points[0]) = j;
				X(points[1]) = i + 1; Y(points[1]) = j;
				X(points[2]) = i + 1; Y(points[2]) = j + 1;
				X(points[3]) = i; Y(points[3]) = j + 1;
				point point_K, point_Kplus1;
				X(point_K) = X(AdductionPointsInSegment[CurrentSegment])[k];
				Y(point_K) = Y(AdductionPointsInSegment[CurrentSegment])[k];
#if 0
				polygon_t square;
				square.outer().push_back(point_t(i, j));
				square.outer().push_back(point_t(i + 1, j));
				square.outer().push_back(point_t(i + 1, j + 1));
				square.outer().push_back(point_t(i, j + 1));
				square.outer().push_back(point_t(i, j));
#endif
#if 0
				linestring_t line;
				line.push_back(point_t{
						X(AdductionPointsInSegment[CurrentSegment])[k],
						Y(AdductionPointsInSegment[CurrentSegment])[k]
				});
				line.push_back(point_t{
						X(AdductionPointsInSegment[CurrentSegment])[k + 1],
						Y(AdductionPointsInSegment[CurrentSegment])[k + 1]
				});
#endif

				X(point_Kplus1) = X(AdductionPointsInSegment[CurrentSegment])[k + 1];
				Y(point_Kplus1) = Y(AdductionPointsInSegment[CurrentSegment])[k + 1];

				if (IntersectionLineAndSquare(&point_K, &point_Kplus1, &points))
				{
#if 0
					multi_linestring_t output;
					bool res = boost::geometry::intersection(line, square, output);
#endif

					if (!Grid(i, j))
					{
						CountOfSquare++;
					}
					Grid(i, j) = true;
				}
			}
		}
	}
	memset(GridMkl, 0, dimension[0] * dimension[1] * sizeof(*GridMkl));

	return CountOfSquare;
}

uint32_t GeneralizationCurve::ComputeQuadricsMath(uint32_t CurrentSegment, double_t dist)
{
	uint32_t CountOfSquare = 0;
	// std::vector<std::vector<bool>> Grid;
	for (uint32_t k = 0; k < CountOfAdductionPointsInSegment[CurrentSegment] - 1; ++k)
	{
		double_t A = Y(AdductionPointsInSegment[CurrentSegment])[k] - Y(AdductionPointsInSegment[CurrentSegment])[k + 1];
		double_t B = X(AdductionPointsInSegment[CurrentSegment])[k + 1] - X(AdductionPointsInSegment[CurrentSegment])[k];
		double_t C = X(AdductionPointsInSegment[CurrentSegment])[k] * Y(AdductionPointsInSegment[CurrentSegment])[k + 1] -
			X(AdductionPointsInSegment[CurrentSegment])[k + 1] * Y(AdductionPointsInSegment[CurrentSegment])[k];
		double_t Dist = ((A * A) + (B * B)) / dist;
		/*std::cout << "----- Left1 = " << (X(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1 << " Left2 = " << (X(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1
			<< " Right1 = " << (Y(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1 << " Right2 = " << (Y(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1 << std::endl;*/
		uint32_t Left1 = (uint32_t)(X(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1;
		uint32_t Left2 = (uint32_t)(X(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1;
		uint32_t Right1 = (uint32_t)(Y(AdductionPointsInSegment[CurrentSegment])[k] + Dist) + 1;
		uint32_t Right2 = (uint32_t)(Y(AdductionPointsInSegment[CurrentSegment])[k] - Dist) - 1;

		/*std::cout << "Left1 = " << Left1 << " Left2 = " << Left2 << " Right1 = " << Right1 << " Right2 = " << Right2 << std::endl;*/
		for (uint32_t i = Left2; i < Left1; ++i)
		{
			for (uint32_t j = Right2; j < Right1; ++j)
			{
				std::vector<point> points;
				points.resize(4);
				X(points[0]) = i; Y(points[0]) = j;
				X(points[1]) = i + 1; Y(points[1]) = j;
				X(points[2]) = i + 1; Y(points[2]) = j + 1;
				X(points[3]) = i; Y(points[3]) = j + 1;
				point point_K, point_Kplus1;
				X(point_K) = X(AdductionPointsInSegment[CurrentSegment])[k];
				Y(point_K) = Y(AdductionPointsInSegment[CurrentSegment])[k];

				X(point_Kplus1) = X(AdductionPointsInSegment[CurrentSegment])[k + 1];
				Y(point_Kplus1) = Y(AdductionPointsInSegment[CurrentSegment])[k + 1];

				if (IntersectionLineAndSquare(&point_K, &point_Kplus1, &points))
				{
					if (!GridMath[i][j])
					{
						CountOfSquare++;
					}
					GridMath[i][j] = true;
				}
			}
		}
	}

	for (uint32_t i = 0; i < dimension[0]; i++)
		GridMath[i].clear();

	return CountOfSquare;
}

void GeneralizationCurve::CopyArraysOfPoints(point *FromArray, point *ToArray,
					     uint32_t Length, uint32_t StartIndexFrom,
					     uint32_t StartIndexTo)
{
	uint32_t k = StartIndexTo;

	for (uint32_t i = StartIndexFrom; i < Length; ++i)
	{
		ToArray[k++] = FromArray[i];
	}
}

double_t GeneralizationCurve::ComputeDistBetweenPointsMath(point *A, point *B)
{
	return sqrt(((X(*B) - X(*A)) * (X(*B) - X(*A))) +
		((Y(*B) - Y(*A)) * (Y(*B) - Y(*A))));
}

double_t GeneralizationCurve::ComputeDistBetweenPointsMkl(point *A, point *B)
{
	double arg {
		(X(*B) - X(*A)) * (X(*B) - X(*A)) +
		(Y(*B) - Y(*A)) * (Y(*B) - Y(*A))
	},  res;

	vmdSqrt(1, &arg, &res, 0);

	return res;
}

double_t GeneralizationCurve::ComputeP(double_t DistAB, double_t DistBC, double_t DistAC)
{
	return (0.5 * (DistAB + DistBC + DistAC));
}

double_t GeneralizationCurve::ComputeS(double_t p, double_t DistAB, double_t DistBC, double_t DistAC)
{
	return sqrt((uint64_t)p * ((uint64_t)p - (uint64_t)DistAB) * ((uint64_t)p - (uint64_t)DistBC) * ((uint64_t)p - (uint64_t)DistAC));
}

double_t GeneralizationCurve::ComputeDistBetweenPointAndLine(point *A, point *B, point *C)
{
	double_t DistAB = ComputeDistBetweenPoints[math_type](A, B);
	double_t DistBC = ComputeDistBetweenPoints[math_type](B, C);
	double_t DistAC = ComputeDistBetweenPoints[math_type](A, C);
	double_t p = ComputeP(DistAB, DistBC, DistAC);
	double_t S = ComputeS(p, DistAB, DistBC, DistAC);

	return (DistAB != 0) ? (2 * S / DistAB) : 0;
}

void GeneralizationCurve::SetValueOfScale(double_t m)
{
	M = m;
}

void GeneralizationCurve::SetParallelismMode(bool mode)
{
	parallelism_enabled = mode;
}

curve* GeneralizationCurve::SimplificationOfCurve(curve *initialPoints, uint32_t len, double_t H, uint32_t &CountOfNewPoints)
{
	curve *newPoints = new curve;
	point outermostPoint;
	X(outermostPoint) = X(*initialPoints)[0];
	Y(outermostPoint) = Y(*initialPoints)[0];

	uint32_t outermostNum = 0;
	double_t outermostDist = 0;

	if (len > 2)
	{
		point first;
		point last;

		X(first) = X(*initialPoints)[0];
		Y(first) = Y(*initialPoints)[0];

		X(last) = X(*initialPoints)[len - 1];
		Y(last) = Y(*initialPoints)[len - 1];

		for (uint32_t i = 1; i < len - 1; ++i)
		{
			point cur;

			X(cur) = X(*initialPoints)[i];
			Y(cur) = Y(*initialPoints)[i];

			if (ComputeDistBetweenPointAndLine(&first, &last, &cur) >= outermostDist)
			{
				outermostDist = ComputeDistBetweenPointAndLine(&first, &last, &cur);
				outermostNum = i;
				X(outermostPoint) = X(*initialPoints)[i];
				Y(outermostPoint) = Y(*initialPoints)[i];
			}
		}
		if (outermostDist < H)
		{
			X(*newPoints).push_back(X(*initialPoints)[0]);
			Y(*newPoints).push_back(Y(*initialPoints)[0]);

			X(*newPoints).push_back(X(*initialPoints)[outermostNum]);
			Y(*newPoints).push_back(Y(*initialPoints)[outermostNum]);

			X(*newPoints).push_back(X(*initialPoints)[len - 1]);
			Y(*newPoints).push_back(Y(*initialPoints)[len - 1]);

			CountOfNewPoints = 3;
		}
		else
		{
			curve InitPoints1, InitPoints2;
			X(InitPoints1).resize(outermostNum + 1);
			Y(InitPoints1).resize(outermostNum + 1);

			X(InitPoints2).resize(len - outermostNum);
			Y(InitPoints2).resize(len - outermostNum);

			uint32_t k = 0;
			for (uint32_t i = 0; i < outermostNum + 1; ++i)
			{
				X(InitPoints1)[k] = X(*initialPoints)[i];
				Y(InitPoints1)[k] = Y(*initialPoints)[i];
				k++;
			}

			k = 0;

			for (uint32_t i = outermostNum; i < len; ++i)
			{
				X(InitPoints2)[k] = X(*initialPoints)[i];
				Y(InitPoints2)[k] = Y(*initialPoints)[i];
				k++;
			}

			uint32_t CountOfNewPoints1 = 0;
			curve* NewPoints1 = SimplificationOfCurve(&InitPoints1, outermostNum + 1, H, CountOfNewPoints1);
			uint32_t CountOfNewPoints2 = 0;
			curve* NewPoints2 = SimplificationOfCurve(&InitPoints2, len - outermostNum, H, CountOfNewPoints2);

			k = 0;
			for (uint32_t i = 0; i < CountOfNewPoints1 - 1; ++i)
			{
				X(*newPoints).push_back(X(*NewPoints1)[i]);
				Y(*newPoints).push_back(Y(*NewPoints1)[i]);
			}
			for (uint32_t i = 0; i < CountOfNewPoints2; ++i)
			{
				X(*newPoints).push_back(X(*NewPoints2)[i]);
				Y(*newPoints).push_back(Y(*NewPoints2)[i]);
			}

			delete NewPoints1;
			delete NewPoints2;

			CountOfNewPoints = CountOfNewPoints1 - 1 + CountOfNewPoints2;
		}
	}
	else if (len > 1)
	{
		X(*newPoints).push_back(X(*initialPoints)[0]);
		Y(*newPoints).push_back(Y(*initialPoints)[0]);

		X(*newPoints).push_back(X(*initialPoints)[len - 1]);
		Y(*newPoints).push_back(Y(*initialPoints)[len - 1]);

		CountOfNewPoints = 2;
	}
	else
	{
		X(*newPoints).push_back(X(*initialPoints)[0]);
		Y(*newPoints).push_back(Y(*initialPoints)[0]);

		CountOfNewPoints = 1;
	}

	return newPoints;
}

curve* GeneralizationCurve::SimplificationOfSegmentMath(curve* AdductionPointsInSegment,
	uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints)
{
	uint32_t Len = *CountOfAdductionPointsInSegment;
	double_t H = pow(M, 2 - AngularCoeffRegresLine[i]);

	curve* NewPoints = SimplificationOfCurve(AdductionPointsInSegment, Len, H, CountOfPoints);

	return NewPoints;
}

curve* GeneralizationCurve::SimplificationOfSegmentMkl(curve* AdductionPointsInSegment,
	uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints)
{
	uint32_t Len = *CountOfAdductionPointsInSegment;

	double_t H;
	vmdPowx(1, &M, 2 - AngularCoeffRegresLine[i], &H, 0);

	curve* NewPoints = SimplificationOfCurve(AdductionPointsInSegment, Len, H, CountOfPoints);

	return NewPoints;
}

void GeneralizationCurve::Simplification()
{
	SimplificationInner[math_type]();
}

void GeneralizationCurve::SimplificationMath()
{
	const uint32_t k = 8;
	std::vector<std::vector<uint32_t>> NE;
	std::vector<std::vector<double_t>> Dbc; // Box-Counting
	NE.resize(ResultSegmentCount);
	Dbc.resize(ResultSegmentCount);

	double_t X = 0;
	double_t X2 = 0;
	std::vector<double_t> Y, XY, LinearRegK, LinearRegB;
	Y.resize(ResultSegmentCount);
	XY.resize(ResultSegmentCount);
	LinearRegK.resize(ResultSegmentCount);
	LinearRegB.resize(ResultSegmentCount);
	AngularCoeffRegresLine.resize(ResultSegmentCount);
	// UInt32[, ] NE = new UInt32[ResultSegmentCount, k];
	// Double[, ] Dbc = new Double[ResultSegmentCount, k]; // Box-Counting

	double_t dist[k] = { Radius * 1, Radius * 2, Radius * 3, Radius * 4,
		Radius * 5, Radius * 6, Radius * 7, Radius * 8 };
	double_t reverse_dist[k] = { (1 / dist[0]), (1 / dist[1]), (1 / dist[2]), (1 / dist[3]),
		(1 / dist[4]), (1 / dist[5]), (1 / dist[6]), (1 / dist[7]) };

	for (uint32_t i = 0; i < k; ++i)
	{
		X += (Radius * (i + 1));
		X2 += ((Radius * (i + 1)) * (Radius * (i + 1)));
	}

	X /= 10;
	X2 /= 10;

	TotalCountOfPointsAfterSimplification = 0;
	PointsAfterSimplification = new curve*[ResultSegmentCount];
	CountOfPointsAfterSimplification.resize(ResultSegmentCount);

//#pragma omp parallel for if ((parallelism_enabled) && (ResultSegmentCount >= 4))
	for (int32_t i = 0; i < ResultSegmentCount; ++i)
	{
		NE[i].resize(k);
		Dbc[i].resize(k);
		for (uint32_t j = 0; j < k; ++j)
		{
			NE[i][j] = (ComputeQuadricsMath(i, Radius * (j + 1)));
			Dbc[i][j] = (log10(NE[i][j]) / log10(1 / (Radius * (j + 1))));
			Y[i] += Dbc[i][j];
		}
		Y[i] /= 10;
		XY[i] = X * Y[i];
		XY[i] /= 10;

		LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * X * X);
		LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - X * X);
		AngularCoeffRegresLine[i] = LinearRegK[i];
		uint32_t CountOfPoints = 0;

		PointsAfterSimplification[i] = SimplificationOfSegmentMath(&AdductionPointsInSegment[i],
			&CountOfAdductionPointsInSegment[i], i, CountOfPoints);

		CountOfPointsAfterSimplification[i] = CountOfPoints;
		
	}
	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
		TotalCountOfPointsAfterSimplification += CountOfPointsAfterSimplification[i];
}

void GeneralizationCurve::SimplificationMkl()
{
	const uint32_t k = 8;
	//std::vector<std::vector<uint32_t>> NE;
	//std::vector<std::vector<double_t>> Dbc; // Box-Counting
	//NE.resize(ResultSegmentCount);
	//Dbc.resize(ResultSegmentCount);
	double_t** NE = (double_t **)mkl_calloc(ResultSegmentCount, sizeof(double_t *), 64);
	double_t** Dbc = (double_t **)mkl_calloc(ResultSegmentCount, sizeof(double_t *), 64); // Box-Counting



	double_t X = 0;
	double_t X2 = 0;
	std::vector<double_t> Y, XY, LinearRegK, LinearRegB;
	Y.resize(ResultSegmentCount);
	XY.resize(ResultSegmentCount);
	LinearRegK.resize(ResultSegmentCount);
	LinearRegB.resize(ResultSegmentCount);
	AngularCoeffRegresLine.resize(ResultSegmentCount);

	std::atomic_uint_fast32_t total_count_atomic{ 0 };

	TotalCountOfPointsAfterSimplification = 0;
	PointsAfterSimplification = new curve*[ResultSegmentCount];
	CountOfPointsAfterSimplification.resize(ResultSegmentCount);

	/*double_t *dist = (double_t *)mkl_calloc(k, sizeof(*dist), 64);*/
	/*double_t *reverse_dist = (double_t *)mkl_calloc(k, sizeof(*reverse_dist), 64);*/
	double_t *reverse_dist_log10 = (double_t *)mkl_calloc(k, sizeof(*reverse_dist_log10), 64);
	double_t *dist2 = (double_t *)mkl_calloc(k, sizeof(*dist2), 64);
	double_t dist[k] = { Radius * 1, Radius * 2, Radius * 3, Radius * 4,
		Radius * 5, Radius * 6, Radius * 7, Radius * 8 };
	double_t reverse_dist[k] = { (1 / dist[0]), (1 / dist[1]), (1 / dist[2]), (1 / dist[3]),
		(1 / dist[4]), (1 / dist[5]), (1 / dist[6]), (1 / dist[7]) };
	double_t E[k]{ 1, 1, 1, 1, 1, 1, 1, 1 };

	vmdPowx(k, dist, 2, dist2, 0);
	/* X += dist[0] + dist[1] + ... */
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, 1, k, 1, dist, k, E, 1, 0, &X, 1);
	/* X2 += dist2[0] + dist2[1] + ... */
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, 1, k, 1, dist2, k, E, 1, 0, &X2, 1);

	vmdLog10(k, (const double_t *)reverse_dist, reverse_dist_log10, 0);

	mkl_free(dist2);

	X /= 10;
	X2 /= 10;

#pragma omp parallel for if ((this->parallelism_enabled) && (ResultSegmentCount >= 4))
	for (int i = 0; i < ResultSegmentCount; ++i)
	{
		NE[i] = (double_t *)mkl_calloc(k, sizeof(double_t), 64);
		Dbc[i] = (double_t *)mkl_calloc(k, sizeof(double_t), 64);

		for (uint32_t j = 0; j < k; ++j)
		{
			double_t NElog10 = 0;

			NE[i][j] = (double_t)(ComputeQuadricsMkl(i, dist[j]));
			vmdLog10(1, (const double_t *)&NE[i][j], &NElog10, 0);
			Dbc[i][j] = NElog10 / reverse_dist_log10[j];
			Y[i] += Dbc[i][j];
		}

		Y[i] /= 10;
		XY[i] = X * Y[i];
		XY[i] /= 10;

		LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * X * X);
		LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - X * X);
		AngularCoeffRegresLine[i] = LinearRegK[i];
		uint32_t CountOfPoints = 0;

		PointsAfterSimplification[i] = SimplificationOfSegmentMkl(&AdductionPointsInSegment[i],
			&CountOfAdductionPointsInSegment[i], i, CountOfPoints);

		CountOfPointsAfterSimplification[i] = CountOfPoints;
		total_count_atomic.fetch_add(CountOfPointsAfterSimplification[i]);

		mkl_free(NE[i]);
		mkl_free(Dbc[i]);
	}
	TotalCountOfPointsAfterSimplification = total_count_atomic;
	mkl_free(reverse_dist_log10);
	mkl_free(NE);
	mkl_free(Dbc);
}

void GeneralizationCurve::Smoothing()
{
	CountOfPointsAfterSmoothing.resize(ResultSegmentCount);
	PointsAfterSmoothing = new curve*[ResultSegmentCount];
	TotalCountOfPointsAfterSmoothing = 0;
	std::atomic_uint_fast32_t total_count_atomic{ 0 };

#pragma omp parallel for if ((this->parallelism_enabled) && (ResultSegmentCount >= 4))
	for (int32_t i = 0; i < ResultSegmentCount; ++i)
	{
		CountOfPointsAfterSmoothing[i] = CountOfPointsAfterSimplification[i];
		/*X(*PointsAfterSmoothing[i]).resize(CountOfPointsAfterSmoothing[i]);
		Y(*PointsAfterSmoothing[i]).resize(CountOfPointsAfterSmoothing[i]);*/

		total_count_atomic.fetch_add(CountOfPointsAfterSmoothing[i]);

		PointsAfterSmoothing[i] = new curve;

		X(*PointsAfterSmoothing[i]).push_back(X(*PointsAfterSimplification[i])[0]);
		Y(*PointsAfterSmoothing[i]).push_back(Y(*PointsAfterSimplification[i])[0]);

		for (uint32_t j = 1; j < CountOfPointsAfterSmoothing[i] - 1; ++j)
		{
			X(*PointsAfterSmoothing[i]).push_back((X(*PointsAfterSimplification[i])[j - 1]
				+ 4 * X(*PointsAfterSimplification[i])[j] + X(*PointsAfterSimplification[i])[j + 1]) / 6);
			Y(*PointsAfterSmoothing[i]).push_back((Y(*PointsAfterSimplification[i])[j - 1]
				+ 4 * Y(*PointsAfterSimplification[i])[j] + Y(*PointsAfterSimplification[i])[j + 1]) / 6);
		}
		X(*PointsAfterSmoothing[i]).push_back(
			X(*PointsAfterSimplification[i])[CountOfPointsAfterSimplification[i] - 1]);
		Y(*PointsAfterSmoothing[i]).push_back(
			Y(*PointsAfterSimplification[i])[CountOfPointsAfterSimplification[i] - 1]);
	}

	TotalCountOfPointsAfterSmoothing = total_count_atomic;
}

double_t GeneralizationCurve::ComputeSinuosityCoeff(curve *Obj,
						    uint32_t countOfPoints,
						    uint32_t numOfSegm)
{
	curve *segments = new curve[numOfSegm];
	std::vector<double_t> *distances = new std::vector<double_t>[numOfSegm];
	std::vector<double_t> sumOfDistances;
	std::vector<double_t> boundDist;
	std::vector<double_t> Ksegm;
	std::vector<uint32_t> objSizes;
	double_t K = 0;
	uint32_t numPointsPerSegm = countOfPoints / numOfSegm;
	uint32_t remainPart = countOfPoints % numOfSegm;
	uint32_t iter = 0;

	boundDist.resize(numOfSegm);
	Ksegm.resize(numOfSegm);
	objSizes.resize(numOfSegm);
	sumOfDistances.resize(numOfSegm);

	if (remainPart > 0)
	{
		for (uint32_t i = 0; i < (numOfSegm - 1); i++)
		{
			distances[i].resize(numPointsPerSegm - 1);
			objSizes[i] = numPointsPerSegm;
			X(segments[i]).resize(numPointsPerSegm);
			Y(segments[i]).resize(numPointsPerSegm);

			for (uint32_t j = 0; j < numPointsPerSegm; j++)
			{
				X(segments[i])[j] = X(*Obj)[iter];
				Y(segments[i])[j] = Y(*Obj)[iter];
				iter++;
			}

			sumOfDistances[i] = UtilComputeDistances[math_type](
				&segments[i],
				objSizes[i],
				&distances[i]
			);
			boundDist[i] = sqrt(
				((X(segments[i])[numPointsPerSegm - 1] - X(segments[i])[0]) *
				 (X(segments[i])[numPointsPerSegm - 1] - X(segments[i])[0])) +
				((Y(segments[i])[numPointsPerSegm - 1] - Y(segments[i])[0]) *
				 (Y(segments[i])[numPointsPerSegm - 1] - Y(segments[i])[0]))
			);
			Ksegm[i] = sumOfDistances[i] / boundDist[i];

			K += Ksegm[i];
		}
		distances[numOfSegm - 1].resize(remainPart - 1);
		objSizes[numOfSegm - 1] = remainPart;
		X(segments[numOfSegm - 1]).resize(remainPart);
		Y(segments[numOfSegm - 1]).resize(remainPart);

		for (uint32_t j = 0; j < remainPart; j++)
		{
			X(segments[numOfSegm - 1])[j] = X(*Obj)[iter];
			Y(segments[numOfSegm - 1])[j] = Y(*Obj)[iter];
			iter++;
		}

		sumOfDistances[numOfSegm - 1] = UtilComputeDistances[math_type](
			&segments[numOfSegm - 1],
			objSizes[numOfSegm - 1],
			&distances[numOfSegm - 1]
		);
		boundDist[numOfSegm - 1] = sqrt(
			((X(segments[numOfSegm - 1])[remainPart - 1] - X(segments[numOfSegm - 1])[0]) *
			 (X(segments[numOfSegm - 1])[remainPart - 1] - X(segments[numOfSegm - 1])[0])) +
			((Y(segments[numOfSegm - 1])[remainPart - 1] - Y(segments[numOfSegm - 1])[0]) *
			 (Y(segments[numOfSegm - 1])[remainPart - 1] - Y(segments[numOfSegm - 1])[0]))
		);
			
		Ksegm[numOfSegm - 1] = (objSizes[numOfSegm - 1] != 1) ?
				sumOfDistances[numOfSegm - 1] / boundDist[numOfSegm - 1] :
				0;
		K += Ksegm[numOfSegm - 1];
	}
	else
	{
		for (uint32_t i = 0; i < numOfSegm; i++)
		{
			distances[i].resize(numPointsPerSegm - 1);
			objSizes[i] = numPointsPerSegm;
			X(segments[i]).resize(numPointsPerSegm);
			Y(segments[i]).resize(numPointsPerSegm);

			for (uint32_t j = 0; j < numPointsPerSegm; j++)
			{
				X(segments[i])[j] = X(*Obj)[iter];
				Y(segments[i])[j] = Y(*Obj)[iter];
				iter++;
			}

			sumOfDistances[i] = UtilComputeDistances[math_type](
				&segments[i],
				objSizes[i],
				&distances[i]
			);
			boundDist[i] = sqrt(
				((X(segments[i])[numPointsPerSegm - 1] - X(segments[i])[0]) *
				 (X(segments[i])[numPointsPerSegm - 1] - X(segments[i])[0])) +
				((Y(segments[i])[numPointsPerSegm - 1] - Y(segments[i])[0]) *
				 (Y(segments[i])[numPointsPerSegm - 1] - Y(segments[i])[0]))
			);
			Ksegm[i] = sumOfDistances[i] / boundDist[i];

			K += Ksegm[i];
		}
	}

	K /= numOfSegm;

	return K;
}

GeneralizationCurve::~GeneralizationCurve()
{
	if (Points)
		delete[] Points;
	if (AdductionPoints)
		delete[] AdductionPoints;

	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
	{
		delete PointsAfterSimplification[i];
	}
	delete[] PointsAfterSimplification;

	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
	{
		delete PointsAfterSmoothing[i];
	}
	delete[] PointsAfterSmoothing;

	if (math_type == IntelMklMath)
		mkl_free(GridMkl);
}
