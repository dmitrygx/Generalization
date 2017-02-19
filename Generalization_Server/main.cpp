#include <string>
#include <list>
#include <vector>
#include <iostream>
#include "GeneralizationFile.h"
#include <conio.h>
#include "GeneralizationCurve.h"

using namespace std;

static GenaraliztionFile GenFile;

int main(int argc, char* argv[])
{
	GenFile.ParseAllDataInFile();

	curves Map = GenFile.GetCurves();

	for (uint32_t i = 0; i < GenFile.GetNumberOfCurves(); i++)
	{
		for (uint32_t j = 0; j < Map[i].first.size(); j++)
		{
			cout << Map[i].first[j] << " " << Map[i].second[j] << endl;
		}
	}

	GeneralizationCurve *Curve = new GeneralizationCurve;

	Curve->BuildCurve(Map[0].first.size());

	_getch();
}