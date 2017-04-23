#include "GeneralizationWorkerInvoke.h"

GeneralizationWorkerInvoke::GeneralizationWorkerInvoke()
{
}

const wchar_t *GeneralizationWorkerInvoke::GetWC(const char *c)
{
	const size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	mbstowcs(wc, c, cSize);

	return wc;
}

VOID GeneralizationWorkerInvoke::StartupWorker(LPCTSTR lpApplicationName, int argc, char *argv[])
{
	// additional information
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	wstring cmd;

	wcout << lpApplicationName << endl;

	for (int i = 0; i < argc; i++)
	{
		const wchar_t *wc = GetWC(argv[i]);
		cmd += wc;
		cmd += TEXT(" ");
		delete[] wc;
	}

	LPTSTR szCmdline = _tcsdup(cmd.c_str());

	// set the size of the structures
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	wcout << szCmdline << endl;
	// start the program up
	int res = CreateProcess(lpApplicationName,   // the path
		szCmdline,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi             // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
	);
	if (!res)
	{
		cout << "CreateProcess faled with " << GetLastError() << endl;
	}

	WaitForSingleObject(pi.hProcess, INFINITE);
}

GeneralizationWorkerInvoke::~GeneralizationWorkerInvoke()
{
	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}
