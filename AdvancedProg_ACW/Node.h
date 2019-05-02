// Node.h 
#pragma once
#include <string>
#include <vector>
#include "Arc.h"

using namespace std;

class Node
{
public: //this is where the functions go 
	Node() {}
	struct node
	{
		node(const string &name, const int ref, const double latitude, const double longitude) :
			Nodename(name), refnum(ref), lat(latitude), longi(longitude) {}

		string Nodename;
		int refnum;
		double lat;
		double longi;
		vector<Arc::arc> m_arcs;
	};
private:

};

