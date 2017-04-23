#pragma once

#include <Windows.h>
#include <cstring>
#include <cstdio>
#include <tchar.h>
#include <string>
#include <iostream>

using namespace std;


class GeneralizationWorkerInvoke
{
private:
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
protected:
	const wchar_t *GetWC(const char *c);
public:
	GeneralizationWorkerInvoke();
	VOID StartupWorker(LPCTSTR lpApplicationName, int argc, char *argv[]);
	virtual ~GeneralizationWorkerInvoke();
};

