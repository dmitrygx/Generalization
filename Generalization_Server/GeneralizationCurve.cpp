#include "GeneralizationCurve.h"
#include <cassert>
#include <iostream>
#ifdef _OPENMP
# include <omp.h>
#endif

#define MKL_INT size_t
#include "mkl.h"

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/segment.hpp> 
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>

#include <boost/foreach.hpp>

#include <boost/geometry/algorithms/intersection.hpp>

#include <fstream>
#include <boost/assign.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/geometry/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/algorithms/envelope.hpp>

#include <boost/geometry/geometries/register/point.hpp>

#include <atomic>

#define _USE_MATH_DEFINES
#include <math.h>

template <typename Geometry1, typename Geometry2>
void create_svg(std::string const& filename, Geometry1 const& a, Geometry2 const& b)
{
	typedef typename boost::geometry::point_type<Geometry1>::type point_type;
	std::ofstream svg(filename.c_str());

	boost::geometry::svg_mapper<point_type> mapper(svg, 400, 400);
	mapper.add(a);
	mapper.add(b);

	mapper.map(a, "fill-opacity:0.5;fill:rgb(153,204,0);stroke:rgb(153,204,0);stroke-width:2");
	mapper.map(b, "opacity:0.8;fill:none;stroke:rgb(255,128,0);stroke-width:4;stroke-dasharray:1,7;stroke-linecap:round");
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
							   std::vector<uint32_t> **countOfPointInSegm)
{
	countOfSegm = ResultSegmentCount;
	*countOfPointInSegm = &CountOfAdductionPointsInSegment;

	return &AdductionPointsInSegment;
}

curve** GeneralizationCurve::GetSimplifiedCurve(uint32_t &countOfSimplSegm,
						std::vector<uint32_t> **countOfPointInSimplSegm)
{
	countOfSimplSegm = ResultSegmentCount;
	*countOfPointInSimplSegm = &CountOfPointsAfterSimplification;

	return PointsAfterSimplification;
}

curve** GeneralizationCurve::GetSmoothedCurve(uint32_t &countOfSmoothSegm,
					      std::vector<uint32_t> **countOfPointInSmoothSegm)
{
	countOfSmoothSegm = ResultSegmentCount;
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

	AverageDistance = 0;
	Radius = 0;
}

double_t GeneralizationCurve::ComputeDistances()
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

double_t GeneralizationCurve::ComputeAvarageDistance()
{
	double_t sum = ComputeDistances();

	return sum / (CountPoints - 1);
}

double_t GeneralizationCurve::ComputeRadius(double_t averageDistance)
{
	return C * ComputeAvarageDistance();
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

point* GeneralizationCurve::CheckInterSection(point point1, point point2,
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

	//if (X1 == X2)
	//{
	//	//System.Windows.Forms.MessageBox.Show(" Œ‰Ì‡ ÚÓ˜Í‡ ÔÂÂÒÂ˜ÂÌËˇ " + X1.ToString() + " " + Y1.ToString());
	//}
	//else
	//{
	//	//System.Windows.Forms.MessageBox.Show(" ƒ‚Â ÚÓ˜ÍË ÔÂÂÒÂ˜ÂÌËˇ " + X1.ToString() + " " + Y1.ToString() + " Ë " + X2.ToString() + " " + Y2.ToString());
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

		res = CheckInterSection(adductionPoint, pnt, Radius, adductionPoint);
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

void GeneralizationCurve::Segmentation()
{
	std::vector<double_t> Dist;
	Dist.resize(AdductionCount - 1);

	for (uint32_t i = 0; i < AdductionCount - 1; i++)
	{
		double_t DistPointSub[2] {
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

	Integral—haract.resize(CountOfSegments);
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
		}
		FullRotationOfSegment[i] = RotationOfSegment[i] / (2 * M_PI); /* Wi */
	}

	LocalMax.resize(CountOfSegments); /* Ei */

	for (uint32_t i = 0; i < CountOfSegments; i++)
	{
		LocalMax[i] = 0;
		for (uint32_t j = 1; j < CountOfSegments - 1; j++)
		{
			if ((AngleOfRotation[i][j] > AngleOfRotation[i][j - 1]) &&
				(AngleOfRotation[i][j] > AngleOfRotation[i][j + 1]))
			{
				LocalMax[i]++;
			}
		}
		Integral—haract[i] = FullRotationOfSegment[i] + f * LocalMax[i]; /* Mi */
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

	for (uint32_t i = 0; i < CountOfSegments; ++i)
	{
		CountOfAdductionPointsInSegment[i] = 0;
	}

	ResultSegmentCount = 0;
	while (ShouldUnionSegment)
	{
		for (uint32_t i = 0; i < CountOfSegments; i += 2)
		{
			if (i + 1 != CountOfSegments)
			{
				if ((Integral—haract[i + 1] - Integral—haract[i] >= -10) && (Integral—haract[i + 1] - Integral—haract[i] <= 10))
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

#define Grid(i,j) Grid[(i)* dimension[1] + (j)]

using point_t = boost::geometry::model::d2::point_xy<double>;
using polygon_t = boost::geometry::model::polygon<point_t>;
using linestring_t = boost::geometry::model::linestring<point_t>;
using multi_linestring_t = boost::geometry::model::multi_linestring<linestring_t>;

uint32_t GeneralizationCurve::OldComputeQuadrics(uint32_t CurrentSegment, double_t dist)
{
	uint32_t CountOfSquare = 0;
	// std::vector<std::vector<bool>> Grid;
	std::vector<std::map<uint32_t, bool>> Grid;
	Grid.resize(10000);

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
					if (!Grid[i][j])
					{
						CountOfSquare++;
					}
					Grid[i][j] = true;
				}
			}
		}
	}

	return CountOfSquare;
}

uint32_t GeneralizationCurve::ComputeQuadrics(uint32_t CurrentSegment, double_t dist)
{
	uint32_t CountOfSquare = 0;
	// std::vector<std::vector<bool>> Grid;
	MKL_LONG dimension[2] = { 10000, 10000 };
	bool *Grid = (bool *) mkl_calloc(1, dimension[0] * dimension[1] * sizeof(*Grid),
					 64);

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
#if 0
				polygon_t square;
				square.outer().push_back(point_t(i, j));
				square.outer().push_back(point_t(i + 1, j));
				square.outer().push_back(point_t(i + 1, j + 1));
				square.outer().push_back(point_t(i, j + 1));
				square.outer().push_back(point_t(i, j));
#endif
//#endif
//#ifndef BOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX
//				segment_t line {
//					{
//						(double_t)X(AdductionPointsInSegment[CurrentSegment])[k], 
//						(double_t)Y(AdductionPointsInSegment[CurrentSegment])[k]
//					},
//					{
//						(double_t)X(AdductionPointsInSegment[CurrentSegment])[k + 1],
//						(double_t)Y(AdductionPointsInSegment[CurrentSegment])[k + 1]
//					}
//				};
//#else
#if 0
				linestring_t line;
				line.push_back(point_t {
					X(AdductionPointsInSegment[CurrentSegment])[k],
					Y(AdductionPointsInSegment[CurrentSegment])[k]
				});
				line.push_back(point_t{
					X(AdductionPointsInSegment[CurrentSegment])[k + 1],
					Y(AdductionPointsInSegment[CurrentSegment])[k + 1]
				});
#endif

//#endif
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

	mkl_free(Grid);

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

double_t GeneralizationCurve::ComputeDistBetweenPoints(point *A, point *B)
{
	double arg[2] {
		(X(*B) - X(*A)),
		(Y(*B) - Y(*A))
	};
	double argXY[2];
	vmdPowx(2, arg, 2, argXY, 0);
	double SumXYarg = argXY[0] + argXY[1];

	return SumXYarg * SumXYarg;
}

double_t GeneralizationCurve::ComputeP(double_t DistAB, double_t DistBC, double_t DistAC)
{
	return (0.5 * (DistAB + DistBC + DistAC));
}

double_t GeneralizationCurve::ComputeS(double_t p, double_t DistAB, double_t DistBC, double_t DistAC)
{
	double Arg = (uint64_t)p * ((uint64_t)p - (uint64_t)DistAB) * ((uint64_t)p - (uint64_t)DistBC) * ((uint64_t)p - (uint64_t)DistAC);
	return Arg * Arg;
}

double_t GeneralizationCurve::ComputeDistBetweenPointAndLine(point *A, point *B, point *C)
{
	double_t DistAB = ComputeDistBetweenPoints(A, B);
	double_t DistBC = ComputeDistBetweenPoints(B, C);
	double_t DistAC = ComputeDistBetweenPoints(A, C);
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

curve* GeneralizationCurve::SimplificationOfSegment(curve* AdductionPointsInSegment,
	uint32_t *CountOfAdductionPointsInSegment, uint32_t i, uint32_t &CountOfPoints)
{
	uint32_t Len = *CountOfAdductionPointsInSegment;
	curve PointsInSegment;
	X(PointsInSegment).resize(Len);
	Y(PointsInSegment).resize(Len);

	uint32_t k = 0;
	for (uint32_t j = 0; j < Len; ++j)
	{
		X(PointsInSegment)[j] = X(*AdductionPointsInSegment)[k];
		Y(PointsInSegment)[j] = Y(*AdductionPointsInSegment)[k];
		k++;
	}

	double_t H;
	vmdPowx(1, &M, 2 - AngularCoeffRegresLine[i], &H, 0);

	curve* NewPoints = SimplificationOfCurve(&PointsInSegment, Len, H, CountOfPoints);

	return NewPoints;
}

void GeneralizationCurve::OldSimplification()
{
	uint32_t k = 8;
	std::vector<std::vector<uint32_t>> NE;
	std::vector<std::vector<double_t>> Dbc; // Box-Counting
	NE.resize(ResultSegmentCount);
	Dbc.resize(ResultSegmentCount);
	// UInt32[, ] NE = new UInt32[ResultSegmentCount, k];
	// Double[, ] Dbc = new Double[ResultSegmentCount, k]; // Box-Counting
#pragma omp parallel for if ((parallelism_enabled) && (ResultSegmentCount >= 4))
	for (int i = 0; i < ResultSegmentCount; ++i)
	{
		std::cout << omp_get_num_threads() << std::endl;
		NE[i].resize(k);
		Dbc[i].resize(k);
		for (uint32_t j = 0; j < k; ++j)
		{
			NE[i][j] = (ComputeQuadrics(i, Radius * (j + 1)));
			Dbc[i][j] = (log10(NE[i][j]) / log10(1 / (Radius * (j + 1))));
		}
	}

	double_t X = 0;
	double_t X2 = 0;
	std::vector<double_t> Y, XY, LinearRegK, LinearRegB;
	Y.resize(ResultSegmentCount);
	XY.resize(ResultSegmentCount);
	LinearRegK.resize(ResultSegmentCount);
	LinearRegB.resize(ResultSegmentCount);
	AngularCoeffRegresLine.resize(ResultSegmentCount);

	for (uint32_t i = 0; i < k; ++i)
	{
		X += (Radius * (i + 1));
		X2 += (Radius * (i + 1)) * (Radius * (i + 1));
	}

	X /= 10;
	X2 /= 10;

	TotalCountOfPointsAfterSimplification = 0;
	PointsAfterSimplification = new curve*[ResultSegmentCount];
	CountOfPointsAfterSimplification.resize(ResultSegmentCount);

#pragma omp parallel for if ((parallelism_enabled) && (ResultSegmentCount >= 4))
	for (int32_t i = 0; i < ResultSegmentCount; ++i)
	{
		for (int32_t j = 0; j < k; ++j)
		{
			Y[i] += Dbc[i][j];
		}
		Y[i] /= 10;
		XY[i] = X * Y[i];
		XY[i] /= 10;

		LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * X*X);
		LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - X*X);
		AngularCoeffRegresLine[i] = LinearRegK[i];
		uint32_t CountOfPoints = 0;

		PointsAfterSimplification[i] = SimplificationOfSegment(&AdductionPointsInSegment[i],
			&CountOfAdductionPointsInSegment[i], i, CountOfPoints);

		CountOfPointsAfterSimplification[i] = CountOfPoints;

	}
	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
		TotalCountOfPointsAfterSimplification += CountOfPointsAfterSimplification[i];
}

void GeneralizationCurve::Simplification()
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

	std::atomic_uint_fast32_t total_count_atomic{0};

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
	double_t E[k] { 1, 1, 1, 1, 1, 1, 1, 1 };

	vmdPowx(k, dist, 2, dist2, 0);
	/* X += dist[0] + dist[1] + ... */
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, 1, k, 1, dist, k, E, 1, 0, &X, 1);
	/* X2 += dist2[0] + dist2[1] + ... */
	cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, 1, k, 1, dist2, k, E, 1, 0, &X2, 1);

	vmdLog10(k, (const double_t *)reverse_dist, reverse_dist_log10, 0);

	mkl_free(dist2);
	mkl_free(reverse_dist_log10);

	X /= 10;
	X2 /= 10;

	if (!use_mkl)
	{
#pragma omp parallel for if ((this->parallelism_enabled) && (ResultSegmentCount >= 4))
		for (int i = 0; i < ResultSegmentCount; ++i)
		{
			/*std::cout << omp_get_num_threads() << std::endl;*/
			/*NE[i].resize(k);
			Dbc[i].resize(k);*/
			for (uint32_t j = 0; j < k; ++j)
			{
				NE[i][j] = (ComputeQuadrics(i, Radius * (j + 1)));
				/*std::cout << "..." << omp_get_thread_num() << "...";*/
				Dbc[i][j] = (log10(NE[i][j]) / log10(1 / (Radius * (j + 1))));
				/*std::cout << "[" << i << "][" << j << "]" << " " << NE[i][j] << "; " << Dbc[i][j] << std::endl;*/
			}
		}
	}
	else
	{
//#pragma omp parallel for if ((this->parallelism_enabled) && (ResultSegmentCount >= 4))
		for (int i = 0; i < ResultSegmentCount; ++i)
		{
			NE[i] = (double_t *)mkl_calloc(k, sizeof(double_t), 64);
			Dbc[i] = (double_t *)mkl_calloc(k, sizeof(double_t), 64);

			for (uint32_t j = 0; j < k; ++j)
			{
				double_t NElog10 = 0;

				NE[i][j] = (double_t) (ComputeQuadrics(i, dist[j]));
				vmdLog10(1, (const double_t *)&NE[i][j], &NElog10, 0);

				/*std::cout << "..." << omp_get_thread_num() << "..." << this->parallelism_enabled << "..."
					<< omp_get_num_threads() << "..." << std::endl;*/
				Dbc[i][j] = NElog10 / reverse_dist_log10[j];
				/*std::cout << "[" << i << "][" << j << "]" << " " << NE[i][j] << "; " << Dbc[i][j] << std::endl;*/
				Y[i] += Dbc[i][j];
			}

			Y[i] /= 10;
			XY[i] = X * Y[i];
			XY[i] /= 10;



			LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * X * X);
			LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - X * X);
			AngularCoeffRegresLine[i] = LinearRegK[i];
			uint32_t CountOfPoints = 0;

			PointsAfterSimplification[i] = SimplificationOfSegment(&AdductionPointsInSegment[i],
				&CountOfAdductionPointsInSegment[i], i, CountOfPoints);

			CountOfPointsAfterSimplification[i] = CountOfPoints;
			total_count_atomic.fetch_add(CountOfPointsAfterSimplification[i]);

			mkl_free(NE[i]);
			mkl_free(Dbc[i]);
		}
	}
	TotalCountOfPointsAfterSimplification = total_count_atomic;
	mkl_free(NE);
	mkl_free(Dbc);

//#pragma omp parallel for if ((parallelism_enabled) && (ResultSegmentCount >= 4))
//	for (int32_t i = 0; i < ResultSegmentCount; ++i)
//	{
//		for (int32_t j = 0; j < k; ++j)
//		{
//			Y[i] += Dbc[i][j];
//		}
//		Y[i] /= 10;
//		XY[i] = X * Y[i];
//		XY[i] /= 10;
//
//		LinearRegK[i] = (XY[i] * X * Y[i]) / (X2 * System::Math::Pow(X, 2));
//		LinearRegB[i] = (X2 * Y[i] - X * XY[i]) / (X2 - System::Math::Pow(X, 2));
//		AngularCoeffRegresLine[i] = LinearRegK[i];
//		uint32_t CountOfPoints = 0;
//
//		PointsAfterSimplification[i] = SimplificationOfSegment(&AdductionPointsInSegment[i],
//			&CountOfAdductionPointsInSegment[i], i, CountOfPoints);
//
//		CountOfPointsAfterSimplification[i] = CountOfPoints;
//		TotalCountOfPointsAfterSimplification += CountOfPointsAfterSimplification[i];
//
//		mkl_free(Dbc[i]);
//	}
	/*for (uint32_t i = 0; i < ResultSegmentCount; ++i)
		TotalCountOfPointsAfterSimplification += CountOfPointsAfterSimplification[i];*/
}

void GeneralizationCurve::Smoothing()
{
	CountOfPointsAfterSmoothing.resize(ResultSegmentCount);
	PointsAfterSmoothing = new curve*[ResultSegmentCount];
	TotalCountOfPointsAfterSmoothing = 0;

	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
	{
		CountOfPointsAfterSmoothing[i] = CountOfPointsAfterSimplification[i];
		/*X(*PointsAfterSmoothing[i]).resize(CountOfPointsAfterSmoothing[i]);
		Y(*PointsAfterSmoothing[i]).resize(CountOfPointsAfterSmoothing[i]);*/

		TotalCountOfPointsAfterSmoothing += CountOfPointsAfterSmoothing[i];

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
}

GeneralizationCurve::~GeneralizationCurve()
{
	delete[] Points;
	delete[] AdductionPoints;

	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
	{
		delete[] PointsAfterSimplification[i];
	}
	delete[] PointsAfterSimplification;

	for (uint32_t i = 0; i < ResultSegmentCount; ++i)
	{
		delete[] PointsAfterSmoothing[i];
	}
	delete[] PointsAfterSmoothing;
}
