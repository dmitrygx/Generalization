#include "GeneralizationCurve.h"


GeneralizationCurve::GeneralizationCurve()
{
	C = 0.5;
	Np = 500;
	Ns = 50;
	f = 5;
	Ninit = 1000;
}

GeneralizationCurve::GeneralizationCurve(double_t C_ = 0.5, uint32_t Np_ = 500,
	uint32_t Ns_ = 50, double_t f_ = 5, uint32_t Ninit_ = 1000)
{
	C = C_;
	Np = Np_;
	Ns = Ns_;
	f = f_;
	Ninit = Ninit_;
}

void GeneralizationCurve::BuildCurve(uint32_t _countOfPoints)
{
	CountPoints = _countOfPoints;

	Point.first.resize(CountPoints);
	Point.second.resize(CountPoints);

	Distance.resize(CountPoints - 1);

	AverageDistance = 0;
	Radius = 0;
}

double_t GeneralizationCurve::ComputeDistances()
{
	double_t sum = 0;
	for (uint32_t i = 0; i < CountPoints - 1; i++)
	{
		Distance[i] = System::Math::Sqrt(System::Math::Pow((X(Point)[i+1] - X(Point)[i]), 2) +
			System::Math::Pow((Y(Point)[i + 1] - Y(Point)[i]), 2));

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
	double_t k = -((y1 - y2) / (x2 - x1));
	double_t b = -((x1 * y2 - x2 * y1) / (x2 - x1));
	double_t D = (System::Math::Pow((2 * k * b - 2 * X(pointCircle) - 2 * Y(pointCircle) * k), 2) -
		(4 + 4 * k * k) * (b * b - radius * radius + 
		X(pointCircle) * X(pointCircle) +
		Y(pointCircle) * Y(pointCircle) -
		2 * Y(pointCircle) * b));
	if (D < 0)
	{
		//System.Windows.Forms.MessageBox.Show("D (" + D.ToString() + ") < 0");
		return nullptr;
	}
	
	double_t X1 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) - System::Math::Sqrt(D)) / (2 + 2 * k * k));
	double_t X2 = ((-(2 * k * b - 2 * x1 - 2 * y1 * k) + System::Math::Sqrt(D)) / (2 + 2 * k * k));
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

	X(AdductionPointsTmp)[0] = X(Point)[0];
	Y(AdductionPointsTmp)[0] = Y(Point)[0];

	X(AdductionPoints)[0] = X(AdductionPointsTmp)[0];
	Y(AdductionPoints)[0] = Y(AdductionPointsTmp)[0];

	uint32_t count = 1;
	uint32_t k = 1;
	uint32_t i = 0;

	while (true)
	{
		point *res;

		point adductionPoint;
		X(adductionPoint) = X(AdductionPoints)[count - 1];
		Y(adductionPoint) = Y(AdductionPoints)[count - 1];

		point pnt;
		X(pnt) = X(Point)[i + k];
		Y(pnt) = Y(Point)[i + k];

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

			X(AdductionPoints)[count] = X(AdductionPointsTmp)[i];
			Y(AdductionPoints)[count] = Y(AdductionPointsTmp)[i];

			count++;
			i = i + k - 1;
			k = 1;
		}

		if (i + k > CountPoints - 1)
		{
			X(AdductionPoints)[count] = X(Point)[CountPoints - 1];
			Y(AdductionPoints)[count] = Y(Point)[CountPoints - 1];

			count++;
			break;
		}
	}

	AdductionCount = count;
}

GeneralizationCurve::~GeneralizationCurve()
{
}
