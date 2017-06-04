#pragma once
#include <iostream>
#include <fstream>

using namespace std;

class GeneralizationLogging
{
private:
	static bool verbose;
	static bool output_results;
public:
	static std::ofstream fout;

	GeneralizationLogging(bool verbose_,
		bool output_results_);
	virtual ~GeneralizationLogging();

	static void SetVerbose(bool value)
	{
		verbose = value;
	}

	static void SetOutputResults(bool value)
	{
		output_results = value;
	}

	static bool GetVerbose(void)
	{
		return verbose;
	}

	static bool GetOutputResults(void)
	{
		return output_results;
	}

	static void InitializeLogging(std::string logging_path)
	{
#ifdef _WIN32
		fout.open(logging_path,
			  ios_base::trunc | ios_base::in | ios_base::out);
#endif
	}
};

