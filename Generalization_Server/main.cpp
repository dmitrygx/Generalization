#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "GeneralizationFile.h"
#include <conio.h>
#include "GeneralizationCurve.h"
#include "GeneralizationServer.h"
#include "GeneralizationWorker.h"
#include "GeneralizationProgramOpt.h"
#include "Thread.h"

using namespace std;

void HandleManagerServerType(void);
void HandleStandaloneServerType(GeneralizationProgramOpt *programOpt);

//static GeneralizationFile GenFile;
//static GeneralizationDataBase GenDataBase;

int main(int argc, char* argv[])
{
	///* TODO: should be removed, only should work as a server */
	///*GenFile.ParseAllDataInFile();

	//curves Map = GenFile.GetCurves();*/

	///*for (uint32_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
	//{
	//	for (uint32_t j = 0; j < Map[i].first.size(); j++)
	//	{
	//		cout << Map[i].first[j] << " " << Map[i].second[j] << endl;
	//	}
	//}*/

	///*GeneralizationCurve *Curve = new GeneralizationCurve;

	//Curve->BuildCurve(Map[0].first.size(), &Map[0]);
	//Curve->Adduction();
	//Curve->Segmentation();
	//Curve->SetValueOfScale(8);
	//Curve->Simplification();
	//Curve->Smoothing();*/
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
