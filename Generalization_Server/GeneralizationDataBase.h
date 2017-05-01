#pragma once
#include "GeneralizationDataBaseLoadFuncs.h"
#include "common.h"
#include "GeneralizationDataBase.h"
#include <string>
//#include "gdbms00f.h"

class GeneralizationDataBase
{
private:
	std::string FileName;
	uint32_t numOfCurves = 0;
	std::vector<uint32_t> pointInCurves;
	typedef struct CurveId {
		BASE_INT codes[10];
		long Number;
	} CurveId;
	std::vector<CurveId> curvesId;
	curvesCoord curvesX;
	curvesCoord curvesY;
	curves Curves;
	GBASE_OBJECT *dbObject;
	void BuildCurvesMap();

	struct {
		GBASE *dataBase;
	} db;

	boolean running;


	typedef struct SimpleCurve {
		uint32_t count;
		BASE_INT *Xpoints;
		BASE_INT *Ypoints;
		struct {
			BASE_INT Code[10];
			long Number;
		} CurveCode;
	} SimpleCurve;

	typedef struct ObjectCode {
		BASE_INT Code[10];
		long Number;
		uint32_t count;
	} ObjectCode;

public:
	const wchar_t *GetWC(const char *c);
	const char *GetC(const wchar_t *wc);

	GeneralizationDataBaseLoadFuncs *DataBaseFunc;
	GeneralizationDataBase();
	void setFileName(std::string name);
	GeneralizationDataBaseLoadFuncs* getDataBaseFunc();
	std::string* getFileName();
	int ParseAllDataInDB();
	DWORD InitializeDataBase();
	uint32_t GetNumberOfCurves();
	curves GetCurves();
	void ChangeStateOfDataBaseThread(boolean state)
	{
		running = state;
	}
	void SetDataBaseObject(GBASE_OBJECT *obj)
	{
		dbObject = obj;
	}
	GBASE_OBJECT *GetDataBaseObject()
	{
		return dbObject;
	}

	std::string GeneralizationDataBase::DBIntCodeToString(BASE_INT nativeCode[10])
	{
		std::string result;
		uint32_t i = 0;

		result += std::to_string(nativeCode[i]);
		i++;

		while ((i < 10) && (nativeCode[i] != 0))
		{
			result += ".";
			result += std::to_string(nativeCode[i]);
			i++;
		}

		return result;
	}

	std::string GeneralizationDataBase::GetDBCode(uint32_t curveNum)
	{
		return DBIntCodeToString(curvesId[curveNum].codes);
	}

	long GeneralizationDataBase::GetDBNumber(uint32_t curveNum)
	{
		return curvesId[curveNum].Number;
	}

	virtual ~GeneralizationDataBase();

	int OpenDataBase(void);
	int CloseDataBase(void);
	int GetDataBaseObjectCount(long &Count);
	int GetDataBaseObjects(long Count, SimpleCurve *Objects, long &readObjCount);
	int GetDataBaseObjectCodes(long Count, ObjectCode **Codes, long &readObjCount);
	int GetDataBaseObjectByCode(ObjectCode *Code, SimpleCurve **Obj);
};

