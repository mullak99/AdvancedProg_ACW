// Arc.h 
#pragma once 
#include <string>
#include "Node.h"

using namespace std;

class Arc
{
public:
	Arc() {}
	struct arc
	{
		arc(const int linkRef1, const int linkRef2, const string &transportMode) :
			linkRef1(linkRef1), linkRef2(linkRef2), transportMode(transportMode) { }

		int linkRef1, linkRef2;
		string transportMode;
	};

private:
	
};
