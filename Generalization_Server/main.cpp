#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "GeneralizationFile.h"
#include <conio.h>

using namespace std;

static GenaraliztionFile GenFile;

int main(int argc, char* argv[])
{
	GenFile.ParseAllDataInFile();

	curves Map = GenFile.GetCurves();

	for (uint32_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
	{
		cout << "Curve #" << i << endl;
		for (uint32_t j = 0; j < Map[i].first.size(); j++)
		{
			cout << Map[i].first[j] << " " << Map[i].second[j] << endl;
		}
	}

	_getch();
}