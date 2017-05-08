#include "GeneralizationProgramOpt.h"
#include "GeneralizationLogging.h"

GeneralizationProgramOpt::GeneralizationProgramOpt(int argc, char* argv[])
{
	server_type_map[standalone] = ServerType::STANDALONE;
	server_type_map[manager] = ServerType::MANAGER;

	output_results = verbose = update_db = false;

	po::options_description desc("Allowed options");
	desc.add_options()
		(help_arg, "produce help message")
		(verbose_arg, "enable verbose mode")
		(output_results_arg, "enable print out of Generalization results")
		(update_arg, "update database by object that was obtained "
			     "from Generalization algorithm")
#ifdef _OPENMP
		(open_mp_support_arg, "enable openmp support")
#endif
		(type_arg, po::value<string>(&type),
			"type of Generalization Server")
		(database_arg, po::value<string>(&database),
			"path to database for read/write")
		(param_C_arg, po::value<string>(&paramC_str),
			"param C")
		(param_Np_arg, po::value<string>(&paramNp_str),
			"param Np")
		(param_Ns_arg, po::value<string>(&paramNs_str),
			"param Ns")
		(param_f_arg, po::value<string>(&paramf_str),
			"param f")
		(param_Ninit_arg, po::value<string>(&paramNinit_str),
			"param Ninit")
		(storage_type_arg, po::value<string>(&storage_type),
			"Storage type: File or DataBase");

	po::variables_map vm;
	po::parsed_options parsed = po::command_line_parser(argc, argv)
		.options(desc)
		.allow_unregistered()
		.run();
	po::store(parsed, vm);
	po::notify(vm);

	if (vm.count(help_main))
	{
		cout << desc << endl;
		exit(0);
	}

	if (vm.count(output_results_main))
	{
		output_results = true;
	}

	if (vm.count(verbose_main))
	{
		verbose = true;
	}

	if (vm.count(update_main))
	{
		update_db = true;
	}

	GeneralizationLogging::SetVerbose(verbose);
	GeneralizationLogging::SetOutputResults(output_results);
	GeneralizationLogging::InitializeLogging("output.txt");

	if (!vm.count(storage_type_main))
	{
		storage_type = default_storage_type;
	}

	if (!vm.count(database_main) && !(type_main))
	{
		cout << "Incomplete set of arguments" << endl << endl;
		cout << desc << endl;
		exit(0);
	}

	if (!vm.count(type_main))
	{
		type = default_type;
	}
	else
	{
		map<string, ServerType>::iterator it = server_type_map.find(type);
		if (it == server_type_map.end())
		{
			// NOT found
			cout << "Incorrect vale was passed, set to default" << default_type << endl;
			type = default_type;
		}
	}
#ifdef _OPENMP
	if (vm.count(open_mp_support_main))
	{
		openmp_support = true;
	}
	else
	{
		openmp_support = false;
	}
#endif

	UpdateValue(vm, C, d);
	UpdateValue(vm, Np, i);
	UpdateValue(vm, Ns, i);
	UpdateValue(vm, f, d);
	UpdateValue(vm, Ninit, i);
}


GeneralizationProgramOpt::~GeneralizationProgramOpt()
{
}
