// Node.h 
#pragma once
#include <string>
#include <vector>
#include "Arc.h"

using namespace std;

//class Arc;

class Node
{
public: //this is where the functions go 
	struct node
	{
		node(string name, int ref, float latitude, float longitude);

		string Nodename;
		int refnum;
		float lat;
		float longitude;
		vector<Arc::arc> m_arcs;
	};
private:

};

