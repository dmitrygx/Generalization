#include "Generalization_ProgramOpt.h"

Generalization_ProgramOpt::Generalization_ProgramOpt(int argc, char* argv[])
{
	po::options_description desc("Allowed options");
	desc.add_options()
		(help_arg, "produce help message")
		(command_arg, po::value<string>(&command),
			"optimization level")
		(io_path_arg, po::value<string>(&file_path),
			"path to file for output/input")
		(database_arg, po::value<string>(&database),
			"path to database for read/write");

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

	if (!vm.count(command_main) || !vm.count(io_path_main) || !vm.count(database_main))
	{
		cout << "Incomplete set of arguments" << endl << endl;
		cout << desc << endl;
		exit(0);
	}
}


Generalization_ProgramOpt::~Generalization_ProgramOpt()
{
}
