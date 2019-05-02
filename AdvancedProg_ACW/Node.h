// Node.h 
#pragma once
#include <string>
#include <vector>
#include "Arc.h"

using namespace std;

class Node
{
public:
	Node() {}
	struct node
	{
		node(const string &name, const int ref, const double latitude, const double longitude) :
			nodeName(name), refNum(ref), latitude(latitude), longitude(longitude) {}

		string nodeName;
		int refNum;
		double latitude;
		double longitude;
		vector<Arc::arc> nodeArcs;
	};
private:

};

