#pragma once

#include <cstdio>
#include <iostream>
#include <boost/program_options.hpp>
#include <vector>
#include <map>

#include <string>

using namespace std;

namespace po = boost::program_options;

#define UpdateValue(vm, name, type)								\
do {															\
	if (!vm.count(param_##name##_main))							\
	{															\
		param##name = default_param##name ;						\
	}															\
	else														\
	{															\
		param##name = std::sto##type (param##name##_str);		\
	}															\
} while(0)

#define INIT_GETTER(type, name)	\
type GetParam##name(void)		\
{								\
	return param##name;			\
}

enum ServerType 
{
	STANDALONE	= 0,
	MANAGER		= 1,
	UNDEFINED	= 2
};

class GeneralizationProgramOpt
{
private:
	string type;
	std::map<string, ServerType> server_type_map;
	const string standalone = "standalone";
	const string manager = "manager";
	const string default_type = "manager";

	string database;

	string storage_type;
	const string default_storage_type = "DataBase";
	const string storage_file = "File";
	const string storage_database = "DataBase";

	string paramC_str;
	const double default_paramC = 0.5;
	double paramC = 0;
	string paramNp_str;
	const uint32_t default_paramNp = 500;
	uint32_t paramNp = 0;
	string paramNs_str;
	const uint32_t default_paramNs = 50;
	uint32_t paramNs = 0;
	string paramf_str;
	double default_paramf = 5;
	uint32_t paramf = 0;
	string paramNinit_str;
	const uint32_t default_paramNinit = 1000;
	uint32_t paramNinit = 0;

	const char *help_arg = "help,h";
	const char *help_main = "help";
	const char *type_arg = "type,t";
	const char *type_main = "type";
	const char *database_arg = "database,d";
	const char *database_main = "database";
	const char *storage_type_arg = "storage,s";
	const char *storage_type_main = "storage";
	const char *param_C_arg = "param_C,C";
	const char *param_C_main = "param_C";
	const char *param_Np_arg = "param_Np,P";
	const char *param_Np_main = "param_Np";
	const char *param_Ns_arg = "param_Ns,S";
	const char *param_Ns_main = "param_Ns";
	const char *param_f_arg = "param_f,F";
	const char *param_f_main = "param_f";
	const char *param_Ninit_arg = "param_Ninit,I";
	const char *param_Ninit_main = "param_Ninit";
public:
	GeneralizationProgramOpt(int argc, char* argv[]);
	virtual ~GeneralizationProgramOpt();

	ServerType GetServerType(void)
	{
		return server_type_map[type];
	}

	string GetDataBase(void)
	{
		return database;
	}

	string GetStorageType(void)
	{
		return storage_type;
	}

	INIT_GETTER(double, C);
	INIT_GETTER(uint32_t, Np);
	INIT_GETTER(uint32_t, Ns);
	INIT_GETTER(double, f);
	INIT_GETTER(uint32_t, Ninit);
};

