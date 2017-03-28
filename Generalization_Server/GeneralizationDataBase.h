#pragma once
#include "GeneralizationDataBaseLoadFuncs.h"
//#include "gdbms00f.h"

class GeneralizationDataBase
{
private:
	GeneralizationDataBaseLoadFuncs *DataBaseFunc;
public:
	GeneralizationDataBase();
	virtual ~GeneralizationDataBase();
};

