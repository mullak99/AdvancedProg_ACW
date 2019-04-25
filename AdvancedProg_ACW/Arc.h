// Arc.h 
#pragma once 
#include <string>
using namespace std;

class Node;

class Arc
{
public:
	struct arc
	{
		string transportmode;
		int linkref1;
		int linkref2;
		struct arc* next;
	};
private: Node* m_destination; //node pointer of destination variable 
		 string mode;
		 // Methods and other stuff 
};
