#include "GeneralizationFile.h"



GenaraliztionFile::GenaraliztionFile()
{
	path = "C:\\Users\\dgladkov\\Documents\\Education\\CourseWork\\coursework_big.txt";
	pathRes = "C:\\Users\\dgladkov\\Documents\\Education\\CourseWork\\coursework_big.txt";
}

void GenaraliztionFile::SetPath(std::string string_path)
{
	path = string_path;
}

void GenaraliztionFile::ParseAllDataInFile()
{
	char separator = ' ';

	System::String ^ gcPath = gcnew System::String(path.c_str());
	array<System::String^>^ lines = System::IO::File::ReadAllLines(gcPath);
	numOfCurves = System::Convert::ToUInt64(lines[0]);

	int k = -1;
	int X = 0;
	int Y = 0;
	bool first = false;

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
			pointInCurves[k] = System::Convert::ToUInt64(str);

			//System.Windows.Forms.MessageBox.Show(str);
		}
		else if (str[0] == 'M')
		{
			array<System::String^> ^rawCord = str->Split(separator);
			int AuxK = 3;
			std::string AuxStr = "";
			//System.Windows.Forms.MessageBox.Show(str);
			while (rawCord[0][AuxK] != ')')
			{
				AuxStr += rawCord[0][AuxK];
				AuxK++;
			}

			uint64_t count = System::Convert::ToUInt64(AuxStr.c_str()) + 5 < pointInCurves[k] ?
				5 * 2 + 1 : (pointInCurves[k] - System::Convert::ToUInt64(AuxStr.c_str()) + 1) * 2 + 1;

			for (uint64_t i = 1; i < count; i++)
			{
				if (i % 2 != 0)
				{
					curvesX[k][X] = System::Convert::ToInt64(rawCord[i]);
					X++;
				}
				else
				{
					curvesY[k][Y] = System::Convert::ToInt64(rawCord[i]);
					//System.Windows.Forms.MessageBox.Show(CurvesY[k][Y].ToString());
					Y++;
				}
			}
		}
	}
}


GenaraliztionFile::~GenaraliztionFile()
{
}
