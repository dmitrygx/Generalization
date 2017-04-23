#include "GeneralizationWorker.h"

#include <direct.h>
#define GetCurrentDir _getcwd



GeneralizationWorker::GeneralizationWorker(const char *progname, command_t cmd,
	string file_path,
	string database_path)
{
	cmd_opt[command_t::READ_OBJECT] = "read_object";
	cmd_opt[command_t::WRITE_OBJECT] = "write_object";


	char cCurrentPath[FILENAME_MAX];

	if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
	{
		cout << "getcwd failed" << endl;
	}

	cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

	cout << cCurrentPath << endl;

	char *splitted_parts[256];
	int iter = 0;

	splitted_parts[iter++] = strtok(cCurrentPath, "\\");

	while (splitted_parts[iter] = strtok(NULL, "\\"))
	{
		wcout << splitted_parts[iter] << endl;
		iter++;
	}

	char *argv[5];

	string prog;
	string command;
	string path;
	string db;

	for (int i = 0; i < iter; i++)
	{
		prog += splitted_parts[i];
		prog += "\\";
	}

	prog += progname;

	cout << "final = " << prog.c_str() << endl;

	command += command_arg;
	command += (cmd_opt[cmd]).c_str();

	path += io_path_arg;
	path += file_path.c_str();

	db += database_arg;
	db += database_path.c_str();

	argv[0] = _strdup(prog.c_str());
	argv[1] = _strdup(command.c_str());
	argv[2] = _strdup(path.c_str());
	argv[3] = _strdup(db.c_str());
	argv[4] = NULL;

	const wchar_t *wc = GetWC(prog.c_str());
	LPTSTR appName = _tcsdup(wc);

	StartupWorker(appName, 4, argv);

	delete[] wc;

	for (int i = 0; i < 4; i++)
	{
		delete[] argv[i];
	}
}


GeneralizationWorker::~GeneralizationWorker()
{
}
