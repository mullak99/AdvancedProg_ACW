// Arc.h 
#pragma once 
#include <string>
#include "Node.h"

using namespace std;

class Node;

class Arc
{
public:
	struct arc
	{
		arc(int linkRef1, int linkRef2, string transportMode);
		int linkref1;
		int linkref2;
		string transportmode;
		
	};

private:
	// Methods and other stuff 
};
