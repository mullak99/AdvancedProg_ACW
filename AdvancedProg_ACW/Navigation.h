#pragma once
#include <fstream>
#include <string>

class Navigation
{
	std::ofstream _outFile;

public:
	Navigation();
	~Navigation();

	const void InitFile();

	const bool BuildNetwork(const std::string& fileNamePlaces, const std::string& fileNameLinks) const;
	const bool ProcessCommand(const std::string& commandString) const;
	const bool MaxDist() const;
	const bool MaxLink() const;
	const bool FindDist(const std::string& params) const;
	const bool FindNeighbour(const std::string& params) const;
	const bool Check(const std::string& params) const;
	const bool FindRoute(const std::string& params) const;
	const bool FindShortestRoute(const std::string& params) const;
};