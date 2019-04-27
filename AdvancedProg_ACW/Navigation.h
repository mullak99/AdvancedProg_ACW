#pragma once

#include <fstream>
#include <string>

class Navigation
{
	std::ofstream _outFile;

	// Add your code here

public:
	Navigation();
	~Navigation();

	bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks);
	bool ProcessCommand(const std::string& commandString);
	bool MaxDist();
	bool MaxLink();
	bool FindDist(const std::string& params);
	bool FindNeighbour(const std::string& params);
	bool Check(const std::string& params);
	bool FindRoute(const std::string& params);
	bool FindShortestRoute(const std::string& params);

	// Add your code here
};