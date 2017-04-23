#include <cstdio>
#include <iostream>
#include "Generalization_ProgramOpt.h"
#include "Windows.h"
#include "tchar.h"
#include "GeneralizationDataBase.h"

using namespace std;

static GeneralizationDataBase GenDataBase;

int main(int argc, char* argv[])
{
	long count = 0;

	for (int i = 0; i < argc; i++)
	{
		cout << argv[i] << endl;
	}
	Generalization_ProgramOpt *programOpt = new Generalization_ProgramOpt(argc, argv);
	cout << "Print command: " << programOpt->GetCommand() << endl;
	cout << "Print file path: " << programOpt->GetFilePath() << endl;
	LPTSTR fileName = _tcsdup(TEXT("file.txt"));

	/*CreateFile(fileName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);*/

	GenDataBase.setFileName(programOpt->GetDataBase());

	GenDataBase.GetDataBaseObjectCount(count);

	cout << "Count of Objects in DB = " << count << endl;
}