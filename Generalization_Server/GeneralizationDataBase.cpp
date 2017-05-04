#include "GeneralizationDataBase.h"
#include <iostream>

using namespace std;

GeneralizationDataBase::GeneralizationDataBase()
{
	DataBaseFunc = new GeneralizationDataBaseLoadFuncs();
}

void GeneralizationDataBase::setFileName(std::string name)
{
	FileName = name;
}

std::string* GeneralizationDataBase::getFileName()
{
	return &FileName;
}

GeneralizationDataBaseLoadFuncs*
GeneralizationDataBase::getDataBaseFunc()
{
	return DataBaseFunc;
}

const wchar_t *GeneralizationDataBase::GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	size_t retVal = 0;
	mbstowcs_s(&retVal, wc, cSize, c, cSize);

	return wc;
}

const char *GeneralizationDataBase::GetC(const wchar_t *wc)
{
	const size_t cSize = wcslen(wc) + 1;
	char* c = new char[cSize];
	size_t retVal = 0;
	wcstombs_s(&retVal, c, cSize, wc, cSize);

	return c;
}

int GeneralizationDataBase::OpenDataBase(void)
{
	int err = 0;
	char *errStr;

	db.dataBase = DataBaseFunc->_BaseOpen(&err,
		const_cast<char*>(
			getFileName()->c_str()),
		"U", NULL,  NULL);
	if (err != 0)
	{
		errStr = DataBaseFunc->_BaseOpenTextError(err, "R");
		wcout << "BaseOpen for \"" << getFileName()->c_str() << "\" failed: " << errStr << endl;
		return -1;
	}

	return err;
}

int GeneralizationDataBase::CloseDataBase(void)
{
	int err;

	err = DataBaseFunc->_BaseClose(db.dataBase);
	if (err != 0)
	{
		wcout << "BaseClose failed: " << endl;
	}

	return err;
}

int GeneralizationDataBase::GetDataBaseObjectCount(long &Count)
{
	int err;
	BASE_INT kod_in[10];

	memset(kod_in, 0, sizeof(kod_in));

	err = DataBaseFunc->_BaseObjectCount(db.dataBase, kod_in,
		OPEN_TREE, &Count);
	if (err != 0)
	{
		wcout << "BaseObjectCount failed: " << endl;
		err = CloseDataBase();
		if (err != 0)
		{
			wcout << "CloseDataBase failed: " << endl;
		}
		return -1;
	}
	return err;
}

int GeneralizationDataBase::GetDataBaseObjects(long Count, SimpleCurve *Objects, long &readObjCount)
{
	GBASE_QUERY Query;
	long objIter = 0;
	int err;
	BASE_INT Code[10] = { 0 };
	GBASE_OBJECT Object;

	DataBaseFunc->_BaseInitQuery(&Query, 32767, 1);

	err = DataBaseFunc->_BaseInitObject(&Object, NULL, 0, NULL, 1,
		NULL, 1, NULL, 0, NULL, 0,
		NULL, 0, NULL, 0);
	if (err != 0)
	{
		wcout << "BaseInitObject failed: " << endl;
		return -1;
	}


	err = DataBaseFunc->_BaseReadObject(db.dataBase, &Query, NULL,
		OPEN_TREE, OBJ_FULL,
		&Object, Code, 0);

	if (err == 0)
	{
		Objects[objIter].count = Object.qMet;
		Objects[objIter].Xpoints = new BASE_INT[Object.qMet];
		Objects[objIter].Ypoints = new BASE_INT[Object.qMet];
		memcpy(Objects[objIter].CurveCode.Code, Object.Code,
		       sizeof(BASE_INT) * 10);
		Objects[objIter].CurveCode.Number = Object.Number;
		long Xiter = 0;
		long Yiter = 0;
		for (long iter = 0; iter < Object.qMet * 4; iter++)
		{
			if (!(iter % 2) && !(iter % 4))
				Objects[objIter].Xpoints[Xiter++] =
					(BASE_INT)((BASE_INT*)Object.pMet)[iter];
			else if (!(iter % 2))
				Objects[objIter].Ypoints[Yiter++] =
					(BASE_INT)((BASE_INT*)Object.pMet)[iter];
		}
		objIter++;
	}
	
	while (err == 0)
	{
		
		DataBaseFunc->_BaseCloseObject(&Object);
		err = DataBaseFunc->_BaseReadObject(db.dataBase, &Query, NULL,
			READ_NEXT, OBJ_FULL,
			&Object, Code, 0);
		if (err == 0)
		{
			Objects[objIter].count = Object.qMet;
			Objects[objIter].Xpoints = new BASE_INT[Object.qMet];
			Objects[objIter].Ypoints = new BASE_INT[Object.qMet];
			memcpy(Objects[objIter].CurveCode.Code, Object.Code,
				sizeof(BASE_INT) * 10);
			Objects[objIter].CurveCode.Number = Object.Number;
			long Xiter = 0;
			long Yiter = 0;
			for (long iter = 0; iter < Object.qMet * 4; iter++)
			{
				if (!(iter % 2) && !(iter % 4))
					Objects[objIter].Xpoints[Xiter++] =
						(BASE_INT)((BASE_INT*)Object.pMet)[iter];
				else if (!(iter % 2))
					Objects[objIter].Ypoints[Yiter++] =
						(BASE_INT)((BASE_INT*)Object.pMet)[iter];
			}
			objIter++;
		}
	}

	// but whatever we need to free memeory that allocated for Count objects.
	readObjCount = objIter;

	return 0;
}

int GeneralizationDataBase::GetDataBaseObjectCodes(long Count, ObjectCode **Codes, long &readObjCount)
{
	GBASE_QUERY Query;
	long objIter = 0;
	int err;
	BASE_INT Code[10] = { 0 };
	GBASE_OBJECT Object;

	DataBaseFunc->_BaseInitQuery(&Query, 32767, 1);

	*Codes = new ObjectCode[Count];
	err = DataBaseFunc->_BaseInitObject(&Object, NULL, 0, NULL, 1,
		NULL, 1, NULL, 0, NULL, 0,
		NULL, 0, NULL, 0);
	if (err != 0)
	{
		wcout << "BaseInitObject failed: " << endl;
		return -1;
	}


	err = DataBaseFunc->_BaseReadObject(db.dataBase, &Query, NULL,
		OPEN_TREE, OBJ_FULL,
		&Object, Code, 0);

	if (err == 0)
	{
		memcpy((*Codes)[objIter].Code, Object.Code, 10 * sizeof(BASE_INT));
		(*Codes)[objIter].count = Object.qSquare;
		(*Codes)[objIter].Number = Object.Number;
		objIter++;
	}

	while (err == 0)
	{

		DataBaseFunc->_BaseCloseObject(&Object);
		err = DataBaseFunc->_BaseReadObject(db.dataBase, &Query, NULL,
			READ_NEXT, OBJ_FULL,
			&Object, Code, 0);
		if (err == 0)
		{
			memcpy((*Codes)[objIter].Code, Object.Code, 10 * sizeof(BASE_INT));
			(*Codes)[objIter].count = Object.qSquare;
			(*Codes)[objIter].Number = Object.Number;
			objIter++;
		}
	}

	// but whatever we need to free memeory that allocated for Count objects.
	readObjCount = objIter;

	return 0;
}

int GeneralizationDataBase::GetDataBaseObjectByCode(ObjectCode *Code, SimpleCurve **Obj)
{
	GBASE_QUERY Query;
	long objIter = 0;
	int err;
	GBASE_OBJECT Object;

	DataBaseFunc->_BaseInitQuery(&Query, 32767, 1);

	err = DataBaseFunc->_BaseInitObject(&Object, NULL, 0, NULL, 1,
		NULL, 1, NULL, 0, NULL, 0,
		NULL, 0, NULL, 0);
	if (err != 0)
	{
		wcout << "BaseInitObject failed: " << endl;
		return -1;
	}

	/*BASE_INT finalCode[10] = { 0 };
	int i = 0;
	for (i = 9; i >= 0; i--)
	{
		if (Code->Code[i] != 0)
			break;
	}
	finalCode[0] = i;
	memcpy(&finalCode[1], Code->Code, 9 * sizeof(BASE_INT));*/

	err = DataBaseFunc->_BaseReadObject(db.dataBase, &Query, NULL,
		OPEN_CODE, OBJ_FULL,
		&Object, Code->Code, Code->Number);
	if (err != 0)
	{
		wcout << "BaseReadObject failed: " << endl;
		return -1;
	}

	*Obj = new SimpleCurve;

	(*Obj)->count = Object.qSquare;
	(*Obj)->Xpoints = new BASE_INT[Object.qSquare];
	(*Obj)->Ypoints = new BASE_INT[Object.qSquare];
	long Xiter = 0;
	long Yiter = 0;
	for (long iter = 0; iter < Object.qSquare * 4; iter++)
	{
		if ((iter % 2) && (iter % 4))
			(*Obj)->Xpoints[Xiter++] = (BASE_INT)((BASE_INT*)Object.pSquare)[iter];
		else if (iter % 2)
			(*Obj)->Ypoints[Yiter++] = (BASE_INT)((BASE_INT*)Object.pSquare)[iter];
	}

	return 0;
}

DWORD GeneralizationDataBase::InitializeDataBase()
{
	int result = 0;
	int err = 0;
	char *errStr;
	GBASE_QUERY Query;
	GBASE *dataBase;
	GBASE_OBJECT Object;
	BASE_INT kod_in[10];
	long Count = 0;

	memset(kod_in, 0, sizeof(kod_in));

	setFileName("C:\\Users\\dgladkov\\Documents\\Education\\61LIN.DPF");
	
	dataBase = DataBaseFunc->_BaseOpen(&err,
		const_cast<char*>(
			getFileName()->c_str()),
		"U", NULL, NULL);
	if (err != 0)
	{
		errStr = DataBaseFunc->_BaseOpenTextError(err, "R");
		wcout << "BaseOpen failed: " << errStr << endl;
		return -1;
	}

	err = DataBaseFunc->_BaseInitObject(&Object, NULL, 0, NULL, 1,
						NULL, 1, NULL, 0, NULL, 0,
						NULL, 0, NULL, 0);
	if (err != 0)
	{
		wcout << "BaseInitObject failed: " << endl;
		return -1;
	}

	err = DataBaseFunc->_BaseObjectCount(dataBase, kod_in,
						 OPEN_TREE, &Count);
	if (err != 0)
	{
		wcout << "BaseObjectCount failed: " << endl;
		return -1;
	}

	DataBaseFunc->_BaseInitQuery(&Query, 32767, 1);

	BASE_INT object1[] = { 3, 6, 3, 1 }; // number = 1
	memset(kod_in, 0, sizeof(BASE_INT) * 10);

	/*Object.pSquare = new BASE_INT[100000];
	Object.qSquare = 100000;*/
	Object.FlagMet = BASE_NEW_FORMAT;

	err = DataBaseFunc->_BaseReadObject(dataBase, &Query, NULL,
						OPEN_CODE, OBJ_FULL,
						&Object, object1, 0);
	if (err != 0)
	{
		wcout << "BaseReadObject failed: Error = " << err << endl;
		if (err == 4) {
			wcout << "The extension of error in BaseReadObject = " <<
				Object.Error[0] << endl;
			wcout << "The Obj number is = " <<
				Object.Number << endl;
		}
		return -1;
	}

	/*for (size_t iter = 0; iter < Object.qSquare * 4; iter++)
	{
	cout << (BASE_INT)((BASE_INT*) Object.pSquare)[iter] << endl;
	}*/

	/*err = DataBaseFunc->BasePrintObjectToFile(
		"output.txt", 0, NULL, &Object,
		OBJ_FULL, NULL, dataBase->CountItemsMet,
		dataBase->SizeItemMet, dataBase->TypeItemMet);
	if (err != 0)
	{
		wcout << "BasePrintObjectToFile failed: Error = " << err << endl;
	}*/

	DataBaseFunc->_BaseCloseObject(&Object);

	err = DataBaseFunc->_BaseClose(dataBase);
	if (err != 0)
	{
		wcout << "BaseClose failed: " << endl;
	}

	return result;
}

int GeneralizationDataBase::ParseAllDataInDB()
{
	int res = 0;
	long count;

	res = OpenDataBase();
	if (res != 0)
	{
		wcout << "OpenDataBase failed: " << endl;
		return -1;
	}

	res = GetDataBaseObjectCount(count);
	if (res != 0)
	{
		wcout << "GetDataBaseObjectCount failed: " << endl;
		return -1;
	}

	SimpleCurve *Objects = new SimpleCurve[count];
	long ObjCount = 0;
	res = GetDataBaseObjects(count, Objects, ObjCount);
	if (res != 0)
	{
		wcout << "GetDataBaseObjects failed: " << endl;
		res = CloseDataBase();
		if (res != 0)
		{
			wcout << "CloseDataBase failed: " << endl;
		}
		return -1;
	}
	cout << "We read " << ObjCount << " objects, and original count is " << count << endl;

	pointInCurves.resize(ObjCount);

	curvesX.resize(ObjCount);
	curvesY.resize(ObjCount);
	curvesId.resize(ObjCount);
	for (long i = 0; i < ObjCount; i++)
	{
		memcpy(curvesId[i].codes, Objects[i].CurveCode.Code,
		       sizeof(*curvesId[i].codes) * 10);
		curvesId[i].Number = Objects[i].CurveCode.Number;
		pointInCurves[i] = Objects[i].count;
		curvesX[i].resize(pointInCurves[i]);
		curvesY[i].resize(pointInCurves[i]);
		for (size_t iter = 0; iter < pointInCurves[i]; iter++)
		{
			curvesX[i][iter] = Objects[i].Xpoints[iter];
			curvesY[i][iter] = Objects[i].Ypoints[iter];
		}
	}

	delete[] Objects;

	numOfCurves = ObjCount;
	BuildCurvesMap();

	res = CloseDataBase();
	if (res != 0)
	{
		wcout << "CloseDataBase failed: " << endl;
		return -1;
	}
	return res;
}

void GeneralizationDataBase::BuildCurvesMap()
{
	for (uint32_t i = 0; i < numOfCurves; i++)
	{
		Curves[i].first = curvesX[i];
		Curves[i].second = curvesY[i];
	}
}

uint32_t GeneralizationDataBase::GetNumberOfCurves()
{
	return numOfCurves;
}

curves GeneralizationDataBase::GetCurves()
{
	return Curves;
}

GeneralizationDataBase::~GeneralizationDataBase()
{
	delete DataBaseFunc;
}
