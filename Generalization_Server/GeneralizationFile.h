#pragma once

#include <string>
#include <list>
#include <vector>

class GenaraliztionFile
{
private:
	std::string path;
	std::string pathRes;
	uint64_t numOfCurves = 0;
	std::vector<uint64_t> pointInCurves;
	std::vector<std::vector<double_t>> curvesX;
	std::vector<std::vector<double_t>> curvesY;
public:
	GenaraliztionFile();
	void SetPath(std::string string_path);
	void ParseAllDataInFile();
	virtual ~GenaraliztionFile();
};

