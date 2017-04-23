#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "GeneralizationFile.h"
#include <conio.h>
#include "GeneralizationCurve.h"
#include "GeneralizationServer.h"
#include "GeneralizationWorker.h"

#include "Thread.h"

using namespace std;

int InitDataBase(GBASE *data_base, GBASE_OBJECT *Object, long &Count);
void FinalizeDataBase(GBASE *data_base, GBASE_OBJECT *Object);

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

	GeneralizationServer server(L"http://localhost/generalization_server");
	Thread<GeneralizationServer> server_thread(&server, &GeneralizationServer::start);

	///*server.GetGeneralizationDataBase()->SetDataBaseObject(Object);*/

	if (server_thread.start())
		std::cout << "Server Thread start()" << std::endl;

	//cout << "Count of objects " << Count << endl;
	/*GeneralizationDataBase *GenDataBase = new GeneralizationDataBase;
	Thread<GeneralizationDataBase> dB_thread(GenDataBase, &GeneralizationDataBase::InitializeDataBase);
	if (dB_thread.start())
		std::cout << "DataBase Thread start()" << std::endl;

	dB_thread.join();*/



	cout << "Done" << endl;


	_getch();

	/*FinalizeDataBase(dataBase, Object);*/

	server.ChageStateOfServerThread(false);

	server_thread.join();
}

//int InitDataBase(GBASE *data_base, GBASE_OBJECT *Object, long &Count)
//{
//	int result = 0;
//	int err = 0;
//	char *errStr;
//	GBASE_QUERY Query;
//	GBASE *dataBase;
//	BASE_INT kod_in[10] = { 0 };
//
//	GenDataBase.setFileName("C:\\Users\\dgladkov\\Documents\\Education\\61LIN.DPF");
//
//	data_base = NULL;
//	assert(Object);
//
//	dataBase = GenDataBase.getDataBaseFunc()->BaseOpen(&err,
//		const_cast<char*>(
//			GenDataBase.getFileName()->c_str()),
//		"U", NULL, NULL);
//	if (err != 0)
//	{
//		errStr = GenDataBase.getDataBaseFunc()->BaseOpenTextError(err, "R");
//		wcout << "BaseOpen failed: " << errStr << endl;
//		return -1;
//	}
//
//	data_base = dataBase;
//
//	err = GenDataBase.getDataBaseFunc()->BaseInitObject(Object, NULL, 0, NULL, 1,
//		NULL, 1, NULL, 0, NULL, 0,
//		NULL, 0, NULL, 0);
//	if (err != 0)
//	{
//		wcout << "BaseInitObject failed: " << endl;
//		return -1;
//	}
//
//	err = GenDataBase.getDataBaseFunc()->BaseObjectCount(dataBase, kod_in,
//		OPEN_TREE, &Count);
//	if (err != 0)
//	{
//		wcout << "BaseObjectCount failed: " << endl;
//		return -1;
//	}
//
//	GenDataBase.getDataBaseFunc()->BaseInitQuery(&Query, 32767, 1);
//
//	BASE_INT object1[] = { 3, 6, 3, 1 }; // number = 1
//	memset(kod_in, 0, sizeof(BASE_INT) * 10);
//
//	/*Object.pSquare = new BASE_INT[100000];
//	Object.qSquare = 100000;*/
//	Object->FlagMet = BASE_NEW_FORMAT;
//
//	err = GenDataBase.getDataBaseFunc()->BaseReadObject(dataBase, &Query, NULL,
//							    OPEN_CODE, OBJ_FULL,
//							    Object, object1, 0);
//	if (err != 0)
//	{
//		wcout << "BaseReadObject failed: Error = " << err << endl;
//		if (err == 4) {
//			wcout << "The extension of error in BaseReadObject = " <<
//				Object->Error[0] << endl;
//			wcout << "The Obj number is = " <<
//				Object->Number << endl;
//		}
//		return -1;
//	}
//
//	/*for (size_t iter = 0; iter < Object.qSquare * 4; iter++)
//	{
//	cout << (BASE_INT)((BASE_INT*) Object.pSquare)[iter] << endl;
//	}*/
//
//	err = GenDataBase.getDataBaseFunc()->BasePrintObjectToFile(
//		"output.txt", 0, NULL, Object,
//		OBJ_FULL, NULL, dataBase->CountItemsMet,
//		dataBase->SizeItemMet, dataBase->TypeItemMet);
//	if (err != 0)
//	{
//		wcout << "BasePrintObjectToFile failed: Error = " << err << endl;
//	}
//
//	return result;
//}
//
//void FinalizeDataBase(GBASE *data_base, GBASE_OBJECT *Object)
//{
//	int err = 0;
//
//	err = GenDataBase.getDataBaseFunc()->BaseCloseObject(Object);
//	if (err != 0)
//	{
//		wcout << "BaseCloseObject failed: " << endl;
//	}
//
//	err = GenDataBase.getDataBaseFunc()->BaseClose(data_base);
//	if (err != 0)
//	{
//		wcout << "BaseClose failed: " << endl;
//	}
//}