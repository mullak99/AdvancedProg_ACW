// Node.h 
#pragma once
#include <string>
#include <vector>
#include "Arc.h"

using namespace std;

class Node
{
public: //this is where the functions go 
	Node();
	struct node
	{
		node(const string &name, const int ref, const float latitude, const float longitude);

		string Nodename;
		int refnum;
		float lat;
		float longi;
		vector<Arc::arc> m_arcs;
	};
private:

};

