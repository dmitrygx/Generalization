#include "GeneralizationDataBaseLoadFuncs.h"

using namespace std;

#define LoadFunction(func, type)							\
	func = (type)GetProcAddress(hinstLib, "_BaseOpen");				\
	if (NULL != func)								\
	{										\
		cout << "Error occured during loading \'" << func << "\'. Error is "	\
		     << GetLastError() << endl;						\
		return;									\
	}

GeneralizationDataBaseLoadFuncs::GeneralizationDataBaseLoadFuncs()
{
	hinstLib = LoadLibrary(TEXT("Gdbms01.dll"));
	if (hinstLib != NULL)
	{
		LoadFunction(BaseOpen, _BaseOpen);
		LoadFunction(BaseOpenTextError, _BaseOpenTextError);
		LoadFunction(BaseClose, _BaseClose);
		LoadFunction(BaseInitObject, _BaseInitObject);
		LoadFunction(BaseInitQuery, _BaseInitQuery);
		LoadFunction(BaseReadObject, _BaseReadObject);
		LoadFunction(BaseObjectCount, _BaseObjectCount);
		LoadFunction(BaseCodeStr, _BaseCodeStr);

		cout << "All functions are loaded" << endl;
	}
}

GeneralizationDataBaseLoadFuncs::~GeneralizationDataBaseLoadFuncs()
{
}
