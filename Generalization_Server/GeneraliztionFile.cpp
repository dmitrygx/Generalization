#include "GeneralizationFile.h"
#include <cassert>
#include <iostream>

GeneralizationFile::GeneralizationFile()
{
	path = "C:\\Users\\dgladkov\\Documents\\Education\\CourseWork\\coursework_big.txt";
	pathRes = "C:\\Users\\dgladkov\\Documents\\Education\\CourseWork\\coursework_big.txt";
}

void GeneralizationFile::SetPath(std::string string_path)
{
	path = string_path;
}

uint32_t GeneralizationFile::GetNumberOfCurves()
{
	return numOfCurves;
}

curves GeneralizationFile::GetCurves()
{
	return Curves;
}

curveCoord GeneralizationFile::GetCurveCoordPointsX(uint32_t curve)
{
	assert(curve < numOfCurves);

	return curvesX[curve];
}

curveCoord GeneralizationFile::GetCurveCoordPointsY(uint32_t curve)
{
	assert(curve < numOfCurves);

	return curvesY[curve];
}

uint32_t GeneralizationFile::GetCurveNumberPoints(uint32_t curve)
{
	assert(curve < numOfCurves);

	return pointInCurves[curve];
}

int GeneralizationFile::ParseAllDataInFile()
{
	char separator = ' ';

	System::String ^ gcPath = gcnew System::String(path.c_str());
	array<System::String^>^ lines = System::IO::File::ReadAllLines(gcPath);
	numOfCurves = System::Convert::ToUInt32(lines[0]);

	int k = -1;
	int X = 0;
	int Y = 0;
	bool first = false;

	pointInCurves.resize(numOfCurves);

	curvesX.resize(numOfCurves);
	curvesY.resize(numOfCurves);

	for each (System::String^ str in lines)
	{
		if (!first)
		{
			first = true;
			continue;
		}
		if (str[0] != 'M')
		{
			X = 0;
			Y = 0;
			k++;

			pointInCurves[k] = System::Convert::ToUInt32(str);

			curvesX[k].resize(pointInCurves[k]);
			curvesY[k].resize(pointInCurves[k]);
		}
		else if (str[0] == 'M')
		{
			array<System::String^> ^rawCord = str->Split(separator);
			uint32_t AuxK = 3;
			System::String^ AuxStr = "";

			while (rawCord[0][AuxK] != ')')
			{
				AuxStr += rawCord[0][AuxK];
				AuxK++;
			}

			uint32_t count = (System::Convert::ToUInt32(AuxStr) + 5 < pointInCurves[k]) ?
				(5 * 2 + 1) : ((pointInCurves[k] - System::Convert::ToUInt32(AuxStr) + 1) * 2 + 1);

			for (uint32_t i = 1; i < count; i++)
			{
				if (i % 2 != 0)
				{
					curvesX[k][X] = System::Convert::ToDouble(rawCord[i]);
					X++;
				}
				else
				{
					curvesY[k][Y] = System::Convert::ToDouble(rawCord[i]);
					Y++;
				}
			}
		}
	}

	BuildCurvesMap();

	return 0;
}

void GeneralizationFile::BuildCurvesMap()
{
	for (uint32_t i = 0; i < numOfCurves; i++)
	{
		Curves[i].first = curvesX[i];
		Curves[i].second = curvesY[i];
	}
}


GeneralizationFile::~GeneralizationFile()
{
}
