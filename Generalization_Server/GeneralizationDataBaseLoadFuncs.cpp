#include "GeneralizationDataBaseLoadFuncs.h"

using namespace std;

#define LoadFunction(func, type, typeStr)						\
	func = (type)GetProcAddress(hinstLib, typeStr);					\
	if (NULL == func)								\
	{										\
		cout << "Error occured during loading \'" << typeStr << "\'. Error is "	\
		     << GetLastError() << endl;						\
		return;									\
	}

GeneralizationDataBaseLoadFuncs::GeneralizationDataBaseLoadFuncs()
{
	hinstLib = LoadLibrary(TEXT("Gdbms01.dll"));
	if (hinstLib != NULL)
	{
		LoadFunction(_BaseOpen, BaseOpen, "BaseOpen");
		LoadFunction(_BaseOpenTextError, BaseOpenTextError, "BaseOpenTextError");
		LoadFunction(_BaseCloseObject, BaseCloseObject, "BaseCloseObject");
		LoadFunction(_BaseClose, BaseClose, "BaseClose");
		LoadFunction(_BaseInitObject, BaseInitObject, "BaseInitObject");
		LoadFunction(_BaseInitQuery, BaseInitQuery, "BaseInitQuery");
		LoadFunction(_BaseReadObject, BaseReadObject, "BaseReadObject");
		LoadFunction(_BaseObjectCount, BaseObjectCount, "BaseObjectCount");
		LoadFunction(_BaseCodeStr, BaseCodeStr, "BaseCodeStr");
		LoadFunction(_BaseWriteObject, BaseWriteObject, "BaseWriteObject");
		LoadFunction(_BaseDeleteObject, BaseDeleteObject, "BaseDeleteObject");
		LoadFunction(_BaseUpdateMetric, BaseUpdateMetric, "BaseUpdateMetric");

		cout << "All functions are loaded" << endl;
	}
	else
	{
		cout << "An error occured during opening DLL file, error - " << GetLastError() << endl;
	}
}

GeneralizationDataBaseLoadFuncs::~GeneralizationDataBaseLoadFuncs()
{
}
