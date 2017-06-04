#pragma once
#include "GeneralizationRequestCurve.h"


class GeneralizationBenchmark :
	public GeneralizationRequestCurve
{
private:
	curve *dataset;
	void GenerateCurveDataSet(uint32_t count_points);
public:
	GeneralizationBenchmark(uint32_t pps, uint32_t seg_count,
		double_t C_, uint32_t Np_, uint32_t Ns_,
		double_t f_, uint32_t Ninit_, int parallelism);
	void RunBenchmarkOnSegmentedDataSet(double simplification_timer,
					    double smoothing_timer);
	virtual ~GeneralizationBenchmark();
};

