#pragma once
#include "GeneralizationWorkerInvoke.h"

#include <map>

typedef enum command_t
{
	READ_OBJECT = 0,
	WRITE_OBJECT = 1
} command_t;

class GeneralizationWorker :
	public GeneralizationWorkerInvoke
{
private:
	map<command_t, string> cmd_opt;
	const char *command_arg = "--command=";
	const char *io_path_arg = "--path=";
	const char *database_arg = "--database=";
public:
	GeneralizationWorker(const char *progname, command_t cmd,
			     string file_path,
			     string database_path);
	virtual ~GeneralizationWorker();
};

