#pragma once

#include <windows.h>
#include <iostream>

typedef void *(__cdecl *_BaseOpen)(int *RetCode, char *BaseFileName, char *Function,
	char *Password, char *BaseName);

typedef char *(__cdecl *_BaseOpenTextError)(int err, char *r);

typedef int (__cdecl *_BaseInitObject)(/*GBASE_OBJECT*/void *Object,
				       /*BASE_INT*/void *sem, long mSem,
				       void *Sv, long mSv, void *Pr, long mPr,
				       void *Sq, long mSq, void *Met, long mMet,
				       char *Text, long mText,
				       /*VIDEO_HEADER*/void *Video, long mVideo);

typedef int (__cdecl *_BaseClose)(/*GBASE*/void *base);

typedef void (__cdecl *_BaseInitQuery)(/*GBASE_QUERY*/void *Query,
				       long Key, int Ident);

typedef int (__cdecl *_BaseTransMKO)(char *Source, /*BASE_INT*/void *MKO,
			     int LengthMKO, int *ErrCode,
			     int *ErrorSymbolNumber);

typedef int (__cdecl *_BaseReadObject)(/*GBASE*/void *base, /*GBASE_QUERY*/void *Query,
				       /*BASE_INT*/void *Select, int Fun, int Read,
				       /*GBASE_OBJECT*/void *Object, /*BASE_INT*/void *Code,
				       unsigned long Number);

typedef int (__cdecl *_BaseObjectCount)(/*GBASE*/void *base, /*BASE_INT*/void *Code,
				int reg, long *Count);

typedef int (__cdecl *_BaseCodeStr)(/*BASE_INT*/void *code, char *str);

class GeneralizationDataBaseLoadFuncs
{
private:
	HINSTANCE hinstLib;
public:
	GeneralizationDataBaseLoadFuncs();
	virtual ~GeneralizationDataBaseLoadFuncs();
	_BaseOpen BaseOpen;
	_BaseOpenTextError BaseOpenTextError;
	_BaseClose BaseClose;
	_BaseInitObject BaseInitObject;
	_BaseInitQuery BaseInitQuery;
	_BaseTransMKO BaseTransMKO;
	_BaseReadObject BaseReadObject;
	_BaseObjectCount BaseObjectCount;
	_BaseCodeStr BaseCodeStr;
};

