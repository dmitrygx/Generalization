#include "GeneralizationLogging.h"

bool GeneralizationLogging::verbose = false;
bool GeneralizationLogging::output_results = false;
std::ofstream GeneralizationLogging::fout;

GeneralizationLogging::GeneralizationLogging(bool verbose_,
	bool output_results_)
{
	verbose = verbose_;
	output_results = output_results_;
}


GeneralizationLogging::~GeneralizationLogging()
{
	fout.close();
}
