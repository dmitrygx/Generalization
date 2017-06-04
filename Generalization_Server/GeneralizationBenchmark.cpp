#include "GeneralizationBenchmark.h"

void GeneralizationBenchmark::GenerateCurveDataSet(uint32_t count_points)
{
	dataset = new curve;
	X(*dataset).resize(count_points);
	Y(*dataset).resize(count_points);
	for (uint32_t i = 0; i < count_points; i++)
	{
		X(*dataset)[i] = i;
		Y(*dataset)[i] = i;
		/* we develop straight line with points:
		 * (0, 0); (1, 1); ...; (K, K) */
	}
}

GeneralizationBenchmark::GeneralizationBenchmark(uint32_t pps, uint32_t seg_count,
	double_t C_, uint32_t Np_, uint32_t Ns_,
	double_t f_, uint32_t Ninit_, int parallelism)
{
	GenerateCurveDataSet(pps * seg_count);
	SetAdductionPointsPerSegment(dataset, pps, seg_count);
}

void GeneralizationBenchmark::RunBenchmarkOnSegmentedDataSet(double simplification_timer,
	double smoothing_timer)
{
	this->RunBenchmarkOnSegmentedCurve();
}

GeneralizationBenchmark::~GeneralizationBenchmark()
{
	delete dataset;
}
