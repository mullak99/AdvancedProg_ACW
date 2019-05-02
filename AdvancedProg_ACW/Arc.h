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
			linkref1(linkRef1), linkref2(linkRef2), transportmode(transportMode) { }

		int linkref1;
		int linkref2;
		string transportmode;
	};

private:
	// Methods and other stuff 
};
