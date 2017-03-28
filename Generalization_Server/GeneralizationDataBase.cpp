#include "GeneralizationDataBase.h"
#include <iostream>

using namespace std;

GeneralizationDataBase::GeneralizationDataBase()
{
	DataBaseFunc = new GeneralizationDataBaseLoadFuncs();
}


GeneralizationDataBase::~GeneralizationDataBase()
{
	delete DataBaseFunc;
}
