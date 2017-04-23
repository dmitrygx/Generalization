#pragma once

#include <cstdio>
#include <iostream>
#include <boost/program_options.hpp>
#include <vector>

#include <string>

using namespace std;

namespace po = boost::program_options;

class Generalization_ProgramOpt
{
private:
	string command;
	string file_path;
	string database;
	const char *help_arg = "help,h";
	const char *help_main = "help";
	const char *command_arg = "command,c";
	const char *command_main = "command";
	const char *io_path_arg = "path,p";
	const char *io_path_main = "path";
	const char *database_arg = "database,d";
	const char *database_main = "database";
public:
	Generalization_ProgramOpt(int argc, char* argv[]);
	virtual ~Generalization_ProgramOpt();

	string GetCommand(void)
	{
		return command;
	}

	string GetFilePath(void)
	{
		return file_path;
	}

	string GetDataBase(void)
	{
		return database;
	}
};

