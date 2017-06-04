#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "GeneralizationBenchmark.h"

#ifdef _WIN32
#include <conio.h>
#include "GeneralizationFile.h"
#include "GeneralizationServer.h"
#include "GeneralizationWorker.h"
#include "GeneralizationProgramOpt.h"
#include "Thread.h"
#endif

using namespace std;

#ifdef _WIN32
void HandleManagerServerType(void);
void HandleStandaloneServerType(GeneralizationProgramOpt *programOpt);

int main(int argc, char* argv[])
{
	GeneralizationProgramOpt *programOpt = new GeneralizationProgramOpt(argc, argv);


	switch (programOpt->GetServerType())
	{
	case ServerType::MANAGER:
		HandleManagerServerType();
		break;
	case ServerType::STANDALONE:
		HandleStandaloneServerType(programOpt);
		break;
	default:
		break;
	}
	
}

void HandleManagerServerType(void)
{
	GeneralizationServer server(L"http://localhost/generalization_server");
	Thread<GeneralizationServer> server_thread(&server, &GeneralizationServer::start);

	///*server.GetGeneralizationDataBase()->SetDataBaseObject(Object);*/

	if (server_thread.start())
		std::cout << "Server Thread start()" << std::endl;

	cout << "Done" << endl;

	_getch();

	server.ChageStateOfServerThread(false);
	server_thread.join();
}

void HandleStandaloneServerType(GeneralizationProgramOpt *programOpt)
{
	string user_input;
	long Number;
	GeneralizationServer server(programOpt->GetDataBase(), programOpt->GetStorageType(),
		programOpt->GetParamC(), programOpt->GetParamNp(), programOpt->GetParamNs(),
		programOpt->GetParamf(), programOpt->GetParamNinit());
#ifdef _OPENMP
	server.SetParallelismMode(programOpt->IsOpenMPSupport());
#endif
	server.SetUpdateDBFlag(programOpt->IsUpdateDB());

	cout << "See output file and then specify curve Code which should be handled" << endl;
	cout << "If you prefer to handle all curve, please specify * (asterix)" << endl;
	cin >> user_input;

	if (user_input == "*")
	{
		server.HandleAllCurves(programOpt->GetStorageType());
	}
	else
	{
		cout << "Specify number of curve with code " << user_input << endl;
		cin >> Number;
		server.HandleCurve(programOpt->GetStorageType(),
			user_input, Number);
	}

	_getch();
}
#else
#include <assert.h>

struct AlgorithmParams
{
	double C;
	uint32_t Np;
	uint32_t Ns;
	double f;
	uint32_t Ninit;
	double M;
	int OpenMP;
	int IntelMKL;
};

const double _C = 0.5;
const uint32_t _Np = 900;
const uint32_t _Ninit = 900;
const uint32_t _Ns = 15;
const double _f = 5;
const double _M = 5;

int main(int argc, char* argv[])
{
	assert(argc > 5);
	GeneralizationBenchmark *BenchCurves;
	uint32_t pps = std::stoi(argv[1]);
	uint32_t min_seg_cnt = std::stoi(argv[2]);
	uint32_t max_seg_cnt = std::stoi(argv[3]);
	uint32_t Count = max_seg_cnt - min_seg_cnt + 1;
	void* raw_memory = operator new[](Count * sizeof(GeneralizationBenchmark));
	BenchCurves = static_cast<GeneralizationBenchmark*>(raw_memory);
	uint32_t iter_thr_mem = 0;

	AlgorithmParams algParams;
	algParams.C = _C;
	algParams.Np = _Np;
	algParams.Ns = _Ns;
	algParams.f = _f;
	algParams.M = _M;
	algParams.Ninit = _Ninit;
	algParams.OpenMP = std::stoi(argv[4]);
	algParams.IntelMKL = std::stoi(argv[5]);

	for (size_t i = min_seg_cnt; i <= max_seg_cnt; ++i) 
	{
		new(&BenchCurves[iter_thr_mem])GeneralizationBenchmark(pps, i, algParams.C,
			algParams.Np, algParams.Ns, algParams.f,
			algParams.Ninit, algParams.OpenMP);
		iter_thr_mem++;
	}

	for (uint32_t i = 0; i < max_seg_cnt - min_seg_cnt + 1; i++)
	{
		GeneralizationBenchmark *benchmark_curve = &BenchCurves[i];
		benchmark_curve->SetMaxValue(max_seg_cnt * pps + 1);
		benchmark_curve->SetUseOpenMP(algParams.OpenMP);
		benchmark_curve->SetUseMkl(algParams.IntelMKL);
		benchmark_curve->InitForBenchmarks();

		benchmark_curve->RunBenchmarkOnSegmentedCurve();

		std::cout << "Simplification timer:" <<
			benchmark_curve->GetsimplificationTimer();
		std::cout << " | Smoothing timer:" <<
			benchmark_curve->GetsmoothingTimer() << endl;

		benchmark_curve->~GeneralizationBenchmark();
	}

	return 0;
}

#endif
